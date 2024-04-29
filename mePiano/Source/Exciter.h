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
		this->time = 0;
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
		this->time = 0;
		this->type = SQUARE;
		this->filter.configure(1, 0, 0, 0, 0);
		this->attackLen = attackDur * this->sampleRate;
		this->decayLen = 0;
		this->amplitude = amplitude;
	};

	void setTriangle(float attackDur, float decayDur, float amplitude)
	{
		this->time = 0;
		this->type = TRIANGLE;
		this->filter.configure(1, 0, 0, 0, 0);
		this->attackLen = attackDur * this->sampleRate;
		this->decayLen = decayDur * this->sampleRate;
		this->amplitude = amplitude;
	};

	void setHammer(float pitch, float velocity)
	{
		DBG(sampleRate);
		this->time = 0;
		this->type = HAMMER;
		this->filter.configure(0.34, 0.33, 0.33, 0, 0);
		//this->attackLen = 0.001f * sampleRate;
		//this->decayLen = (1.f / 8.f / pitch) * sampleRate; // fourth of wave period
		//this->amplitude = (0.5f + velocity) * 0.67f; // from 0.33 to 1
		this->attackLen = 0.001f * (1.f - velocity) * sampleRate;
		this->decayLen = (4.f / pitch) * sampleRate; // fourth of wave period
		this->amplitude = (0.5f + velocity / 2); // from 0.5 to 1
	}

	float process()
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

		time++;
		
		y = this->filter.process(y);
		y *= this->amplitude;

		return y;
	}

	bool isAttacking()
	{
		return (time < attackLen);
	}

	bool isDecaying()
	{
		return (time < attackLen + decayLen);
	}

private:

	juce::uint32 time;

	ExciterTypes type;
	Filter filter;

	juce::uint32 attackLen;
	juce::uint32 decayLen;

	float amplitude;
	float sampleRate;
};
