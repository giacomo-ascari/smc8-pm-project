#include "Key.h"

Key::Key(float sampleRate)
{
	state = IDLE;
	time = 0;
	activeStrings = 0;
	activeMidiNote = 0;
	activeVelocity = 0;
	dampenWorks = true;
	this->sampleRate = sampleRate;

	for (int i = 0; i < MAX_STRINGS_COUNT; i++)
	{
		strings[i] = new String(sampleRate);
	}

	excitationFilter = new Filter();
	excitationFilter->configure(0.1, 0.9, 0, 0, 0);

	tune(60);
}

Key::~Key()
{
	for (int i = 0; i < MAX_STRINGS_COUNT; i++)
	{
		delete strings[i];
	}
	delete excitationFilter;
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
}

void Key::press(float velocity)
{
	// reset time and start excitation
	time = 0;
	state = ATTACKING;
	activeVelocity = velocity / 127.f;
}

void Key::dampen()
{
	state = RELEASING;
	DBG("dampen invoked for key nn " << activeMidiNote);
}

float Key::process()
{
	static juce::Random rand;

	if (state != IDLE)
	{
		float y = 0.f;
		float excitation = 0.f;

		/*
		att = 0.001 * (1 - velocity);
		rel = 4/string0Pitch(key);
		exc = (0.5 + velocity/2) * en.ar(att, rel, trigger);
		*/

		if (state == ATTACKING)
		{
			float amplitude = (1.f + activeVelocity) * 0.5f; // from 0.5 to 1
			float hitDur = 0.001f * sampleRate;
			float bounceDur = (0.25f / pitches[0]) * sampleRate; // fourth of wave period

			//if (time == 0) {
			//	DBG(amplitude);
			//	DBG(hitDur);
			//}

			if (time < hitDur)
			{
				excitation = sin(juce::MathConstants<float>::twoPi * 1.f / hitDur * sampleRate * time) + rand.nextFloat() * 0.1;
			}
			else if (time < hitDur + bounceDur)
			{
				excitation = 1 - sin(juce::MathConstants<float>::twoPi * 1.f / bounceDur * sampleRate * (time - hitDur));
			}
			else
			{
				excitation = 0;
				state = DECAYING;
			}

			excitation *= amplitude;
		}
		else
		{
			excitation = 0;
		}
		excitation = excitationFilter->process(excitation);

		bool dampen = (state == RELEASING || state == IDLE) && dampenWorks;

		y = String::process(strings, activeStrings, excitation, dampen);

		time++;

		return y;
	}
	else
	{
		time++;

		return 0.f;
	}
	
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