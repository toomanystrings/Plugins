/*
  ==============================================================================

    BDFftPanel.cpp
    Created: 21 Feb 2021 3:59:03pm
    Author:  Olorin

  ==============================================================================
*/

#include "BDFftPanel.h"

EqCurve::EqCurve(BassDivisionAudioProcessor& inProcessor): BDPanelBase(inProcessor), audioProcessor(inProcessor), leftChannelFifo(&audioProcessor.leftChannelFifo)
{
    auto const& params = audioProcessor.getParameters();
    for (auto param : params)
        param->addListener(this);
    
    leftChannelFFTDataGenerator.changeOrder(FFTOrder::order4096);
    monoBuffer.setSize(1, leftChannelFFTDataGenerator.getFFTSize());
    
    startTimerHz(60);
    
    updateMonoChain();
}

EqCurve::~EqCurve()
{
    auto const& params = audioProcessor.getParameters();
    for (auto param :params)
        param->removeListener(this);
    
    stopTimer();
}

void EqCurve::paint(juce::Graphics& g)
{
    //auto bounds = getLocalBounds();
    //auto w = bounds.getWidth();
    g.fillAll(juce::Colours::black);
    g.drawImage(background, getLocalBounds().toFloat());
    
    auto responseArea = getAnalysisArea();
    auto w = responseArea.getWidth();
    
    // This draws an EQ response curve. Currently working!!
    
    auto& lowShelf = monochain.get<ChainPositions::lowShelf>();
    auto& band1 = monochain.get<ChainPositions::band1>();
    auto& band2 = monochain.get<ChainPositions::band2>();
    auto& band3 = monochain.get<ChainPositions::band3>();
    auto& band4 = monochain.get<ChainPositions::band4>();
    auto& band5 = monochain.get<ChainPositions::band5>();
    auto& highShelf = monochain.get<ChainPositions::highShelf>();

    auto sampleRate = audioProcessor.getSampleRate();

    std::vector<double> mags;
    mags.resize(w);

    for (int i = 0; i < w; ++i)
    {
        double mag = 1.0;
        auto freq = juce::mapToLog10(double(i) / double(w), 20.0, 20000.0);

        mag *= lowShelf.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        mag *= band1.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        mag *= band2.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        mag *= band3.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        mag *= band4.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        mag *= band5.coefficients->getMagnitudeForFrequency(freq, sampleRate);
        mag *= highShelf.coefficients->getMagnitudeForFrequency(freq, sampleRate);

        mags[i] = juce::Decibels::gainToDecibels(mag);
    }

    juce::Path responseCurve;

    const double outputMin = responseArea.getBottom();
    const double outputMax = responseArea.getY();
    auto map = [outputMin, outputMax](double input)
    {
        return juce::jmap(input, -36.0, 36.0, outputMin, outputMax);
    };

    responseCurve.startNewSubPath(responseArea.getX(), map(mags.front()));

    for (size_t i = 0; i < mags.size(); ++i)
    {
        responseCurve.lineTo(responseArea.getX() + i, map(mags[i]));
    }
    
    leftChannelFFTPath.applyTransform(juce::AffineTransform().translation(responseArea.getX(), responseArea.getY()));

    g.setColour(juce::Colours::red);
    g.strokePath(leftChannelFFTPath, juce::PathStrokeType(2.0f));
    
    g.setColour(juce::Colours::white);
    g.strokePath(responseCurve, juce::PathStrokeType(2.0f));
}

void EqCurve::resized()
{
    background = juce::Image(juce::Image::PixelFormat::RGB, getWidth(), getHeight(), true);
    
    juce::Graphics g(background);
}

