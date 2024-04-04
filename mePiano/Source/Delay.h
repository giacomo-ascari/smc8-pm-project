#pragma once

#include <JuceHeader.h>

// 96kHz, 20Hz wave period = 4800
#define DELAY_DEF_SIZE 5000

class Delay
{
public:
	Delay();
	~Delay();
	void setLength(float l);
	void resizeMemory(int s);
	float process(float x);
	void pushSample(float x);
	float getSample();

private:
	int size;
	int cursor;
	int lengthInt; // integer part of the delay length 
	float lengthDec; // decimal part of the delay length
	bool isFractional; // indicates if the delay is fractional
	float* memory;
};


