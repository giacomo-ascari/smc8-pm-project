/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MePianoAudioProcessorEditor :
    public juce::AudioProcessorEditor,
    private juce::Timer,
    public juce::Slider::Listener
{
public:
    MePianoAudioProcessorEditor (MePianoAudioProcessor&);
    ~MePianoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MePianoAudioProcessor& audioProcessor;
    juce::TextButton noteButton;
    juce::Slider velocitySlider;
    juce::Label  velocityLabel;
    juce::Slider noteSlider;
    juce::Label  noteLabel;
    juce::ToggleButton randomButton;
    juce::ToggleButton sinewaveButton;

    juce::Slider reverbSlider;
    juce::Label reverbLabel;

    juce::Slider gainSlider;
    juce::Label gainLabel;

    juce::Image image;

    juce::Random noteRand;

    void timerCallback() final {
        AudioProcessorEditor::repaint();
    }

    void sliderValueChanged(juce::Slider* slider) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MePianoAudioProcessorEditor)
};
