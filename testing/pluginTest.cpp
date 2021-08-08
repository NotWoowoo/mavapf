#include "mavapf/mavapf.h"

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
		setParamLabel(pAmp, "Ampl");
		setParamValue(pAmp, 0.1f);

		setParamLabel(pFreq, "freq");
		setParamValue(pFreq, 0.1f);
	}

	void processAudioBlock(double** inputs, double** outputs, int numSamples) override {
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
