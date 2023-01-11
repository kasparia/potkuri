#pragma once
#include <JuceHeader.h>


class SynthOscillator : public juce::dsp::Oscillator<float> {

  public:
    void setWaveType (const int waveChoice);
    void setWaveFrequency (const int midiNoteNumber);

  private:

};