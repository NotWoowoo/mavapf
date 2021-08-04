#pragma once

#include "mavapf/parameter.h"

class Plugin{
public:

	Plugin(int numParams, int numInputChannels = 2, int numOutputChannels = 2);
	
	~Plugin();
	
	virtual void processAudioBlock(double** inputs, double** outputs, int numSamples);
	
	
	const int numInputChannels;
	
	const int numOutputChannels;
	
	const int numParams;
	
	Parameter* parameters;
};

Plugin *createPluginInstance();