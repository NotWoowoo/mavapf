#include <stdio.h> //io only works when testing with a CLI host like mrswatson
#include <aeffectx.h>
#include "mavapf/plugin.h"
#include "mavapf/host.h"

static audioMasterCallback hostCallback;

static int pluginInstanceCount = 0;

static mavapfFunc envInitFunc = nullptr;
static mavapfFunc envUninitFunc = nullptr;

struct pluginWrapper {
	pluginWrapper(){
		plugin = createPluginInstance();
		vstEffect = {0};

		plugin->hostInfo = this;

		channelStats = new bool[plugin->getNumInputChannels() + plugin->getNumOutputChannels()];

		vstEffect.magic = kEffectMagic;

		vstEffect.numPrograms = 0;
		vstEffect.numParams = plugin->getNumParams();
		vstEffect.numInputs = plugin->getNumInputChannels();
		vstEffect.numOutputs = plugin->getNumOutputChannels();

		vstEffect.flags = effFlagsCanReplacing | effFlagsCanDoubleReplacing;

		vstEffect.initialDelay = plugin->getLatency();

		vstEffect.object = plugin;
		//vstEffect.user = NULL;

		//TODO - have a way to set unique ID in the least platform specific way. Maybe do it after other formats have some implimentation
		//vstEffect.uniqueID = 0;
		vstEffect.version = 1000;
	}

	~pluginWrapper() {
		delete plugin;
		delete[] channelStats;
	}

	AEffect *getAEffect() { return &vstEffect; }
	Plugin *getPlugin() { return plugin; }

	Plugin *plugin;
	AEffect vstEffect;

	int maximumBlockSize = -1;
	int sampleRate = -1;

	bool *channelStats; //whether input and output channels are enabled
};

static pluginWrapper* AEff2Wrapper(AEffect* e) { return (pluginWrapper*)((Plugin*)e->object)->hostInfo; }
static pluginWrapper* plug2Wrapper(Plugin* p) { return (pluginWrapper*)p->hostInfo; }

static AEffect* plug2AEff(Plugin* p) { return plug2Wrapper(p)->getAEffect(); }
static Plugin* AEff2Plug(AEffect* e) { return AEff2Wrapper(e)->getPlugin(); }

static VstIntPtr VSTCALLBACK dispatcherProc (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt){
	switch(opcode){
		case effOpen:
			//TODO pluginOpened and closed
			if(pluginInstanceCount == 1 && envInitFunc != nullptr) envInitFunc();
		break;
		
		case effClose:
			delete AEff2Wrapper(effect);

			if (pluginInstanceCount == 1 && envUninitFunc != nullptr) envUninitFunc();
			--pluginInstanceCount;
		break;
		
		//case effGetParamLabel:
			//strncpy((char *)(ptr), "label", kVstMaxParamStrLen);
		//break;
		
		//case effGetParamDisplay:
			//strncpy((char *)(ptr), "disp", kVstMaxParamStrLen);
		//break;
		
		case effGetParamName: {
			const char* name = AEff2Plug(effect)->getParamLabel(index);
			if(name)
				strncpy((char *)(ptr), name, kVstMaxParamStrLen);
		} break;
			
		case effSetSampleRate:
			AEff2Wrapper(effect)->sampleRate = (int)opt;
			AEff2Plug(effect)->sampleRateChanged();
		break;
		
		case effSetBlockSize:
			AEff2Wrapper(effect)->maximumBlockSize = (int)value;
			AEff2Plug(effect)->maximumBlockSizeChanged();
		break;

		case effMainsChanged:
			if (value == 0){
				AEff2Plug(effect)->pluginSwitchedToOff();
			}else{
				if (hostCallback != nullptr) {
					for (int i = 0; i < AEff2Plug(effect)->getNumInputChannels() + AEff2Plug(effect)->getNumOutputChannels(); ++i) {
						bool isInput = i < AEff2Plug(effect)->getNumInputChannels();
						AEff2Wrapper(effect)->channelStats[i] = (hostCallback(effect, audioMasterPinConnected, index - (isInput ? 0 : AEff2Plug(effect)->getNumInputChannels()), isInput, 0, 0) == 0);
					}
				}

				AEff2Plug(effect)->pluginSwitchedToOn();
			}
		break;
		
		//TODO aeffecx opcodes and editor

		case effGetVstVersion:
			return 2400;
		break;

		case effProcessEvents:
			AEff2Plug(effect)->setParamLabel(0, "MIDI");
			return 1;

		case effCanBeAutomated:
			return 1;

		case effGetPlugCategory:
			return kPlugCategSynth;
			
		// ignoring all other opcodes
		default:
			printf("Unknown opCode %i [ignored] \n", opcode);
		break;
	}
	
	return 0;
}

