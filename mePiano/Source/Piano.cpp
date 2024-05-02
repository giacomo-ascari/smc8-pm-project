#include "Piano.h"


Piano::Piano(float sampleRate, float samplesPerBlock)
{
	
	this->sampleRate = sampleRate;

	for (int i = 0; i < VOICE_COUNT; i++) voices[i] = new Key(sampleRate);
	for (int i = 0; i < NOTE_COUNT; i++) noteToVoice[i] = -1;

	blockProc = 0;
	time = 0;

	hasClipped = false;
	sinewaveActive = false;
	reverbBalance = 0.5;
	reverbBalanceFilter.configure(0.05f, 0.f, 0.f, 0.95f, 0.f);
	outputGain = 0;
	outputGainFilter.configure(0.05f, 0.f, 0.f, 0.95f, 0.f);

	reverb = new juce::dsp::Convolution();
	spec = juce::dsp::ProcessSpec();
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = 2;
	reverb->reset();
	reverb->loadImpulseResponse(
		BinaryData::ir_wav,
		BinaryData::ir_wavSize,
		juce::dsp::Convolution::Stereo::no,
		juce::dsp::Convolution::Trim::yes,
		(size_t)0,
		juce::dsp::Convolution::Normalise::no
	);
	reverb->prepare(spec);

	startTime = 0;
	stopTime = 0;
	elapsed = 0;
	available = 1;
}

Piano::~Piano()
{
	for (int i = 0; i < VOICE_COUNT; i++)
	{
		delete voices[i];
	}
	delete reverb;
}

void Piano::renderNextBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{	
	startTime = juce::Time::getMillisecondCounter();

	buffer.clear();

	int numSamples = buffer.getNumSamples();
	int numChannels = buffer.getNumChannels();

	if (!blockProc) {
		// first block
		wetBuffer.setSize(numChannels, numSamples);
		available = (float)numSamples / sampleRate * 1000.f;
	}

	if (!midiMessages.isEmpty())
	{
		for (juce::MidiBufferIterator iter = midiMessages.begin(); iter != midiMessages.end(); iter++)
		{
			juce::MidiMessageMetadata metadata = *iter;
			juce::MidiMessage msg = metadata.getMessage();

			int noteNumber = msg.getNoteNumber();

			// if note number is in acceptable range
			if (noteNumber >= LOWER_NOTE && noteNumber <= HIGHER_NOTE)
			{
				// if note on
				if (msg.isNoteOn())
				{
					//DBG("Note " << noteNumber << " on");

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
					//DBG("Note " << noteNumber << " off");

					int voiceToShut = noteToVoice[noteNumber - LOWER_NOTE];
					if (voiceToShut != -1)
					{
						voices[voiceToShut]->dampen();
					}
				}
			}
			
		}
	}

	float y = 0;

	for (int i = 0; i < numSamples; i++)
	{
		y = 0;
		
		if (sinewaveActive)
			y += 0.3f * std::sin(juce::MathConstants<float>::twoPi * 440.f * time / sampleRate);

		for (int i = 0; i < VOICE_COUNT; i++)
		{
			y += voices[i]->process();
		}

		y /= 8;

		if (y > 1.f)
		{
			hasClipped = true;
			y = 1.f;
		}
		else if (y < -1.f)
		{
			hasClipped = true;
			y = -1.f;
		}

		for (int c = 0; c < numChannels; c++)
		{
			buffer.addSample(c, i, y);
		}

		time++;

	}


	float wetGain = reverbBalanceFilter.process(reverbBalance);
	float dryGain = 1 - reverbBalanceFilter.process(reverbBalance);
	float gain = powf(10.f, 0.1 * outputGainFilter.process(outputGain));

	juce::dsp::AudioBlock<float> block { buffer };
	juce::dsp::AudioBlock<float> wetBlock { wetBuffer };
	reverb->process(juce::dsp::ProcessContextNonReplacing<float>(block, wetBlock));

	for (int i = 0; i < numSamples; i++)
	{
		for (int c = 0; c < numChannels; c++)
		{
			y = buffer.getSample(c, i) * dryGain + wetBuffer.getSample(c, i) * wetGain;
			y *= gain;
			buffer.setSample(c, i, y);
		}
	}

	blockProc++;

	stopTime = juce::Time::getMillisecondCounter();
	elapsed = stopTime - startTime;
	load = (float)elapsed / available;
}

Key** Piano::getVoices(int& len)
{
	len = VOICE_COUNT;
	return voices;
}

bool Piano::getHasClipped()
{
	return hasClipped;
}

void Piano::toggleSinewave(bool value)
{
	sinewaveActive = value;
}

void Piano::setReverbBalance(float value)
{
	reverbBalance = value;
}

void Piano::setOutputGain(float value)
{
	outputGain = value;
}

float Piano::getLoad()
{
	return load;
}