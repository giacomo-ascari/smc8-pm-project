#pragma once

#include <JuceHeader.h>
#include "DSP.h"
#include "PitchTables.h"

enum KeyState
{
	IDLE,
	ATTACKING,
	DECAYING,
	RELEASING
};

class Key 
{
public:
	Key(float sr);
	~Key();
	void press(int midiNoteNumber, float v);
	float process();

private:
	KeyState state;
	uint32_t time;
	float pitch;
	float sampleRate;
	// damn man, find a better way please
	// string 1
	Delay s1FrontTopSegment;
	Delay s1FrontBotSegment;
	Delay s1BackSegment;
	// string 2
	Delay s2FrontTopSegment;
	Delay s2FrontBotSegment;
	Delay s2BackSegment;
	// string 3
	Delay s3FrontTopSegment;
	Delay s3FrontBotSegment;
	Delay s3BackSegment;
};

class String
{
public:
	String();
	~String();
	void setLength(float len);
	void set
	float process();

private:
	Delay frontTopSegment;
	Delay frontBotSegment;
	Delay backSegment;

};