#pragma once

#include "Delay.h"
#include "Filter.h"

class String
{
protected:
	float sampleRate;
	Delay frontTopSegment, frontBotSegment, backTopSegment, backBotSegment;
	Filter filterTop, filterBot;
	float gain;

public:
	String(float sampleRate)
	{
		this->sampleRate = sampleRate;
		filterTop.biquad(LPF, sampleRate, 14000.f, 0.701f);
		filterBot.biquad(LPF, sampleRate, 12000.f, 0.701f);
		//filterTop.configure(0.5, 0, 0, 0.5, 0);
		//filterBot.configure(0.5, 0, 0, 0.5, 0);
		gain = -0.995f; // -0.995
		setSize(440.f, 0.2f);
	};

	~String() { };

	void setSize(float pitch, float hammerPos)
	{
		float length = sampleRate / pitch / 2.f;
		// hammer pos between 0 and 1
		frontTopSegment.setLength(length * (1 - hammerPos));
		frontBotSegment.setLength(length * hammerPos);
		backTopSegment.setLength(length * (1 - hammerPos));
		backBotSegment.setLength(length * hammerPos);
	};

	static float process(String** strings, int stringsCount, float x, bool dampen)
	{
		float y = 0;

		float temp = 0;
		static float middle[3]; // max 3 string dum dum
		float sympathetic = 0;

		if (stringsCount == 0 || stringsCount > 3) return 0;

		// Calc the first part of the model and the output
		// Needed to not have delay on sympathetic influence
		for (int i = 0; i < stringsCount; i++)
		{
			String* string = strings[i];

			// Sum x and the output of third delay and feed to first delay
			// if dampening, also pass it through the additional filter
			temp = x + string->frontBotSegment.getSample();
			if (dampen) string->frontTopSegment.pushSample(temp * 0.95f);
			else string->frontTopSegment.pushSample(temp);

			// Get output of first delay and process it through the termination
			// Get the ouput of the termination
			middle[i] = string->gain * string->filterTop.process(string->frontTopSegment.getSample());
		}

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
				string->backTopSegment.pushSample(middle[i]);
			}
			else if (stringsCount == 2)
			{
				sympathetic = middle[(i + 1) % 2];
				string->backTopSegment.pushSample(0.5f * middle[i] + 0.5f * sympathetic);
			}
			else
			{
				sympathetic = middle[(i + 1) % 3] + middle[(i + 2) % 3];
				sympathetic *= 0.5;
				string->backTopSegment.pushSample(0.33f * middle[i] + 0.67f * sympathetic);
			}

			// Get output of bottom top termination and use it as y output
			// then feed it to fourth delay
			temp = string->backTopSegment.getSample();
			string->backBotSegment.pushSample(temp);
			y += temp;

			// Get output of the bottom termination and feed to the third delay
			string->frontBotSegment.pushSample(string->gain * string->filterBot.process(string->backBotSegment.getSample()));
		}

		// Rescale the output thing
		y /= stringsCount;

		return y;
	};

};