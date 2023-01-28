/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthParameter.h"

#define CUTOFF_ID "cutoff"
#define CUTOFF_NAME "VCF Cutoff"

//==============================================================================
Guru2AudioProcessor::Guru2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr)
#endif
{
    parameterDict = createParameterDict();

    std::map<juce::String, SynthParameter*>::iterator it;

    for (it = parameterDict.begin(); it != parameterDict.end(); it++)
    {
        treeState.createAndAddParameter(std::make_unique<juce::AudioParameterInt>(it->first, it->second->name, it->second->minValue, it->second->maxValue, it->second->defaultValue));
        treeState.addParameterListener(it->first, this);
    }

    lastValue = "";
}

Guru2AudioProcessor::~Guru2AudioProcessor()
{
}

//==============================================================================
const juce::String Guru2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Guru2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Guru2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Guru2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Guru2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Guru2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Guru2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Guru2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Guru2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Guru2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Guru2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Guru2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Guru2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Guru2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    juce::MidiBuffer processedMidi;

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;

        if (message.isNoteOn())
        {
            message = juce::MidiMessage::noteOn(message.getChannel(),
                message.getNoteNumber(),
                (juce::uint8)noteOnVel);
        }

        if (message.isController())
        {
            int cc = message.getControllerNumber();
            switch (cc)
            {
                case 14:
                    cc = 9;
                case 15:
                    cc = 74;
                case 82:
                    cc = 77;
                case 19:
                    cc = 11;
                case 22:
                    cc = 75;
            }
            message = juce::MidiMessage::controllerEvent(message.getChannel(), cc, message.getControllerValue());
        }
        processedMidi.addEvent(message, time);
    }

    midiMessages.swapWith(processedMidi);
}

void Guru2AudioProcessor::parameterChanged(const juce::String & parameterID, float newValue)
{
    int ccValue = static_cast<int>(newValue);
    lastValue = parameterID + " : " + std::to_string(ccValue) + " : " + std::to_string(parameterDict[parameterID]->ccNumber);
    auto message = juce::MidiMessage::controllerEvent(1, parameterDict[parameterID]->ccNumber, (juce::uint8)ccValue);
    //message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
    addMessageToBuffer(message);
}

void Guru2AudioProcessor::addMessageToBuffer(const juce::MidiMessage& message)
{
    auto timestamp = message.getTimeStamp();
    auto sampleNumber = (int)(timestamp * sampleRate);
    midiBuffer.addEvent(message, sampleNumber);
}

//==============================================================================
bool Guru2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Guru2AudioProcessor::createEditor()
{
    return new Guru2AudioProcessorEditor (*this);
}

//==============================================================================
void Guru2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Guru2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

std::map<juce::String, SynthParameter*> Guru2AudioProcessor::createParameterDict() {
    std::map<juce::String, SynthParameter*> dict;

    dict.insert(std::make_pair("LPF_CUTOFF", new SynthParameter("LPF_CUTOFF", "Cutoff", CC, 100)));
    dict.insert(std::make_pair("LPF_RESONANCE", new SynthParameter("LPF_RESONANCE", "Resonance", CC, 101)));

    return dict;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Guru2AudioProcessor();
}
