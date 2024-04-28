/*
  ==============================================================================

    Filter.h
    Created: 1 Apr 2024 11:17:13am
    Author:  giaco

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define FILTER_MAX_ORDER 5

#define PI juce::MathConstants<float>::pi

enum FilterTypes {
	HPF, // high-pass
	LPF, // low-pass
	BPF, // band-pass
	BSF, // band-stop
	APF, // all-pass
	BYP  // bypass
};

class Filter
{
public:

	float b[FILTER_MAX_ORDER]; // b0, b1, b2...
	float a[FILTER_MAX_ORDER]; // a0, a1, a2...

	Filter()
	{
		clearCoeff();
        for (int i = 0; i < FILTER_MAX_ORDER - 1; i++)
        {
            xBuffer[i] = 0;
            yBuffer[i] = 0;
        }
	};

	~Filter() {};

	float process(float x)
	{
		float y = b[0] * x;
		for (int i = 0; i < FILTER_MAX_ORDER-1; i++)
		{
			y += b[i+1] * xBuffer[i];
			y -= a[i+1] * yBuffer[i];
		}

		// shift x and y buffer
		for (int i = FILTER_MAX_ORDER-1; i > 0; i--) {
			xBuffer[i] = xBuffer[i - 1];
			yBuffer[i] = yBuffer[i - 1];
		}
		xBuffer[0] = x;
		yBuffer[0] = y;

		return y;
	}

	void configure(float b0, float b1, float b2, float a1, float a2)
	{
		clearCoeff();
		b[0] = b0;
		b[1] = b1;
		b[2] = b2;
		a[1] = a1;
		a[2] = a2;
	}

	void biquad(FilterTypes type, float sampleRate, float f0, float q_bw)
    {
        clearCoeff();
        // the bible and jesus christ himself
        // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

        float w0 = juce::MathConstants<float>::twoPi * f0 / sampleRate;
        float cosw0 = std::cosf(w0);
        float sinw0 = std::sinf(w0);
        float alpha;
        if (type == LPF || type == HPF || type == APF)
            alpha = sinw0 / (2.f * q_bw); // used as Q
        else if (type == BPF || type == BSF)
            alpha = sinw0 * std::sinhf(std::log10f(2.f) / 2.f * q_bw * w0 / sinw0); // used as BW
        else alpha = 0;

        if (type == LPF)
        {
            b[0] = (1.f - cosw0) * 0.5f;
            b[1] = 1.f - cosw0;
            b[2] = (1.f - cosw0) * 0.5f;
            a[0] = 1.f + alpha;
            a[1] = -2.f * cosw0;
            a[2] = 1.f - alpha;
        }
        else if (type == HPF)
        {
            b[0] = (1.f + cosw0) * 0.5f;
            b[1] = -1.f - cosw0;
            b[2] = (1.f + cosw0) * 0.5f;
            a[0] = 1.f + alpha;
            a[1] = -2.f * cosw0;
            a[2] = 1.f - alpha;
        }
        else if (type == BPF)
        {
            b[0] = alpha;
            b[1] = 0.f;
            b[2] = -alpha;
            a[0] = 1.f + alpha;
            a[1] = -2.f * cosw0;
            a[2] = 1.f - alpha;
        }
        else if (type == BSF)
        {
            b[0] = 1.f;
            b[1] = -2.f * cosw0;
            b[2] = 1.f;
            a[0] = 1.f + alpha;
            a[1] = -2.f * cosw0;
            a[2] = 1.f - alpha;
        }
        else if (type == APF)
        {
            b[0] = 1.f - alpha;
            b[1] = -2.f * cosw0;
            b[2] = 1.f + alpha;
            a[0] = 1.f + alpha;
            a[1] = -2.f * cosw0;
            a[2] = 1.f - alpha;
        }
        else if (type == BYP)
        {
            b[0] = 1;
            b[1] = 0;
            b[2] = 0;
            a[0] = 1;
            a[1] = 0;
            a[2] = 0;
        }

        // normalize to a0 = 1
        float gain = (1 / a[0]);
        b[0] *= gain; b[1] *= gain; b[2] *= gain;
        a[0] *= gain; a[1] *= gain; a[2] *= gain;
    };

private:
	float xBuffer[FILTER_MAX_ORDER]; // x1, x2
	float yBuffer[FILTER_MAX_ORDER]; // y1, y2

	void clearCoeff() {
		for (int i = 1; i < FILTER_MAX_ORDER; i++)
		{
			b[i] = 0.f;
			a[i] = 0.f;
			xBuffer[i] = 0.f;
			yBuffer[i] = 0.f;
		}
		b[0] = 1.f;
		a[0] = 1.f;
	}
};