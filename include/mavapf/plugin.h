#pragma once

typedef void (*mavapfFunc)(void);

class Plugin{
public:
	Plugin(int numParams, int numInputChannels = 2, int numOutputChannels = 2);
	
	virtual ~Plugin();
	
	virtual void processAudioBlock(double** inputs, double** outputs, int numSamples);
	
	virtual int getNumInputChannels() final;
	virtual int getNumOutputChannels() final;
	
	virtual int getNumParams() final;

	virtual void hostNotifyParameterChange(int index, float value) final;

	virtual void setParamValue(int index, float value) final;
	virtual float getParamValue(int index) final;

	virtual void setParamLabel(int index, const char *label) final;
	virtual const char *getParamLabel(int index) final;

	virtual void setParamUnits(int index, const char* units) final;
	virtual const char* getParamUnits(int index) final;

	
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
};

Plugin *createPluginInstance();

void setEnvironmentInitFunc(mavapfFunc func);
void setEnvironmentUninitFunc(mavapfFunc func);