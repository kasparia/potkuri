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
    attackSlider.setRange(0.01f, 1.0f, 0.02f);
    attackSlider.setValue(0.01f);
    attackSlider.addListener(this);
    attackSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, attackSlider.getTextBoxHeight());
    addAndMakeVisible(attackSlider);

    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.attachToComponent(&attackSlider, true);
    

    releaseSlider.setSliderStyle( juce::Slider::SliderStyle::LinearVertical );
    releaseSlider.setRange(0.01f, 1.0f, 0.01f);
    releaseSlider.setValue(0.01f);
    releaseSlider.addListener(this);
    releaseSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, releaseSlider.getTextBoxHeight());
    addAndMakeVisible(releaseSlider);

    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, true);
    

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
}

void AudioPluginAudioProcessorEditor::resized()
{
    attackSlider.setBounds(50, 30, 100, 140);
    releaseSlider.setBounds(200, 30, 100, 140);
    keyboardComponent.setBounds(30, 300, 500, 200);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged (juce::Slider *slider) {
    if (slider == &attackSlider) {
        processorRef.attackSliderValue = attackSlider.getValue();
    }
    if (slider == &releaseSlider) {
        processorRef.releaseSliderValue = releaseSlider.getValue();
    }
}
