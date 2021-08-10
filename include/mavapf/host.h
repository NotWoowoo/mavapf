#pragma once

class Plugin;

typedef void (*mavapfFunc)(void);

void setEnvironmentInitFunc(mavapfFunc func);
void setEnvironmentUninitFunc(mavapfFunc func);

//These functions are used by base class Plugin
void hostNotifyParameterChange(Plugin *plugin, int index, float value);

bool hostGetChannelStatus(Plugin *plugin, bool isInput /*otherwise output*/, int index);

int hostGetSampleRate(Plugin *plugin);
int hostGetMaximumBlockSize(Plugin* plugin);

//Remaining functions can be called in subclasses of Plugin

int hostGetNumOpenPluginInstances();