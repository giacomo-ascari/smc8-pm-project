#pragma once

#include "Delay.h"
#include "Filter.h"

class String
{
public:
	String(float sampleRate);
	~String();
	void setSize(float pitch, float hammerPos);
	static float process(String** strings, int stringsCount, float x, bool dampen);

protected:
	float sampleRate;
	Delay frontTopSegment, frontBotSegment, backSegment;
	Filter filterTop, filterBot, dampener;
	float gain;
};