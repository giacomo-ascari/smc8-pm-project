/*
  ==============================================================================

    Exciter.h
    Created: 24 Apr 2024 7:32:18pm
    Author:  Giacomo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Filter.h"

#define TWOPI juce::MathConstants<float>::twoPi

enum ExciterTypes {
	SQUARE,
	TRIANGLE,
	HAMMER,
};

class Exciter
{
public:
	Exciter(float sampleRate)
	{
		this->type = SQUARE;
		this->filter.configure(1, 0, 0, 0, 0);
		this->attackLen = 1;
		this->decayLen = 0;
		this->amplitude = 1.f;
		this->sampleRate = sampleRate;
	};

	~Exciter() {};

	void setSquare(float attackDur, float amplitude)
	{
		this->type = SQUARE;
		this->filter.configure(1, 0, 0, 0, 0);
		this->attackLen = attackDur * this->sampleRate;
		this->decayLen = 0;
		this->amplitude = amplitude;
	};

	void setTriangle(float attackDur, float decayDur, float amplitude)
	{
		this->type = TRIANGLE;
		this->filter.configure(1, 0, 0, 0, 0);
		this->attackLen = attackDur * this->sampleRate;
		this->decayLen = decayDur * this->sampleRate;
		this->amplitude = amplitude;
	};

	void setHammer(float pitch, float velocity)
	{
		DBG(sampleRate);
		this->type = HAMMER;
		//this->filter.configure(0.5, 0, 0, 0.5, 0);
		this->filter.configure(1, 0, 0, 0, 0);
		this->attackLen = 0.001f * sampleRate;
		this->decayLen = (0.5f / pitch) * sampleRate;
		this->amplitude = (0.5f + velocity / 2.f);
		//this->attackLen = 0.001f * (1.f - velocity) * sampleRate;
		//this->decayLen = (4.f / pitch) * sampleRate; // fourth of wave period
		//this->amplitude = (0.5f + velocity / 2); // from 0.5 to 1
	}

	float process(uint32_t time)
	{
		float y = 0;

		if (type == SQUARE)
		{
			if (time < attackLen) y = 1;
			else y = 0;
		}
		else if (type == TRIANGLE)
		{
			if (time < attackLen) y = time / attackLen;
			else if (time < attackLen + decayLen) y = 1 - (time - attackLen) / decayLen;
			else y = 0;
		}
		else if (type == HAMMER)
		{
			//if (time < attackLen) y = sin(TWOPI / attackLen * sampleRate * time);
			//else if (time < attackLen + decayLen) y = 1 - sin(TWOPI / decayLen * sampleRate * (time - attackLen));
			//else y = 0;
			if (time < attackLen) y = time / attackLen;
			else if (time < attackLen + decayLen) y = 1 - (time - attackLen) / decayLen;
			else y = 0;
		}

		y = this->filter.process(y);
		y *= this->amplitude;

		return y;
	}

	bool isAttacking(uint32_t time)
	{
		return (time < attackLen);
	}

	bool isDecaying(uint32_t time)
	{
		return (time < attackLen + decayLen);
	}

private:

	ExciterTypes type;
	Filter filter;

	uint32_t attackLen;
	uint32_t decayLen;

	float amplitude;
	float sampleRate;
};
