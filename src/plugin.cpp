#include "mavapf/plugin.h"

mavapfFunc envInitFunc = nullptr;
mavapfFunc envUninitFunc = nullptr;

void setEnvironmentInitFunc(mavapfFunc func) {
	envInitFunc = func;
}

void setEnvironmentUninitFunc(mavapfFunc func) {
	envUninitFunc = func;
}

Plugin::Plugin(int numParams, int numInputChannels, int numOutputChannels)
	: numParams(numParams), numInputChannels(numInputChannels), numOutputChannels(numOutputChannels)
{
	parameters = new Parameter[numParams];
}

Plugin::~Plugin(){
	delete[] parameters;
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

int Plugin::getNumInputChannels() {
	return numInputChannels;
}

int Plugin::getNumOutputChannels() {
	return numOutputChannels;
}

int Plugin::getNumParams() {
	return numParams;
}

Parameter *Plugin::getParam(int index) {
	return &(parameters[index]);
}