#include "mavapf/mavapf.h"

enum myParams{
	pSamples,
	pFreq,
	
	pNumParams
};

class myPlugin : public Plugin {
public:
	myPlugin()
		: Plugin(pNumParams, 2, 2)
	{
		getParam(pFreq)->setLabel("freq");
	}
	
	void processAudioBlock(double** inputs, double** outputs, int numSamples) override{
		double *inL = inputs[0];
		double *inR = inputs[1];
		
		double *outL = outputs[0];
		double *outR = outputs[1];
		
		for(int i = 0; i < numSamples; ++i){
			outL[i] = inL[i] * getParam(pFreq)->getValue();
			outR[i] = inR[i] * getParam(pFreq)->getValue();
		}
	}
};

Plugin *createPluginInstance(){
	return new myPlugin();
}
