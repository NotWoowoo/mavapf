#include "mavapf/mavapf.h"

#include <math.h>

enum myParams{
	pMix,
	
	pNumParams
};

static float clamp(float f, float low, float high) {
	return fmin(high, fmax(low, f));
}

class myPlugin : public Plugin {
public:
	myPlugin()
		: Plugin(pNumParams, 2, 2)
	{
		setParamLabel(pMix, "mix");
		setParamValue(pMix, 0.5f);

		//setParamLabel(pFreq, "freq");
		//setParamValue(pFreq, 0.1f);
	}

	void processAudioBlock(double** inputs, double** outputs, int numSamples) override {
		double *inL = inputs[0];
		double *inR = inputs[1];
		
		double *outL = outputs[0];
		double *outR = outputs[1];

		for(int i = 0; i < numSamples; ++i){
			float mono = (inL[i] + inR[i]) / 2.f;
			float mix = getParamValue(pMix);

			float leftAmp = (-1*clamp(mono, -1.f, 0.f) == 0) ? mix : 1-mix;
			float rightAmp = (clamp(mono, 0.f, 1.f) == 0) ? mix : 1-mix;

			outL[i] = inL[i] * leftAmp;
			outR[i] = inR[i] * rightAmp;
		}
	}

};

Plugin *createPluginInstance(){
	return new myPlugin(); //delete called by host
}
