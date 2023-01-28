/*
  ==============================================================================

    SynthParameter.cpp
    Created: 19 Jan 2023 8:44:54am
    Author:  fredrik.stolpe

  ==============================================================================
*/

#include "SynthParameter.h"

SynthParameter::SynthParameter(juce::String id, int ccNumber)
{
    this->id = id;
    this->ccNumber = ccNumber;
}
