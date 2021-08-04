#include "mavapf/mavapf.h"

enum myParams{
	pSamples,
	pFreq,
	
	pNumParams
};

class myPlugin : public Plugin {
public:
	myPlugin()
		: Plugin(pNumParams, 3, 4)
	{
		//impl
		parameters[pFreq].setLabel("freq");
	}
	
	//void processAudioBlock(double** inputs, double** outputs, int numSamples) override{
		//impl
	//}
};

Plugin *createPluginInstance(){
	return new myPlugin();
}
