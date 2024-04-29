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

    image = juce::ImageFileFormat::loadFrom(BinaryData::background_png, BinaryData::background_pngSize);

    addAndMakeVisible(noteButton);
    noteButton.setTriggeredOnMouseDown(false);
    noteButton.onClick = [&]() {
        // released
        audioProcessor.addArtificialMidi(velocitySlider.getValue(), noteSlider.getValue(), false);
    };
    noteButton.onStateChange = [&]() {
        if (noteButton.isDown()) {
            // pressed
            audioProcessor.addArtificialMidi(velocitySlider.getValue(), noteSlider.getValue(), true);
        }
    };

    addAndMakeVisible(velocitySlider);
    velocitySlider.setRange(1, 127, 1);
    velocitySlider.addListener(this);

    addAndMakeVisible(velocityLabel);
    velocityLabel.setText("Velocity", juce::dontSendNotification);
    velocityLabel.attachToComponent(&velocitySlider, true);

    addAndMakeVisible(noteSlider);
    noteSlider.setRange(33, 117, 1);
    noteSlider.addListener(this);

    addAndMakeVisible(noteLabel);
    noteLabel.setText("Note #", juce::dontSendNotification);
    noteLabel.attachToComponent(&noteSlider, true);

    setSize (600, 450);
    startTimerHz(10);
}

MePianoAudioProcessorEditor::~MePianoAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void MePianoAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.drawImage(image, 0, 0, 600, 450, 0, 0, 600, 450);

   
    g.setFont(15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    int voiceCount = 0;
    Key** voices = audioProcessor.piano->getVoices(voiceCount);

    std::string text;
    
    text = "Voices (max. " + std::to_string(voiceCount) + ")";
    g.setColour(juce::Colours::white);
    g.drawText(text, 20, 15, 200, 100, juce::Justification::topLeft, true);

    text = "# / note / time / strings";
    g.setColour(juce::Colours::grey);
    g.drawText(text, 20, 40, 200, 100, juce::Justification::topLeft, true);

    for (int i = 0; i < voiceCount; i++)
    {
        text = std::to_string(i);
        text = text + " " + std::to_string(voices[i]->getMidiNote());
        text = text + " " + std::to_string(voices[i]->getTime() / audioProcessor.getSampleRate());
        text = text + " " + std::to_string(voices[i]->getStrings());
        text = text + " " + std::to_string(voices[i]->getLastValue());
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
        g.drawText(text, 20, 59+i * 19, 200, 100, juce::Justification::topLeft, true);
    }

    if (audioProcessor.piano->getHasClipped())
    {
        g.setColour(juce::Colours::orange);
        g.drawText("CLIP", 140, 15, 200, 100, juce::Justification::topLeft, true);
    }
}

void MePianoAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //if (slider == &velocitySlider)
    //    durationSlider.setValue(1.0 / velocitySlider.getValue(), juce::dontSendNotification);
}

void MePianoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int w = 300;
    int h = 25;
    int p = 15;
    noteButton.setBounds(getWidth() - w - p, p, w, h);
    velocitySlider.setBounds(getWidth() - w - p, 2 * p + h, w, h);
    noteSlider.setBounds(getWidth() - w - p, 3 * p + 2 * h, w, h);
}