#pragma once

#include "mavapf/parameter.h"

class Plugin{
public:
	Plugin(int numParams, int numInputChannels = 2, int numOutputChannels = 2);
	
	~Plugin();
	
	virtual void processAudioBlock(double** inputs, double** outputs, int numSamples);
	
	
	virtual int getNumInputChannels() final;
	virtual int getNumOutputChannels() final;
	
	virtual int getNumParams() final;
	
	virtual Parameter *getParam(int index) final;
	
private:
	int numInputChannels;
	
	int numOutputChannels;
	
	int numParams;
	
	Parameter *parameters;
};

Plugin *createPluginInstance();