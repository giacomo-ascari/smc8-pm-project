/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MePianoAudioProcessorEditor::MePianoAudioProcessorEditor (MePianoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 450);
    startTimerHz(10);
}

MePianoAudioProcessorEditor::~MePianoAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void MePianoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    int voiceCount = 0;
    Key** voices = audioProcessor.piano->getVoices(voiceCount);

    g.drawText(std::to_string(voiceCount), 200, 0, 100, 100, juce::Justification::centred, true);

    for (int i = 0; i < voiceCount; i++)
    {
        juce::String text = "v #" + std::to_string(i);
        text = text + " " + std::to_string(voices[i]->getMidiNote());
        text = text + " " + std::to_string(voices[i]->getTime() / audioProcessor.getSampleRate());
        text = text + " " + std::to_string(voices[i]->getStrings());
        switch (voices[i]->getState()) {
        case ATTACKING:
            g.setColour(juce::Colours::greenyellow);
            break;
        case DECAYING:
            g.setColour(juce::Colours::yellow);
            break;
        case RELEASING:
            g.setColour(juce::Colours::white);
            break;
        case IDLE:
            g.setColour(juce::Colours::grey);
            break;
        }
        g.drawText(text, 20, 30+i * 20, 200, 100, juce::Justification::topLeft, true);
    }
}

void MePianoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
