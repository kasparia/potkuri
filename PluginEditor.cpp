#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p),
      keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      startTime (juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible (keyboardComponent);
    keyboardState.addListener (this);


    attackSlider.setSliderStyle( juce::Slider::SliderStyle::LinearVertical );
    attackSlider.setRange(0.01f, 1.0f, 0.01f);
    attackSlider.setValue(0.1f);
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);

    setSize (640, 480);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    attackSlider.setBounds(30, 30, 50, 80);
    keyboardComponent.setBounds(30, 300, 500, 200);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged (juce::Slider *slider) {
    if (slider == &attackSlider) {
        processorRef.attackSliderValue = attackSlider.getValue();
    }
}
