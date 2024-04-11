/*
  ==============================================================================

    String.cpp
    Created: 1 Apr 2024 10:56:36am
    Author:  giaco

  ==============================================================================
*/

#include "String.h"

String::String(float sr)
{
	sampleRate = sr;
	filterTop.configure(LPF, sampleRate, 15000.f, 0.8f);
	filterBot.configure(APF, sampleRate, 15000.f, 0.7f);
	dampener.configure(LPF, sampleRate, 100.f, 0.7071f);
	gain = -0.995f; // -0.995
	setSize(440.f, 0.2f);
}

String::~String() { }

void String::setSize(float pitch, float hammerPos)
{
	float length = sampleRate / pitch / 2.f;
	// hammer pos between 0 and 1
	frontTopSegment.setLength(length * hammerPos);
	frontBotSegment.setLength(length * (1 - hammerPos));
	backSegment.setLength(length);
}

float String::process(String** strings, int stringsCount, float x, bool dampen)
{
	float input = 0, output = 0;
	static float middle[10]; // max 10 string dum dum

	if (stringsCount == 0) return 0;

	// Calc the first part of the model and the output
	// Needed to not have delay on sympathetic influence
	for (int i = 0; i < stringsCount; i++)
	{
		String* string = strings[i];

		// Sum x and the output of third delay and feed to first delay
		// if dampening, also pass it through the additional lpf
		input = x + string->frontBotSegment.getSample();
		if (dampen)
		{
			string->frontTopSegment.pushSample(string->dampener.process(input));
			//string->frontTopSegment.pushSample(input);
		}
		else
		{
			string->frontTopSegment.pushSample(input);
			string->dampener.process(0);
		}

		// Get output of first delay and process it through the termination
		// Get the ouput of the termination
		middle[i] = string->filterTop.process(string->gain * string->frontTopSegment.getSample());
		output += middle[i];
	}

	// Rescale the output thing
	output /= stringsCount;

	// Simulate the second part of the model
	// and feed the simpathetic influence
	for (int i = 0; i < stringsCount; i++)
	{
		String* string = strings[i];
		
		// If more than one string
		// Sum sym to result of the top termination
		// read the simpathetic of the other string and rescaleit
		if (stringsCount == 1)
		{
			string->backSegment.pushSample(middle[i]);
		}
		else
		{
			float sympathetic = 0;
			for (int j = 0; j < stringsCount; j++)
			{
				if (j != i) sympathetic += middle[i];
			}
			sympathetic /= (stringsCount - 1);
			string->backSegment.pushSample(0.7f * middle[i] + 0.3f * sympathetic);
		}

		
		

		// Get output of the bottom termination and feed to the third delay
		string->frontBotSegment.pushSample(string->filterBot.process(string->gain * string->backSegment.getSample()));
		//frontBotSegment.pushSample(gain * backSegment.getSample());
	}

	return output;
}