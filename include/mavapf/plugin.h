#pragma once
#include <mavapf/host.h>

class Plugin{
public:
	Plugin(int numParams, int numInputChannels = 2, int numOutputChannels = 2);
	
	virtual ~Plugin();
	
	virtual void processAudioBlock(double** inputs, double** outputs, int numSamples);

	virtual void sampleRateChanged();
	virtual void maximumBlockSizeChanged();

	virtual void pluginSwitchedToOn();
	virtual void pluginSwitchedToOff();
	
	virtual int getNumInputChannels() final;
	virtual int getNumOutputChannels() final;

	virtual bool isInputChannelEnabled(int index) final;
	virtual bool isOutputChannelEnabled(int index) final;
	
	virtual int getNumParams() final;

	virtual void setParamValue(int index, float value) final;
	virtual float getParamValue(int index) final;

	virtual void setParamLabel(int index, const char *label) final;
	virtual const char *getParamLabel(int index) final;

	virtual void setParamUnits(int index, const char* units) final;
	virtual const char* getParamUnits(int index) final;

	virtual void setLatency(int numSamples) final; //TODO something in aeffectx has to be done for this to work when called outside of setup
	virtual int getLatency() final;

	virtual int getSampleRate() final;
	virtual int getMaximumBlockSize() final;

	void *hostInfo; //to be used by host for any purpose - initialized to nullptr

private:
	int numInputChannels;
	
	int numOutputChannels;
	
	struct Parameter {
		float value = 0.5;
		const char *label = nullptr;
		const char *units = nullptr;
	};

	int numParams;
	
	Parameter *parameters;

	int numSamplesLatency;
};

Plugin *createPluginInstance();