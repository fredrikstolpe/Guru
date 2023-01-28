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
    SynthParameter(juce::String id, int ccNumber);
    juce::String id;
    MidiParameterType type = CC;
    int ccNumber;
    int minValue = 0;
    int maxValue = 128;
    int defaultValue = 0;
};