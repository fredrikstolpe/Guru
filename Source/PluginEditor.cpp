/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Guru2AudioProcessorEditor::Guru2AudioProcessorEditor (Guru2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{


    startTimer(5000);

    auto availableMidiOutDevices = juce::MidiOutput::getAvailableDevices();

    addAndMakeVisible(textLabel);
    textLabel.setFont(textFont);

    // add items to the combo-box
    addAndMakeVisible(midiOutMenu);

    int i = 0;
    for (juce::MidiDeviceInfo& device : availableMidiOutDevices)
    {
        midiOutMenu.addItem(device.name, i);
        i++;
    }

    midiOutMenu.onChange = [this] { midiOutMenuChanaged(); };
    midiOutMenu.setSelectedId(0);
    midiOutMenuChanaged();

    // these define the parameters of our slider object
    //midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    //midiVolume.setRange(0.0, 127.0, 1.0);
    //midiVolume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 0);
    //midiVolume.setPopupDisplayEnabled(true, false, this);
    //midiVolume.setTextValueSuffix(" Volume");
    //midiVolume.setValue(1.0);

    //addAndMakeVisible(&midiVolume);
    //midiVolume.addListener(this);

    midiMonitor.setMultiLine(true);
    midiMonitor.setReturnKeyStartsNewLine(false);
    midiMonitor.setReadOnly(true);
    midiMonitor.setScrollbarsShown(true);
    midiMonitor.setCaretVisible(false);
    midiMonitor.setPopupMenuEnabled(false);
    midiMonitor.setText({});
    addAndMakeVisible(&midiMonitor);


    setSize(732, 520);
}

void Guru2AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();
}

void Guru2AudioProcessorEditor::midiOutMenuChanaged()
{
    int id = midiOutMenu.getSelectedId();
    auto availableMidiOutDevices = juce::MidiOutput::getAvailableDevices();
    int i = 0;
    for (juce::MidiDeviceInfo& device : availableMidiOutDevices)
    {
        if (i == id) {
            audioProcessor.changeMidiDevice(device.identifier);
        }
        i++;
    }
}

Guru2AudioProcessorEditor::~Guru2AudioProcessorEditor()
{
}

//==============================================================================
void Guru2AudioProcessorEditor::paint (juce::Graphics& g)
{
    //// (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

    // fill the whole window white
    //g.fillAll(juce::Colours::white);

    g.setColour(juce::Colours::black);
    g.setFont(15.0f);

    //g.drawFittedText("Midi Volume", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void Guru2AudioProcessorEditor::resized()
{
    auto margin = 10;
    auto y = (getHeight() / 2) + ((2 * 24) + (3 * margin) + 64);
    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
    midiMonitor.setBounds(margin, y,
        getWidth() - (2 * margin), getHeight() - y - margin);
    textLabel.setBounds(10, 10, getWidth() - 20, 20);
    midiOutMenu.setBounds(10, 40, getWidth() - 20, 20);
}

Guru2AudioProcessor& Guru2AudioProcessorEditor::getProcessor()
{
    return static_cast<Guru2AudioProcessor&> (audioProcessor);
}

void Guru2AudioProcessorEditor::timerCallback()
{
    monitorMessage(getProcessor().lastValue);
}

void Guru2AudioProcessorEditor::monitorMessage(juce::String message)
{
    midiMonitor.insertTextAtCaret(message + "\n");
}

//void handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
//{
//    MidiMessage m(MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity));
//    m.setTimeStamp(Time::getMillisecondCounterHiRes() * 0.001);
//    sendToOutputs(m);
//}
