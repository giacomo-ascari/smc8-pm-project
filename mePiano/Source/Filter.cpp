/*
  ==============================================================================

    Filter.cpp
    Created: 1 Apr 2024 11:17:13am
    Author:  giaco

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter()
{
    a0 = 1.f;
    a1 = 1.f;
    a2 = 1.f;
    b0 = 1.f;
    b1 = 1.f;
    b2 = 1.f;
    for (int i = 0; i < FILTER_BUFFER_SIZE; i++)
    {
        xBuffer[i] = 0.f;
        yBuffer[i] = 0.f;
    }
}

Filter::~Filter() { }

float Filter::process(float x)
{
    float y =
        b0 * x + b1 * xBuffer[0] + b2 * xBuffer[1] +
        a1 * yBuffer[0] +  a2 * yBuffer[1];
    
    // shift x and y buffer
    xBuffer[1] = xBuffer[0];
    xBuffer[0] = x;
    yBuffer[1] = yBuffer[0];
    yBuffer[0] = y;

    return y;
}

void Filter::configure(FilterTypes type, float sampleRate, float f0, float q_bw)
{
    // the bible and jesus christ himself
    // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
    float w0 = juce::MathConstants<float>::twoPi * f0 / sampleRate;
    float cosw0 = std::cosf(w0);
    float sinw0 = std::sinf(w0);
    float alpha;
    if (type == LPF || type == HPF || type == APF)
        alpha = sinw0 / (2 * q_bw); // used as Q
    else if (type == BPF || type == BSF)
        alpha = sinw0 * std::sinhf(std::log10f(2) / 2 * q_bw * w0 / sinw0); // used as BW
    else alpha = 0;
    if (type == LPF)
    {
        b0 = (1.f - cosw0) * 0.5f;
        b1 = 1.f - cosw0;
        b2 = (1.f - cosw0) * 0.5f;
        a0 = 1.f + alpha;
        a1 = -2.f * cosw0;
        a2 = 1.f - alpha;
    }
    else if (type == HPF)
    {
        b0 = (1.f + cosw0) * 0.5f;
        b1 = -1.f - cosw0;
        b2 = (1.f + cosw0) * 0.5f;
        a0 = 1.f + alpha;
        a1 = -2.f * cosw0;
        a2 = 1.f - alpha;
    }
    else if (type == BPF)
    {
        b0 = alpha;
        b1 = 0.f;
        b2 = -alpha;
        a0 = 1.f + alpha;
        a1 = -2.f * cosw0;
        a2 = 1.f - alpha;
    }
    else if (type == BSF)
    {
        b0 = 1.f;
        b1 = -2.f * cosw0;
        b2 = 1.f;
        a0 = 1.f + alpha;
        a1 = -2.f * cosw0;
        a2 = 1.f - alpha;
    }
    else if (type == APF)
    {
        b0 = 1.f - alpha;
        b1 = -2.f * cosw0;
        b2 = 1.f + alpha;
        a0 = 1.f + alpha;
        a1 = -2.f * cosw0;
        a2 = 1.f - alpha;
    }

    // normalize to a0 = 1
    float gain = (1 / a0);
    b0 *= gain;
    b1 *= gain;
    b2 *= gain;
    a0 *= gain;
    a1 *= gain;
    a2 *= gain;

}

bool Filter::isStable()
{
    return fabsf(b0) < 1 && fabsf(b1) < 1 && fabsf(b2) < 1 && fabsf(a0) < 1 && fabsf(a1) < 1 && fabsf(a2) < 1;
}