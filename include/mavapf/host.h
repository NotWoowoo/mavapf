// host.h contains undefined functions for the host to impliment.
// The plugin can use some of these functions, but most of them have an equivilent in class Plugin.
// The reason for equivilant functions in plugin.h and host.h is so the host only has to impliment host.h. Likewise, plugins only have to impliment plugin.h

#pragma once

class Plugin; // forward declaration of Plugin to use as an opaque handle. Plugin uses functions in host.h, so we can't #include "plugin.h" 

//--These functions are used by base class Plugin--//

/// Tells the host that a plugin's parameter has changed.
/// @param plugin - the plugin instance whose parameter changed
/// @param index - the index of the parameter changed
/// @param value - the new value of the changed parameter
void hostNotifyParameterChange(Plugin *plugin, int index, float value);

/// Determines whether an input or output channel is enabled or not.
/// @param plugin - the plugin to check the channel for
/// @param isInput - whether we are checking the input channel, if not then the output channel is checked
/// @param index - index of the channel to check
/// @return whether the channel is enabled
bool hostGetChannelStatus(Plugin *plugin, bool isInput /*otherwise output*/, int index);

/// Gets the sample rate that the host is using for a plugin. 
/// The host could use different sample rates for different plugins.
/// @param plugin - the plugin to check sample rate for
/// @return sample rate
int hostGetSampleRate(Plugin *plugin);

/// Gets the maximum size for the processing blocks used by a plugin. 
/// The host could use differnt maximum block sizes for different plugins
/// @param plugin - the plugin to check the maximum block size for
/// @return the maximum block size in samples
int hostGetMaximumBlockSize(Plugin* plugin);

//--Remaining functions can be called in subclasses of Plugin--//

/// A void return no parameter function pointer. 
typedef void (*mavapfFunc)(void);

/// Sets the init function for all instances of a plugin. 
/// Runs once when the first instance of a plugin is opened. 
/// ex: to set up shared memory between plugin instances
/// @param func - a void return, no parameter function to be called
void setEnvironmentInitFunc(mavapfFunc func);

/// Sets the uninit function for all instances of a plugin. 
/// Runs once when the last instance of a plugin is closed. 
/// ex: to clean up shared memory between plugin instances
/// @param func - a void return, no parameter function to be called
void setEnvironmentUninitFunc(mavapfFunc func);

/// Gets the number of open plugin instances. 
/// ex: if you only want to allow one instance of a plugin to run in a host at a time
/// @return number of open plugin instances
int hostGetNumOpenPluginInstances();