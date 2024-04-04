#include "Delay.h"

// ========================================================================================

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

