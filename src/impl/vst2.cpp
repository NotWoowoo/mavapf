#include <stdlib.h>
#include <stdio.h> //io only works when testing with a CLI host like mrswatson
#include <aeffectx.h>
#include "mavapf/plugin.h"
#include "mavapf/host.h"

int pluginInstanceCount = 0;

mavapfFunc envInitFunc = nullptr;
mavapfFunc envUninitFunc = nullptr;

int maximumBlockSize = -1;
int sampleRate = -1;
	
audioMasterCallback hostCallback;

VstIntPtr VSTCALLBACK dispatcherProc (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt){
	switch(opcode){
		case effOpen:
			if(pluginInstanceCount == 1 && envInitFunc != nullptr) envInitFunc();
		break;
		
		case effClose:
			delete ((Plugin*) effect->object);
			free(effect);

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
			const char* name = ((Plugin*)effect->object)->getParamLabel(index);
			if(name)
				strncpy((char *)(ptr), name, kVstMaxParamStrLen);
		} break;
			
		case effSetSampleRate:
			sampleRate = (int)opt;
			((Plugin*)effect->object)->sampleRateChanged();
		break;
		
		case effSetBlockSize:
			maximumBlockSize = (int)value;
			((Plugin*)effect->object)->maximumBlockSizeChanged();
		break;

		case effMainsChanged:
			if (value == 0)
				((Plugin*)effect->object)->pluginSwitchedToOff();
			else
				((Plugin*)effect->object)->pluginSwitchedToOn();
		break;
		
		//TODO aeffecx opcodes and editor

		case effGetVstVersion:
			return 2400;
		break;

		case effProcessEvents:
			((Plugin*)(effect->object))->setParamLabel(0, "MIDI");
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

void VSTCALLBACK setParameterProc (AEffect* effect, VstInt32 index, float parameter){
	((Plugin*) effect->object)->setParamValue(index, parameter);
}

float VSTCALLBACK getParameterProc (AEffect* effect, VstInt32 index){
	return ((Plugin*) effect->object)->getParamValue(index);
}

void VSTCALLBACK processProc (AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames){
	//Do this more eloquently later --
	//Have a single array of numIn*sampleFrames doubles
	const int numIn = ((Plugin*) effect->object)->getNumInputChannels();
	const int numOut = ((Plugin*) effect->object)->getNumOutputChannels();
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
	
	((Plugin*) effect->object)->processAudioBlock(dInputs, dOutputs, numFrames);
	
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

void VSTCALLBACK processDoubleProc (AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames){
	((Plugin*) effect->object)->processAudioBlock(inputs, outputs, (int)sampleFrames);
}

extern "C" __declspec(dllexport) AEffect *VSTPluginMain(audioMasterCallback vstHostCallback){
	++pluginInstanceCount;
	Plugin *pluginInstance = createPluginInstance();
	
	AEffect *effectInstance = (AEffect *)malloc(sizeof(AEffect));
	if(!effectInstance) return nullptr;

	setInternalPluginInstance(pluginInstance, effectInstance);

	hostCallback = vstHostCallback;

	effectInstance->magic = kEffectMagic;
	
	effectInstance->dispatcher = dispatcherProc;
	effectInstance->setParameter = setParameterProc;
	effectInstance->getParameter = getParameterProc;
	effectInstance->processReplacing = processProc;
	effectInstance->processDoubleReplacing = processDoubleProc;
	
	effectInstance->numPrograms = 0;   
	effectInstance->numParams = pluginInstance->getNumParams();		
	effectInstance->numInputs = pluginInstance->getNumInputChannels();		
	effectInstance->numOutputs = pluginInstance->getNumOutputChannels();	
	
	effectInstance->flags = effFlagsCanReplacing | effFlagsCanDoubleReplacing;
	
	effectInstance->initialDelay = 0;
	
	effectInstance->object = pluginInstance;
	//effectInstance->user = NULL;
	
	//effectInstance->uniqueID = 'woow';
	effectInstance->version = 1000;
	
	return effectInstance;
}

//host.h IMPLIMENTATION//.......................

void setEnvironmentInitFunc(mavapfFunc func) {
	envInitFunc = func;
}

void setEnvironmentUninitFunc(mavapfFunc func) {
	envUninitFunc = func;
}

void setInternalPluginInstance(Plugin *plugin, void *internalInstance) {
	plugin->internalPluginInstance = internalInstance;
}

void notifyParameterChange(Plugin *plugin, int index, float value) {
	if(hostCallback != nullptr)
		hostCallback((AEffect*)plugin->internalPluginInstance, audioMasterAutomate, index, 0, 0, value);
}

int getNumOpenPluginInstances() {
	return pluginInstanceCount;
}

int getSampleRate() {
	return sampleRate;
}

int getMaximumBlockSize() {
	return maximumBlockSize;
}