static void VSTCALLBACK setParameterProc (AEffect* effect, VstInt32 index, float parameter){
	AEff2Plug(effect)->setParamValue(index, parameter);
}

static float VSTCALLBACK getParameterProc (AEffect* effect, VstInt32 index){
	return AEff2Plug(effect)->getParamValue(index);
}

static void VSTCALLBACK processProc (AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames){
	//Do this more eloquently later --
	//Have a single array of numIn*sampleFrames doubles
	const int numIn = AEff2Plug(effect)->getNumInputChannels();
	const int numOut = AEff2Plug(effect)->getNumOutputChannels();
	const int numFrames = sampleFrames;
	
	double **dInputs = new double*[numIn];
	for(int i = 0; i < numIn; ++i)
		dInputs[i] = new double[numFrames];
	
	double **dOutputs = new double*[numOut];
	for(int i = 0; i < numOut; ++i)
		dOutputs[i] = new double[numFrames];
	
	for(int n = 0; n < numIn; ++n)
		for(int s = 0; s < numFrames; ++s)
			dInputs[n][s] = (double)inputs[n][s];
	
	AEff2Plug(effect)->processAudioBlock(dInputs, dOutputs, numFrames);
	
	for(int n = 0; n < numOut; ++n)
		for(int s = 0; s < numFrames; ++s)
			outputs[n][s] = (float)dOutputs[n][s];

	for(int n = 0; n < numIn; ++n)
		delete[] dInputs[n];

	for(int n = 0; n < numOut; ++n)
		delete[] dOutputs[n];

	delete[] dInputs;
	delete[] dOutputs;
}

static void VSTCALLBACK processDoubleProc (AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames){
	AEff2Plug(effect)->processAudioBlock(inputs, outputs, (int)sampleFrames);
}

extern "C" __declspec(dllexport) AEffect *VSTPluginMain(audioMasterCallback vstHostCallback){
	++pluginInstanceCount; //call before creating pluginInstance

	pluginWrapper* pluginInstance = new pluginWrapper();

	pluginInstance->vstEffect.dispatcher = dispatcherProc;
	pluginInstance->vstEffect.setParameter = setParameterProc;
	pluginInstance->vstEffect.getParameter = getParameterProc;
	pluginInstance->vstEffect.processReplacing = processProc;
	pluginInstance->vstEffect.processDoubleReplacing = processDoubleProc;

	hostCallback = vstHostCallback;
	
	return &pluginInstance->vstEffect;
}

//host.h IMPLIMENTATION//.......................

void setEnvironmentInitFunc(mavapfFunc func) {
	envInitFunc = func;
}

void setEnvironmentUninitFunc(mavapfFunc func) {
	envUninitFunc = func;
}

void hostNotifyParameterChange(Plugin *plugin, int index, float value) {
	if(hostCallback != nullptr)
		hostCallback(plug2AEff(plugin), audioMasterAutomate, index, 0, 0, value);
}

int hostGetNumOpenPluginInstances() {
	return pluginInstanceCount;
}

int hostGetSampleRate(Plugin *plugin) {
	return plug2Wrapper(plugin)->sampleRate;
}

int hostGetMaximumBlockSize(Plugin *plugin) {
	return plug2Wrapper(plugin)->maximumBlockSize;
}

bool hostGetChannelStatus(Plugin* plugin, bool isInput, int index) {
	return plug2Wrapper(plugin)->channelStats[index + (isInput ? 0 : plugin->getNumInputChannels())];
}