void EqCurve::timerCallback()
{
    while (leftChannelFifo->getNumCompleteBuffersAvailable() > 0)
    {
        juce::AudioBuffer<float> tempIncomingBuffer;
        if (leftChannelFifo->getAudioBuffer(tempIncomingBuffer))
        {
            auto size = tempIncomingBuffer.getNumSamples();
            
            juce::FloatVectorOperations::copy(monoBuffer.getWritePointer(0, 0), monoBuffer.getReadPointer(0, size), monoBuffer.getNumSamples() - size);
            
            juce::FloatVectorOperations::copy(monoBuffer.getWritePointer(0, monoBuffer.getNumSamples() - size), tempIncomingBuffer.getReadPointer(0, 0), size);
            
            leftChannelFFTDataGenerator.produceFFTDataForRendering(monoBuffer, -60.0f);
        }
    }
    
    const auto fftBounds = getAnalysisArea().toFloat();
    const auto fftSize = leftChannelFFTDataGenerator.getFFTSize();
    const auto binWidth = audioProcessor.getSampleRate() / (double)fftSize;
    
    while (leftChannelFFTDataGenerator.getNumAvailableFFTDataBlocks() > 0)
    {
        std::vector<float> fftData;
        if (leftChannelFFTDataGenerator.getFFTData(fftData))
        {
            pathProducer.generatePath(fftData, fftBounds, fftSize, binWidth, -60.0f);
        }
    }
    
    while (pathProducer.getNumPathsAvailable())
    {
        pathProducer.getPath(leftChannelFFTPath);
    }
    
    if (parametersChanged.compareAndSetBool(false, true))
    {
        updateMonoChain();
    }
    
    repaint();
}

void EqCurve::updateMonoChain()
{
    auto chainSettings = getChainSettings(audioProcessor.treeState);
    auto sampleRate = audioProcessor.getSampleRate();
    
    auto lowShelfCoeffs = createFilter(chainSettings, sampleRate, ChainPositions::lowShelf);
    auto band1Coeffs = createFilter(chainSettings, sampleRate, ChainPositions::band1);
    auto band2Coeffs = createFilter(chainSettings, sampleRate, ChainPositions::band2);
    auto band3Coeffs = createFilter(chainSettings, sampleRate, ChainPositions::band3);
    auto band4Coeffs = createFilter(chainSettings, sampleRate, ChainPositions::band4);
    auto band5Coeffs = createFilter(chainSettings, sampleRate, ChainPositions::band5);
    auto highShelfCoeffs = createFilter(chainSettings, sampleRate, ChainPositions::highShelf);
    
    updateCoefficients(monochain.get<ChainPositions::lowShelf>().coefficients, lowShelfCoeffs);
    updateCoefficients(monochain.get<ChainPositions::band1>().coefficients, band1Coeffs);
    updateCoefficients(monochain.get<ChainPositions::band2>().coefficients, band2Coeffs);
    updateCoefficients(monochain.get<ChainPositions::band3>().coefficients, band3Coeffs);
    updateCoefficients(monochain.get<ChainPositions::band4>().coefficients, band4Coeffs);
    updateCoefficients(monochain.get<ChainPositions::band5>().coefficients, band5Coeffs);
    updateCoefficients(monochain.get<ChainPositions::highShelf>().coefficients, highShelfCoeffs);
}

void EqCurve::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}

juce::Rectangle<int> EqCurve::getRenderArea()
{
    return getLocalBounds();
}

juce::Rectangle<int> EqCurve::getAnalysisArea()
{
    return getLocalBounds();
}

//===============================================================================
BDFftPanel::BDFftPanel(BassDivisionAudioProcessor& inProcessor,
                       juce::Value& repaintSpectrumViewer,
                       drow::Buffer& spectrumMagnitudeBuffer,
                       juce::Value& detectedFrequency)
						: BDPanelBase(inProcessor), spectrumViewer(repaintSpectrumViewer, spectrumMagnitudeBuffer, detectedFrequency), /*eqCurve(inProcessor),*/ audioProcessor(inProcessor)
{
    //addAndMakeVisible(eqCurve);
    addAndMakeVisible(&spectrumViewer);
}

BDFftPanel::~BDFftPanel()
{
}

void BDFftPanel::paint(juce::Graphics& g)
{
    BDPanelBase::paint(g);
}

void BDFftPanel::resized()
{
    auto bounds = getBounds();
    spectrumViewer.setBounds(bounds);
}
