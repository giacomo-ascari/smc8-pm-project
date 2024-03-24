#include "Piano.h"


Piano::Piano() {
	sampleRate = 48000.f;
}

Piano::~Piano() {

}

void Piano::renderNextBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{	
	buffer.clear();

	int numSamples = buffer.getNumSamples();
	int numChannels = buffer.getNumChannels();
	

	if (!midiMessages.isEmpty())
	{
		for (juce::MidiBufferIterator iter = midiMessages.begin(); iter != midiMessages.end(); iter++)
		{
			juce::MidiMessageMetadata metadata = *iter;
			std::string s = ">>>" + std::to_string(metadata.numBytes);
			juce::MidiMessage msg = metadata.getMessage();
			DBG(msg.getDescription());

		}
		// for (const MidiMessageMetadata metadata : midiBuffer)
		// if (metadata.numBytes == 3)
		// Logger::writeToLog(metadata.getMessage().getDescription());
	}


	static uint32_t time = 0;
	static uint32_t blockProc = 0;

	if (!time) {
		DBG("FIRST PROC");
		DBG("sr" + std::to_string(sampleRate));
		DBG("nc" + std::to_string(numChannels));
		DBG("ns" + std::to_string(numSamples));
	}
	for (int i = 0; i < numSamples; i++)
	{

		float x = (float)time / sampleRate;

		float val = 0.1 * std::sin(juce::MathConstants<float>::twoPi * 440.f * x);


		for (int c = 0; c < numChannels; c++)
		{
			buffer.addSample(c, i, val);
		}

		time++;
	}
	blockProc++;
}
