#pragma once

#include <JuceHeader.h>
#include "Key.h"

#define VOICE_COUNT 20
#define NOTE_COUNT 85

class Piano
{
public:
	Piano(float sampleRate, float samplesPerBlock);
	~Piano();
	void renderNextBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
	Key** getVoices(int &len);
	
private:
	float sampleRate;
	int noteToVoice[NOTE_COUNT];
	Key* voices[VOICE_COUNT]; // equivalent to voices
	juce::dsp::Convolution* reverb;
	juce::dsp::ProcessSpec spec;
};

