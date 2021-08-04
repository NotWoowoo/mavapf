#include "mavapf/parameter.h"

Parameter::Parameter(){
	value = 0.5;
	label = nullptr;
	units = nullptr;
}

void Parameter::setValue(float v){
	value = v;
}

float Parameter::getValue(){
	return value;
}

void Parameter::setLabel(const char *l){
	label = l;
}

const char *Parameter::getLabel(){
	return label;
}

void Parameter::setUnits(const char *u){
	units = u;
}

const char *Parameter::getUnits(){
	return units;
}