// plugin.h contains an undefined function for the plugin to impliment, and a Plugin class to extend
// The host only needs to be able to interact with class Plugin, and to call Plugin *createPluginInstance()

#pragma once
#include <mavapf/host.h>

/// Uses:
/// to store plugin instance data
/// to interface with a plugin
/// to be a base class for plugin implimentation
/// 
/// inherit this class and return it in Plugin *createPluginInstance() to impliment a plugin
class Plugin{
public:
	/// Call in subclass to initialize plugin. 
	/// The number of channels can't be changed after initialization (could be changed in future under some circumstances).
	/// @param numParams - the number of parameters for this plugin
	/// @param numInputChannels - the number of audio input channels
	/// @param numOutputChannels - the number of audio output channels
	Plugin(int numParams, int numInputChannels = 2, int numOutputChannels = 2);
	
	virtual ~Plugin();
	
	/// Impliment to process audio
	/// @param inputs - audio data to process - access via inputs[channel_number][sample_index]
	/// @param outputs - where to write output data - access via outputs[channel_number][sample_index]
	/// @param numSamples - the number of samples in inputs and outputs
	virtual void processAudioBlock(double **inputs, double **outputs, int numSamples);

	
	virtual void sampleRateChanged();
	virtual void maximumBlockSizeChanged();

	virtual void pluginSwitchedToOn(); 
	virtual void pluginSwitchedToOff();
	
	virtual int getNumInputChannels() final;
	virtual int getNumOutputChannels() final;
	
	// These functions are only guaranteed to work when called outside of the constructor
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

	///to be used by host for any purpose - initialized to nullptr by plugin
	void *hostInfo;

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