/*
  ==============================================================================

    SynthParameter.h
    Created: 19 Jan 2023 8:44:54am
    Author:  fredrik.stolpe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiParameterType.h"

class SynthParameter
{
public:
    SynthParameter(juce::String id, juce::String name, MidiParameterType type, int ccNumber);
    juce::String id;
    juce::String name;
    MidiParameterType type;
    int ccNumber;
    int minValue;
    int maxValue;
    int defaultValue;
};