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
class Guru2AudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener,
    private juce::Timer
{
public:
    Guru2AudioProcessorEditor (Guru2AudioProcessor&);
    ~Guru2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    Guru2AudioProcessor& getProcessor();
    void timerCallback() override;
    void monitorMessage(juce::String);

private:
    void sliderValueChanged(juce::Slider* slider) override; // [3]

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Guru2AudioProcessor& audioProcessor;

    juce::TextEditor midiMonitor{ "MIDI Monitor" };
    juce::Slider midiVolume; // [1]

    juce::Label textLabel{ {}, "The quick brown fox jumps over the lazy dog." };
    juce::Font textFont{ 12.0f };
    juce::ComboBox midiOutMenu;

    void midiOutMenuChanaged();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Guru2AudioProcessorEditor)
};
