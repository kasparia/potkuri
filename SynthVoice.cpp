#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
  baseNote = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
  osc.setFrequency(baseNote);
  adsr.noteOn();

  runningTime = 0.0;
  runningNote = 3.0 * baseNote;
}

void SynthVoice::stopNote (float velocity, bool allowTailOff) {
  adsr.noteOff();
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {
    
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue) {
    
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels) {
  adsr.setSampleRate(sampleRate);

  adsrParams.attack = 0.0f;
  adsrParams.decay = 0.5f;
  adsrParams.sustain = 0.5f;
  adsrParams.release = 0.2f;


  adsr.setParameters(adsrParams);

  juce::dsp::ProcessSpec spec;
  spec.maximumBlockSize = samplesPerBlock;
  spec.sampleRate = sampleRate;
  spec.numChannels = outputChannels;
  osc.prepare(spec);
  gain.prepare(spec);

  osc.setFrequency(440.0f);
  gain.setGainLinear(0.25f);

  isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) {
  jassert(isPrepared);

  setPitchValue();

  juce::dsp::AudioBlock<float> audioBlock { outputBuffer };
  osc.process( juce::dsp::ProcessContextReplacing<float> (audioBlock) );
  gain.process( juce::dsp::ProcessContextReplacing<float> (audioBlock) );

  adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void SynthVoice::setADSRParameters(float attack, float release) {
  adsrParams.attack = attack;
  adsrParams.release = release;
  adsr.setParameters(adsrParams);
}

void SynthVoice::setPitchValue() {
  runningTime += 0.01;
  
  if (runningNote > 40.0) {
    runningNote = runningNote - abs(15.0 * sin(200000.0 / (2000.0 + runningTime)));
  }

  if (runningNote < 40.0) {
    runningNote = 20.0;
  }

  // std::cout << "runner:" << runningTime << std::endl;
  std::cout << "note:" << runningNote << std::endl;
  osc.setFrequency(runningNote);
  
}
