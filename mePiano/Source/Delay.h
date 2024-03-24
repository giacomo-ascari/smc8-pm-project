#pragma once

#include <JuceHeader.h>

// 96kHz, 20Hz wave period = 4800
#define DELAY_LEN 5000

class Delay {
public:
	Delay();
	~Delay();
	void pushSample(float x);
	float getSample(int i);
	float getSampleF(float i);

private:
	float memory[DELAY_LEN];
	int cursor;
};

