#include "DSP.h"

// ========================================================================================
// Delay                                                                                

Delay::Delay()
{
    size = DELAY_DEF_SIZE;
    cursor = size-1;
    lengthInt = 1; // default
    lengthDec = 0.f; // default
    isFractional = false;

    memory = new float[size];
    for (int i = 0; i < size; i++)
    {
        memory[i] = 0.f;
    }
}

Delay::~Delay()
{
    delete memory;
}

void Delay::pushSample(float x)
{
    cursor++;
    if (cursor >= size)
    {
        cursor = 0;
    }
    memory[cursor] = x;
}


void Delay::setLength(float l)
{
    if (l < 0 || l >= size) return;

    float floored = std::floorf(l);
    float decimal = l - floored;
    float epsilon = 0.01f;
    if (decimal < epsilon)
    {
        lengthInt = floored;
        lengthDec = 0.f;
        isFractional = false;
    }
    else {
        lengthInt = floored;
        lengthDec = decimal;
        isFractional = true;
    }
}

void Delay::resizeMemory(int s)
{
    if (s <= 0) return;
    delete memory;
    size = s;
    memory = new float[size];
    for (int i = 0; i < size; i++)
    {
        memory[i] = 0.f;
    }
}

float Delay::getSample() {

    if (isFractional)
    {
        int floorIndex = cursor - lengthInt;
        int ceilIndex = cursor - lengthInt - 1;
        // shift back into valid range
        if (floorIndex < 0) floorIndex += size;
        if (ceilIndex < 0) ceilIndex += size;
        return memory[floorIndex] * (1.f - lengthDec) + memory[ceilIndex] * (lengthDec);   
    }
    else
    {
        int index = cursor - lengthInt;
        // shift back into valid range
        if (index < 0) index += size;
        return memory[index];
    }

}

float Delay::process(float x)
{
    pushSample(x);
    return getSample();
}

// ========================================================================================
// Bi-quad filter



Filter::Filter(float sr)
{
    sampleRate = sr;
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
        (b0 / a0) * x +
        (b1 / a0) * xBuffer[0] +
        (b2 / a0) * xBuffer[1] -
        (a1 / a0) * yBuffer[0] -
        (a2 / a0) * yBuffer[1];
    
    // shift x and y buffer
    xBuffer[1] = xBuffer[0];
    xBuffer[0] = x;
    yBuffer[1] = yBuffer[0];
    yBuffer[0] = y;

    return y;
}

void Filter::configure(FilterTypes type, float f0, float q, float bw)
{
    // the bible and jesus christ himself
    // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
    float w0 = juce::MathConstants<float>::twoPi * f0 / sampleRate;
    float cosw0 = std::cosf(w0);
    float sinw0 = std::sinf(w0);
    float alpha;
    if (type == LPF || type == HPF || type == APF)
        alpha = sinw0 / (2 * q);
    else if (type == BPF || type == BSF)
        alpha = sinw0 * std::sinhf(std::log10f(2) / 2 * bw * w0 / sinw0);
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
        a2 + 1.f - alpha;
    }
    else if (type == BSF)
    {
        b0 = 1.f;
        b1 = -2.f * cosw0;
        b2 = 1.f;
        a0 = 1.f + alpha;
        a1 = -2.f * cosw0;
        a2 + 1.f - alpha;
    }
    else if (type == APF)
    {
        b0 = 1.f - alpha;
        b1 = -2.f * cosw0;
        b2 = 1.f + alpha;
        a0 = 1.f + alpha;
        a1 = -2.f * cosw0;
        a2 + 1.f - alpha;
    }
}
