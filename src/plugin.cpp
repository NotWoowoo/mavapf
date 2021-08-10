#include "mavapf/plugin.h"
#include "mavapf/host.h"

Plugin::Plugin(int numParams, int numInputChannels, int numOutputChannels)
	: numParams(numParams), numInputChannels(numInputChannels), numOutputChannels(numOutputChannels), internalPluginInstance(nullptr)
{
	parameters = new Parameter[numParams];
}

Plugin::~Plugin(){
	delete[] parameters;
}

void Plugin::processAudioBlock(double** inputs, double** outputs, int numSamples) {}

void Plugin::sampleRateChanged() {}
void Plugin::maximumBlockSizeChanged() {}

void Plugin::pluginSwitchedToOn() {}
void Plugin::pluginSwitchedToOff() {}

int Plugin::getNumInputChannels() {
	return numInputChannels;
}

int Plugin::getNumOutputChannels() {
	return numOutputChannels;
}

int Plugin::getNumParams() {
	return numParams;
}

void Plugin::setParamValue(int index, float value) {
	notifyParameterChange(this, index, value);
	parameters[index].value = value;
}

float Plugin::getParamValue(int index) {
	return parameters[index].value;
}

void Plugin::setParamLabel(int index, const char* label) {
	parameters[index].label = label;
}

const char* Plugin::getParamLabel(int index) {
	return parameters[index].label;
}

void Plugin::setParamUnits(int index, const char* units) {
	parameters[index].units = units;
}

const char* Plugin::getParamUnits(int index) {
	return parameters[index].units;
}