/*
  ==============================================================================

    PitchTables.h
    Created: 28 Mar 2024 12:17:57pm
    Author:  Giacomo

  ==============================================================================
*/

#pragma once

#define LOWER_NOTE 33
#define HIGHER_NOTE 117

#define NOTE_COUNT 85

#define ONE_STRING_NOTES 13
#define TWO_STRING_NOTES 17
#define THREE_STRING_NOTES 55

#define MAX_PITCHES_COUNT 3


const float pitchTable[NOTE_COUNT][MAX_PITCHES_COUNT] =
{
	{1, 1, 1},
	{1, 1, 1},
	{1, 1, 1}
};

class PitchTable {
public:

	static int getStringsCount(int midiNoteNumber)
	{
		int index = midiNoteNumber - LOWER_NOTE;
		if (index < ONE_STRING_NOTES)
		{
			return 1;
		}
		else if (index < ONE_STRING_NOTES + TWO_STRING_NOTES)
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}

	// to re-do when i sort out the pitch tracking

	static void getPitches(int midiNoteNumber, float *p)
	{
		// assumes that the note number is playable by the piano
		// the check is a constraint belonging to the piano

		int index = midiNoteNumber - LOWER_NOTE;

		float pitch = 27.5f * std::powf(2.f, (float)(index + 1) / 12.f);

		int stringsCount = getStringsCount(midiNoteNumber);

		if (stringsCount == 1)
		{
			p[0] = pitch;
			p[1] = 0;
			p[2] = 0;
		}
		else if (stringsCount == 2)
		{
			p[0] = pitch * std::powf(2.f, -0.1f / 12.f);
			p[1] = pitch * std::powf(2.f, 0.1f / 12.f);
			p[2] = 0;
		}
		else if (stringsCount == 3)
		{
			p[0] = pitch;
			p[1] = pitch * std::powf(2.f, 0.2f / 12.f);
			p[2] = pitch * std::powf(2.f, -0.2f / 12.f);
		}
	}
};


