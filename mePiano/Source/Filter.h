/*
  ==============================================================================

    Filter.h
    Created: 1 Apr 2024 11:17:13am
    Author:  giaco

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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
	Filter();
	~Filter();
	float process(float x);
	void configure(FilterTypes type, float sampleRate, float f0, float q_bw);
	bool isStable();

private:
	float sampleRate;
	float b0, b1, b2, a0, a1, a2;
	float xBuffer[FILTER_BUFFER_SIZE];
	float yBuffer[FILTER_BUFFER_SIZE];
};