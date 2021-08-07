#include "mavapf/mavapf.h"

#include <Windows.h>

enum myParams{
	pAmp,
	pFreq,
	
	pNumParams
};

class myPlugin : public Plugin {
public:
	myPlugin()
		: Plugin(pNumParams, 2, 2)
	{
		getParam(pAmp)->setLabel("Ampl");
		getParam(pAmp)->setValue(0.1f);
		getParam(pFreq)->setLabel("freq");
		getParam(pFreq)->setValue(0.1);
	}

	void processAudioBlock(double** inputs, double** outputs, int numSamples) override{
		double *inL = inputs[0];
		double *inR = inputs[1];
		
		double *outL = outputs[0];
		double *outR = outputs[1];
		
		for(int i = 0; i < numSamples; ++i){
			outL[i] = inL[i];
			outR[i] = inR[i];
		}
	}
};

Plugin *createPluginInstance(){
	return new myPlugin();
}
