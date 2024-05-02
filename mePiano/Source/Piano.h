#pragma once

#include <JuceHeader.h>
#include "Key.h"
#include "Filter.h"

#define VOICE_COUNT 20
#define NOTE_COUNT 85

class Piano
{
public:
	Piano(float sampleRate, float samplesPerBlock);
	~Piano();
	void renderNextBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
	// user interface
	Key** getVoices(int& len);
	bool getHasClipped();
	void toggleSinewave(bool value);
	void setReverbBalance(float value);
	void setOutputGain(float value);
	// performance
	float getLoad();
	
private:
	float sampleRate;
	int noteToVoice[NOTE_COUNT];
	uint32_t blockProc;
	Key* voices[VOICE_COUNT];
	juce::dsp::Convolution* reverb;
	juce::dsp::ProcessSpec spec;
	juce::AudioBuffer<float> wetBuffer;
	uint32_t time;
	// user interface
	bool hasClipped;
	bool sinewaveActive;
	float reverbBalance;
	Filter reverbBalanceFilter;
	float outputGain;
	Filter outputGainFilter;

	// performance
	uint32_t startTime, stopTime, elapsed, available;
	float load;

};

