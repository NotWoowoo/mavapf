#pragma once

class Plugin;

typedef void (*mavapfFunc)(void);

void setEnvironmentInitFunc(mavapfFunc func);
void setEnvironmentUninitFunc(mavapfFunc func);

//These functions are used by base class Plugin

void setInternalPluginInstance(Plugin *plugin, void *internalInstance);

void notifyParameterChange(Plugin *plugin, int index, float value);

//Remaining functions can be called in subclasses of Plugin

int getNumOpenPluginInstances();

int getSampleRate();
int getMaximumBlockSize();