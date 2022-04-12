/*
  ==============================================================================

    SpectroscopeViewer.h
    Created: 27 Jul 2014 2:34:48pm
    Author:  Samuel Gaehwiler

    Heavily based on the dRowAudio Spectroscope class by David Rowland.

  ==============================================================================
*/

#ifndef SPECTRUM_VIEWER_H_INCLUDED
#define SPECTRUM_VIEWER_H_INCLUDED

#include "SpectrumAnalyserHeader.h"
#define _USE_MATH_DEFINES
#include <cmath>
//#include <corecrt_math_defines.h>

#if JUCE_MAC || JUCE_IOS || DROWAUDIO_USE_FFTREAL



//==============================================================================
/** Creates a standard Spectroscope.
    This will display the amplitude of each frequency bin from an FFT in a
    continuous line which will decay with time.
    This is very simple to use, it is a GraphicalComponent so just register one
    with a TimeSliceThread, make sure its running and then continually call the
    copySamples() method. The FFT itself will be performed on a background thread.
 */
class SpectrumViewer :  public juce::Component,
                        public juce::Value::Listener,
						public juce::Timer
{
public:
    //==============================================================================
    /** Provide it with the parameters from the SpectrumViewer.
     */
	SpectrumViewer (juce::Value& repaintViewerValue,
	                drow::Buffer& magnitudeBuffer,
	                juce::Value& detectedFrequencyValue);
	
    /** Destructor. */
	~SpectrumViewer();
	
    /** @internal */
	void resized() override;
	
    /** @internal */
	void paint (juce::Graphics &g) override;

    //==============================================================================
    void setSampleRate (double newSampleRate);
    
    int getHeightOfFrequencyCaption();

    juce::Value & getFrequencyToDisplay();

	void timerCallback() override;
    
    void valueChanged (juce::Value &value) override;
    
    void mouseEnter (const juce::MouseEvent &event) override;
    
    void mouseMove (const juce::MouseEvent &event) override;
    
    void mouseExit (const juce::MouseEvent &event) override;
	

private:
    //==============================================================================
    void createGradientImage();

    juce::Path get571RecordingStudioPath();
    
    static const int frequenciesToPlot[];
    static const int numberOfFrequenciesToPlot;
    
    double sampleRate;
    juce::Value repaintViewer;
    drow::Buffer& fftMagnitudeBuffer;

	
    juce::OpenGLContext openGL;
    
	
    juce::Value detectedFrequency;
    juce::Value frequencyToDisplay;
    bool mouseMode;
    int mouseXPosition;
    
    //==============================================================================
    /*  The numbers below the graph, indication the frequency.
     */
    class FrequencyCaption    : public Component
    {
    public:
        FrequencyCaption();
        ~FrequencyCaption();
        
        void paint (juce::Graphics&);
        void resized();
        
        void setSampleRate (double newSampleRate);
        
    private:
        juce::OwnedArray<juce::Label> frequencyLabels;
        
        double sampleRate;
        DivisionVoidFonts fonts;
    };
    //==============================================================================
    
    FrequencyCaption frequencyCaption;
    int heightForFrequencyCaption;

    juce::Image gradientImage;

    juce::Path path571;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumViewer);
};

#endif  // #if JUCE_MAC || JUCE_IOS || DROWAUDIO_USE_FFTREAL
#endif  // SPECTRUM_VIEWER_H_INCLUDED
