#pragma once

#include <JuceHeader.h>

// 96kHz, 20Hz wave period
#define DELAY_MAX_L 

class Delay {
public:
	Delay();
	~Delay();
	void setLength(float l);
	void pushSample(float x);
	float getSample(int i);
	void elapseTime();
private:
	float memory;
	int cursor;
};
