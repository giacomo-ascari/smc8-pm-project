#include "Key.h"

Key::Key(float sampleRate) : exciter(sampleRate)
{
	state = IDLE;
	time = 0;
	activeStrings = 0;
	activeMidiNote = 0;
	activeVelocity = 0;
	dampenWorks = true;
	dampenRandomizable = true;
	lastValue = 0;
	this->sampleRate = sampleRate;

	for (int i = 0; i < MAX_STRINGS_COUNT; i++)
	{
		strings[i] = new String(sampleRate);
	}

	tune(60);
}

Key::~Key()
{
	for (int i = 0; i < MAX_STRINGS_COUNT; i++)
	{
		delete strings[i];
	}
}

void Key::tune(int midiNoteNumber)
{

	// get data and update private class members
	activeMidiNote = midiNoteNumber;
	//PitchTable::getPitches(activeMidiNote, pitches);
	PitchTable::getMeasuredPitches(activeMidiNote, pitches, dampenWorks);
	activeStrings = PitchTable::getStringsCount(midiNoteNumber);

	// tune
	for (int i = 0; i < activeStrings; i++)
	{
		strings[i]->setSize(pitches[i], 0.1721);
	}

	// for the unreliability feature
	dampenRandomizable = dampenWorks;
}

void Key::press(float velocity)
{
	// reset time and start excitation
	time = 0; // used for exciter
	state = ATTACKING;
	activeVelocity = velocity / 127.f;

	// dampener unreliability 10%
	if (dampenRandomizable) {
		if (reliabilityRand.nextFloat() < 0.1f) dampenWorks = false;
		else dampenWorks = true;
	}

	exciter.setHammer(pitches[0], activeVelocity);
	//exciter.setSquare(0.001, activeVelocity);
}

void Key::dampen()
{
	state = RELEASING;
}

float Key::process()
{
	float y = 0.f;
	float excitation = 0.f;

	if (state != IDLE)
	{
		excitation = exciter.process(time);

		if (exciter.isAttacking(time)) state = ATTACKING;
		else if (exciter.isDecaying(time)) state = DECAYING;
		else state = RELEASING;

		bool dampen = (state == RELEASING) && dampenWorks;

		y = String::process(strings, activeStrings, excitation, dampen);
	}

	time++;
	lastValue = y;
	return y;
}

int Key::getMidiNote()
{
	return activeMidiNote;
}

KeyState Key::getState()
{
	return state;
}

uint32_t Key::getTime()
{
	return time;
}

int Key::getStrings()
{
	return activeStrings;
}

float Key::getLastValue()
{
	return lastValue;
}