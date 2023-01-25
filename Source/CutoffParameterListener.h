/*
  ==============================================================================

    CutoffParameterListener.h
    Created: 24 Jan 2023 12:36:33pm
    Author:  fredrik.stolpe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CutoffParameterListener : private juce::AudioProcessorValueTreeState::Listener
    : parameters(*this, nullptr)
{
public:
    CutoffParameterListener() {
        parameters.addParameterListener("yourParamId", this);
    }
private:
    void parameterChanged(const String & parameterID, float newValue) override
    {
        // Called when parameter "yourParamId" is changed.
    }
};
