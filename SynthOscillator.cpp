#include "SynthOscillator.h"

void SynthOscillator::setWaveType(const int waveChoice) {
    switch (waveChoice) {
        case 0:
            // sine wave
            initialise(
                [](float x) {
                    return std::sin(x);
                }
            );
            break;
        case 1: 
            // saw wave
            initialise(
                [](float x) {
                    return x / juce::MathConstants<float>::pi;;
                }
            );
            break;
        case 2:
            // square wave
            initialise(
                [](float x) {
                    return x < 0.0f ? -1.0f : 1.0f;
                }
            );
            break;
        default:
            jassertfalse;
            break;
    }
}

void SynthOscillator::setWaveFrequency (const int midiNoteNumber){
    setFrequency ( juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) );
}