#include <stdlib.h>
#include <stdio.h>
#include <aeffectx.h>
#include "mavapf/plugin.h"

#include <windows.h>
#include <math.h>
	
float param = 0.5;
float phase = 0;
	
audioMasterCallback hostCallback;

VstIntPtr VSTCALLBACK dispatcherProc (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt){
	switch(opcode){
		case effOpen:
			
		break;
		
		case effClose:
			free(effect);
		break;
		
		case effGetPlugCategory:
			return kPlugCategEffect;
	 
		// request for the vendor string (usually used in the UI for plugin grouping)
		case effGetVendorString:
			strncpy((char *)(ptr), "woow", kVstMaxVendorStrLen);
		break;
	 
		// request for the version
		case effGetVendorVersion:
			return 1000;
			
		case effSetSampleRate:
			printf("sample rate: %f \n", opt);
		break;
		
		case effSetBlockSize:
			printf("block size: %i \n", value);
		break;
		
		case effCanBeAutomated:
			return 1;
			
		case effEditClose:
			
		break;
			
		// ignoring all other opcodes
		default:
			printf("Unknown opCode %i [ignored] \n", opcode);
		break;
	}
	
	return 0;
}

void VSTCALLBACK setParameterProc (AEffect* effect, VstInt32 index, float parameter){
	param = parameter;
}

float VSTCALLBACK getParameterProc (AEffect* effect, VstInt32 index){
	return param;
}

void VSTCALLBACK processProc (AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames){
	float *inL = inputs[0];
	float *inR = inputs[1];
	
	float *outL = outputs[0];
	float *outR = outputs[1];
	
	for(int i = 0; i < sampleFrames; ++i){
		outL[i]=outR[i] = 0.1*sinf(phase);
		phase += param*16*440/44100.f;
	}
}

void VSTCALLBACK processDoubleProc (AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames){
	
}

extern "C" __declspec(dllexport) AEffect *VSTPluginMain(audioMasterCallback vstHostCallback){
	hostCallback = vstHostCallback;
	
	AEffect *effectInstance = (AEffect *)malloc(sizeof(AEffect));
	effectInstance->magic = kEffectMagic;
	
	effectInstance->dispatcher = dispatcherProc;
	effectInstance->setParameter = setParameterProc;
	effectInstance->getParameter = getParameterProc;
	effectInstance->processReplacing = processProc;
	effectInstance->processDoubleReplacing = processDoubleProc;
	
	effectInstance->numPrograms = 0;   
	effectInstance->numParams = 1;		
	effectInstance->numInputs = 2;		
	effectInstance->numOutputs = 2;	
	
	effectInstance->flags = effFlagsCanReplacing | effFlagsHasEditor;
	
	effectInstance->initialDelay = 0;
	
	effectInstance->object = effectInstance;
	//effectInstance->user = NULL;
	
	//effectInstance->uniqueID = 'woow';
	effectInstance->version = 1000;
	
	return effectInstance;
}