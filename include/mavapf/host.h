#pragma once

class Plugin;

typedef void (*mavapfFunc)(void);

void setEnvironmentInitFunc(mavapfFunc func);
void setEnvironmentUninitFunc(mavapfFunc func);

void setInternalPluginInstance(Plugin *plugin, void *internalInstance);

void notifyParameterChange(Plugin *plugin, int index, float value);

int getNumOpenPluginInstances();