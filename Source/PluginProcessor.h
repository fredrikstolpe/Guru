/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthParameter.h"

//==============================================================================
/**
*/
class Guru2AudioProcessor  : public juce::AudioProcessor,
                                public juce::AudioProcessorValueTreeState::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    Guru2AudioProcessor();
    ~Guru2AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void addMessageToBuffer(const juce::MidiMessage& message);

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float noteOnVel;

    juce::String lastValue;

    juce::AudioProcessorValueTreeState treeState;

    void parameterChanged(const juce::String &parameterID, float newValue) override;

    void changeMidiDevice(juce::String& identifier);

    std::map<juce::String, SynthParameter*> parameterDict;

private:
    std::map<juce::String, SynthParameter*> createParameterDict();

    double startTime;

    std::unique_ptr<juce::MidiOutput> midiOutput;

    juce::MidiBuffer midiBuffer;
    double sampleRate = 44100.0;
    int previousSampleNumber = 0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Guru2AudioProcessor)
};
