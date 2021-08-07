#This CMake file is intended to show how the project should be built. While it can be used, a custom build method can also be used.

#normal CMake stuff
cmake_minimum_required(VERSION 3.5)
project(mavapf)

#define vst2 library as interface with headers in deps/vst2 -- linking mavapf against this simply adds deps/vst2 as an include path in mavapf
add_library(vst2 INTERFACE)
target_include_directories(vst2 INTERFACE "deps/vst2")

#define mavapf as a library. Specifically an OBJECT library, b/c linking PLUGIN against it actually just adds mavapf's sources and includes to PLUGIN
add_library(mavapf OBJECT
	"src/plugin.cpp"
	"src/impl/vst2.cpp"
)
target_include_directories(mavapf PUBLIC "include")
target_link_libraries(mavapf PRIVATE vst2) #link vst2 to mavapf (actually just adds include directory, see above @ #define vst2...)

#testing if TESTING isn't defined
if(NOT DEFINED TESTING)
	set(TESTING TRUE)
endif()

#build test if TESTING is true
if(TESTING)
	add_library(pluginTest SHARED "testing/pluginTest.cpp") #tests an implimentation of mavapf
	target_link_libraries(pluginTest mavapf)
endif()