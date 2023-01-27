/*
  ==============================================================================

    SynthParameter.cpp
    Created: 19 Jan 2023 8:44:54am
    Author:  fredrik.stolpe

  ==============================================================================
*/

#include "SynthParameter.h"

SynthParameter::SynthParameter(juce::String id, juce::String name, MidiParameterType type, int ccNumber)
{
    this->id = id;
    this->name = name;
    this->type = type;
    this->ccNumber = ccNumber;
}
