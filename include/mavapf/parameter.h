#pragma once

class Parameter{
public:
	Parameter();
	
	void setValue(double value);
	float getValue();
	
	void setLabel(const char *label);
	const char *getLabel();
	
	void setUnits(const char *units);
	const char *getUnits();

private:
	float value;
	const char *label;
	const char *units;
};