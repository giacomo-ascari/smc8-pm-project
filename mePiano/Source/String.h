#pragma once

#include "Delay.h"
#include "Filter.h"

class String
{
protected:
	float sampleRate;
	Delay frontTopSegment, frontBotSegment, backSegment;
	Filter filterTop, filterBot;
	float gain;

public:
	String(float sampleRate)
	{
		this->sampleRate = sampleRate;
		//filterTop.biquad(LPF, sampleRate, 15000.f, 0.7071f);
		//filterBot.biquad(LPF, sampleRate, 12000.f, 0.7071f);
		gain = -0.990f; // -0.995
		setSize(440.f, 0.2f);
	};

	~String() { };

	void setSize(float pitch, float hammerPos)
	{
		float length = sampleRate / pitch / 2.f;
		// hammer pos between 0 and 1
		frontTopSegment.setLength(length * (1 - hammerPos));
		frontBotSegment.setLength(length * hammerPos);
		backSegment.setLength(length);
	};

	static float process(String** strings, int stringsCount, float x, bool dampen)
	{
		float input = 0, output = 0;
		static float middle[3]; // max 3 string dum dum

		if (stringsCount == 0 || stringsCount > 3) return 0;

		// Calc the first part of the model and the output
		// Needed to not have delay on sympathetic influence
		for (int i = 0; i < stringsCount; i++)
		{
			String* string = strings[i];

			// Sum x and the output of third delay and feed to first delay
			// if dampening, also pass it through the additional filter
			input = x + string->frontBotSegment.getSample();
			if (dampen)
			{
				//string->frontTopSegment.pushSample(string->dampener.process(input));
				string->frontTopSegment.pushSample(input * 0.94f);
			}
			else
			{
				string->frontTopSegment.pushSample(input);
			}

			// Get output of first delay and process it through the termination
			// Get the ouput of the termination
			//middle[i] = string->filterTop.process(string->gain * string->frontTopSegment.getSample());
			middle[i] = string->gain * string->frontTopSegment.getSample();
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
				string->backSegment.pushSample(0.5f * middle[i] + 0.5f * sympathetic);
			}

			// Get output of the bottom termination and feed to the third delay
			//string->frontBotSegment.pushSample(string->filterBot.process(string->gain * string->backSegment.getSample()));
			string->frontBotSegment.pushSample(string->gain * string->backSegment.getSample());
		}

		return output;
	};

};