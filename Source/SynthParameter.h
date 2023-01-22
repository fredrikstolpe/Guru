/*
  ==============================================================================

    SynthParameter.h
    Created: 19 Jan 2023 8:44:54am
    Author:  fredrik.stolpe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthParameter
{
public:
    //Id
    juce::String name;
    //type; // "sysex", "cc", "nrpn", "pitchbend"
    int number;
    int lsbNumber;
    int defaultValue;
    int minValue;
    int maxValue;
    int displayMinValue;
    int displayMaxValue;
    int displayValueFactor = 1;
    int randomizeMinValue;
    int randomizeMaxValue;
    //items;
    //itemValues;
    //guiType
    juce::String sysexMessageTemplate;
    //valueCallback ?
};