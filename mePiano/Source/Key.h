#pragma once

#include <JuceHeader.h>
#include "PitchTables.h"
#include "String.h"

#define MAX_STRINGS_COUNT 3

enum KeyState
{
	IDLE,
	ATTACKING,
	DECAYING,
	RELEASING
};

// sustain doesn't exist ofc

class Key 
{
public:
	Key(float sampleRate);
	~Key();
	void tune(int midiNoteNumber);
	void press(float velocity);
	void dampen();
	float process();
	int getMidiNote();
	KeyState getState();
	uint32_t getTime();
	int getStrings();

private:
	KeyState state;
	uint32_t time;
	int activeStrings;
	int activeMidiNote;
	float activeVelocity;
	float sampleRate;
	String* strings[MAX_STRINGS_COUNT];
	float pitches[MAX_STRINGS_COUNT];
};