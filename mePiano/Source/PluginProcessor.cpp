/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MePianoAudioProcessor::MePianoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
   // boh
}

MePianoAudioProcessor::~MePianoAudioProcessor()
{
    delete piano;
}

//==============================================================================
const juce::String MePianoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MePianoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MePianoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MePianoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MePianoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MePianoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MePianoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MePianoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MePianoAudioProcessor::getProgramName (int index)
{
    return {};
}

void MePianoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MePianoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(samplesPerBlock);
    lastSampleRate = sampleRate;
    piano = new Piano(sampleRate, samplesPerBlock);

}

void MePianoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MePianoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MePianoAudioProcessor::addArtificialMidi(float velocity, int noteNumber, int duration)
{
    juce::uint8 v = velocity;
    if (v > 127) v = 127;

    juce::uint8 n = noteNumber;
    if (n > 127) n = 127;

    juce::MidiMessage onMessage = juce::MidiMessage::noteOn(1, n, v);
    artificialMidi.addEvent(onMessage, 0);

    static juce::MidiMessage offMessage = juce::MidiMessage::noteOff(1, n);

    juce::Timer::callAfterDelay(duration, [&]() {
        artificialMidi.addEvent(offMessage, 0);
    });
}

void MePianoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    if (!artificialMidi.isEmpty())
    {
        for (juce::MidiBufferIterator iter = artificialMidi.begin(); iter != artificialMidi.end(); iter++)
        {
            juce::MidiMessageMetadata metadata = *iter;
            std::string s = ">>>" + std::to_string(metadata.numBytes);
            juce::MidiMessage msg = metadata.getMessage();

            midiMessages.addEvent(msg, midiMessages.getLastEventTime());
        }
    }
    artificialMidi.clear();
    
    piano->renderNextBlock(buffer, midiMessages);
}

//==============================================================================
bool MePianoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MePianoAudioProcessor::createEditor()
{
    return new MePianoAudioProcessorEditor (*this);
}

//==============================================================================
void MePianoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MePianoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MePianoAudioProcessor();
}
