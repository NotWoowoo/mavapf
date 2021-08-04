#include "mavapf/plugin.h"

Plugin::Plugin(int numParams, int numInputChannels, int numOutputChannels)
	: numParams(numParams), numInputChannels(numInputChannels), numOutputChannels(numOutputChannels)
{
	parameters = new Parameter[numParams];
}

Plugin::~Plugin(){
	delete parameters;
}

void Plugin::processAudioBlock(double** inputs, double** outputs, int numSamples){
	double *inL = inputs[0];
	double *inR = inputs[1];
	
	double *outL = outputs[0];
	double *outR = outputs[1];
	
	for(int i = 0; i < numSamples; ++i){
		outL[i] = inL[i];
		outR[i] = inR[i];
	}
}