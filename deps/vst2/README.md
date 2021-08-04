The VST2 format is licenced by Steinberg, so the necessary files can't be included in this repository. However, you can download the vst2 sdk from https://archive.org/details/VST2SDK

The files you need are called aeffect.h and aeffectx.h, both of which can be found in pluginterfaces/vst2.x within the vst2 sdk.
Copy aeffect.h and aeffectx.h into this folder so that the file structure of mavapf has:

	mavapf/deps/vst2/aeffect.h
	mavapf/deps/vst2/aeffectx.h