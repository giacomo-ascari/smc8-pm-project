#include "Piano.h"


Piano::Piano(float sr) {
	
	sampleRate = sr;

	for (int i = 0; i < VOICE_COUNT; i++)
	{
		voices[i] = new Key(sampleRate);
	}
	for (int i = 0; i < NOTE_COUNT; i++)
	{
		noteToVoice[i] = -1;
	}
}

Piano::~Piano()
{
	for (int i = 0; i < VOICE_COUNT; i++)
	{
		delete voices[i];
	}
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

			int noteNumber = msg.getNoteNumber();

			// if note number is in acceptable range
			if (noteNumber >= LOWER_NOTE && noteNumber <= HIGHER_NOTE)
			{
				// if note on
				if (msg.isNoteOn())
				{
					// if no voice is set to the note
					if (noteToVoice[noteNumber - LOWER_NOTE] == -1)
					{
						// find a voice with the oldest note and replace it
						int result = 0;
						for (int i = 1; i < VOICE_COUNT; i++)
						{
							if (voices[i]->getTime() > voices[result]->getTime()) result = i;
						}

						
						// update the old note pointing to the voice
						for (int i = 1; i < NOTE_COUNT; i++)
						{
							if (noteToVoice[i] == result) noteToVoice[i] = -1;
						}
						// update the noteToVoice structure for the new note
						noteToVoice[noteNumber - LOWER_NOTE] = result;

						voices[result]->tune(noteNumber);
						voices[result]->press(msg.getVelocity());
					}
					else
					{
						// there is a voice already set to the note
						voices[noteToVoice[noteNumber - LOWER_NOTE]]->press(msg.getVelocity());
					}
				}
				else if (msg.isNoteOff())
				{
					int voiceToShut = noteToVoice[noteNumber - LOWER_NOTE];
					if (voiceToShut != -1)
					{
						voices[voiceToShut]->dampen();
					}
				}
			}
			
		}
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

		float y = 0;
		//y = 0.1 * std::sin(juce::MathConstants<float>::twoPi * 440.f * x);

		for (int i = 0; i < VOICE_COUNT; i++)
		{
			y += voices[i]->process();
		}

		y /= 5;

		if (y >= 1.f) y = 1.f;
		else if (y < +-1.f) y = -1.f;

		for (int c = 0; c < numChannels; c++)
		{
			buffer.addSample(c, i, y);
		}

		time++;
	}
	blockProc++;
}

Key** Piano::getVoices(int& len)
{
	len = VOICE_COUNT;
	return voices;
}