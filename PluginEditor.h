#pragma once


#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_utils/juce_audio_utils.h>

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Slider::Listener,
                                        private juce::MidiInputCallback,
                                        private juce::MidiKeyboardStateListener
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    /*    */
    juce::AudioDeviceManager deviceManager;
    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;

    juce::Label attackLabel;
    juce::Slider attackSlider;

    juce::Label decayLabel;
    juce::Slider decaySlider;

    juce::Label releaseLabel;
    juce::Slider releaseSlider;
    
    juce::Label modulationLabel;
    juce::Slider modulationSlider;

    juce::Label waveformLabel;
    juce::ComboBox waveformSelect;

    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage (juce::MidiInput* source, const juce::MidiMessage& message) override
    {
        keyboardState.processNextMidiEvent (message);
    }

    void handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        auto midiNoteToOsc = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        processorRef.synthPlayNote(midiNoteNumber, velocity);
    }

    void handleNoteOff (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override
    {
        // auto m = juce::MidiMessage::noteOff (midiChannel, midiNoteNumber);
        processorRef.synthStopNote(midiNoteNumber, velocity);
    }

     
    juce::TextEditor midiMessagesBox;
    double startTime;

    /** Starts listening to a MIDI input device, enabling it if necessary. */
    void setMidiInput (int index)
    {
        auto list = juce::MidiInput::getAvailableDevices();

        deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);

        auto newInput = list[index];

        if (! deviceManager.isMidiInputDeviceEnabled (newInput.identifier))
            deviceManager.setMidiInputDeviceEnabled (newInput.identifier, true);

        deviceManager.addMidiInputDeviceCallback (newInput.identifier, this);
        midiInputList.setSelectedId (index + 1, juce::dontSendNotification);

        lastInputIndex = index;
    }




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
