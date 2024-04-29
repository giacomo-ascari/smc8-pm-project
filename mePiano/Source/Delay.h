#pragma once

#include <JuceHeader.h>

// 96kHz, 20Hz wave period = 4800
#define DELAY_DEF_SIZE 5000

class SimpleDelay
{
private:
	int cursor;
	int length; // integer part of the delay length 
	float memory[DELAY_DEF_SIZE];
public:
	SimpleDelay()
	{
		cursor = DELAY_DEF_SIZE - 1;
		length = 1; // default
		for (int i = 0; i < DELAY_DEF_SIZE; i++)
		{
			memory[i] = 0.f;
		}
	};

	~SimpleDelay() { };

	void setLength(float l)
	{
		if (l < 0 || l >= DELAY_DEF_SIZE) return;
		length = (int)round(l);
	};

	void pushSample(float x)
	{
		cursor++;
		if (cursor >= DELAY_DEF_SIZE)
		{
			cursor = 0;
		}
		memory[cursor] = x;
	};

	float getSample()
	{
		int index = cursor - length;
		// shift back into valid range
		if (index < 0) index += DELAY_DEF_SIZE;
		return memory[index];
	};
};

class Delay
{
private:
	int cursor;
	int lengthInt; // integer part of the delay length 
	float lengthDec; // decimal part of the delay length
	bool isFractional; // indicates if the delay is fractional
	float memory[DELAY_DEF_SIZE];

public:

	Delay()
	{
		cursor = DELAY_DEF_SIZE - 1;
		lengthInt = 1; // default
		lengthDec = 0.f; // default
		isFractional = false;

		for (int i = 0; i < DELAY_DEF_SIZE; i++)
		{
			memory[i] = 0.f;
		}
	};

	~Delay() { };

	void setLength(float l)
	{
		if (l < 0 || l >= DELAY_DEF_SIZE) return;

		float floored = std::floorf(l);
		float decimal = l - floored;
		float epsilon = 0.01f;
		if (decimal < epsilon)
		{
			lengthInt = floored;
			lengthDec = 0.f;
			isFractional = false;
		}
		else {
			lengthInt = floored;
			lengthDec = decimal;
			isFractional = true;
		}
	};

	void pushSample(float x)
	{
		cursor++;
		if (cursor >= DELAY_DEF_SIZE)
		{
			cursor = 0;
		}
		memory[cursor] = x;
	};

	float getSample()
	{
		if (isFractional)
		{
			int floorIndex = cursor - lengthInt;
			int ceilIndex = cursor - lengthInt - 1;
			// shift back into valid range
			if (floorIndex < 0) floorIndex += DELAY_DEF_SIZE;
			if (ceilIndex < 0) ceilIndex += DELAY_DEF_SIZE;
			return memory[floorIndex] * (1.f - lengthDec) + memory[ceilIndex] * (lengthDec);
		}
		else
		{
			int index = cursor - lengthInt;
			// shift back into valid range
			if (index < 0) index += DELAY_DEF_SIZE;
			return memory[index];
		}
	};

	float process(float x)
	{
		pushSample(x);
		return getSample();
	}

};


