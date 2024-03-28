#pragma once

#include <JuceHeader.h>
#include "Key.h"
#include "DSP.h"

class Piano
{
public:
	Piano(float sr);
	~Piano();
	void renderNextBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
private:
	float sampleRate;
	Key* testKey;
};

