#pragma once

#include <JuceHeader.h>

// ========================================================================================
// Delay

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

// ========================================================================================
// Bi-quad filter

#define FILTER_BUFFER_SIZE 2

enum FilterTypes {
	HPF, // high-pass
	LPF, // low-pass
	BPF, // band-pass
	BSF, // band-stop
	APF  // all-pass
};

class Filter
{
public:
	Filter(float sr);
	~Filter();
	float process(float x);
	void configure(FilterTypes type, float f0, float q = 1.f, float bw = 1.f);

private:
	float sampleRate;
	float a0;
	float a1;
	float a2;
	float b0;
	float b1;
	float b2;
	float xBuffer[FILTER_BUFFER_SIZE];
	float yBuffer[FILTER_BUFFER_SIZE];
};