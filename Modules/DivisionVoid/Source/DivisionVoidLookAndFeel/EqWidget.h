#pragma once

// Here we are going to need to think about structure.
// We will need a overall EQ visualiser component, with a FFT, unless the FFT is the responsibility of a component that
// lives underneath.
// We will need a response curve viewer, which responds to EQ coefficients

namespace DivisionVoid
{
class EqVisualiser : public juce::Component, juce::Slider::Listener
{
public:
    enum EqSetting { Gain, Freq, Q };

    class Node : public juce::Component
    {
    public:
        Node()
        {
            constrainer.setMinimumOnscreenAmounts(
                    nodeWidth,
                    nodeWidth,
                    nodeWidth,
                    nodeWidth);
        }

        void mouseDown(const juce::MouseEvent &event) override
        {
            dragger.startDraggingComponent(this, event);
        }

        void mouseDrag(const juce::MouseEvent &event) override
        {
            dragger.dragComponent(this, event, &constrainer);
        }

        void mouseWheelMove(const juce::MouseEvent &event, const juce::MouseWheelDetails &wheel) override
        {
        }

        void paint(juce::Graphics &g) override
        {
            g.setColour(juce::Colours::green);
            g.fillEllipse(getLocalBounds().toFloat());
        }

    private:
        juce::ComponentDragger dragger;
        juce::ComponentBoundsConstrainer constrainer;
    };

    EqVisualiser()
    {
        addAndMakeVisible(node);
    }
    //~EqVisualiser();

    void registerSlider(juce::Slider* slider, EqSetting setting)
    {
        // Probably need to register a listener here at some point
        const std::lock_guard<std::mutex> lock(mutex);
        if (setting == Gain)
            gainSliders.push_back(slider);
        if (setting == Freq)
            freqSliders.push_back(slider);
        if (setting == Q)
            qSliders.push_back(slider);

    }

    void deregisterSlider( juce::Slider* slider)
    {
        //slider->removeListener(this);
        const std::lock_guard<std::mutex> lock(mutex);
        freqSliders.erase(std::remove(freqSliders.begin(), freqSliders.end(), slider), freqSliders.end());
        gainSliders.erase(std::remove(gainSliders.begin(), gainSliders.end(), slider), gainSliders.end());
        qSliders.erase(std::remove(qSliders.begin(), qSliders.end(), slider), qSliders.end());
    }

    void paint(juce::Graphics& g) override
    {
        const auto bounds = getLocalBounds().toFloat();

        g.setColour(DivisionVoidColours::black);
        g.fillRect(bounds);
    }

    void resized() override
    {
        node.setBounds(getLocalBounds().withSizeKeepingCentre(nodeWidth, nodeWidth));
    }

private:
    void sliderValueChanged(juce::Slider *slider) override
    {
    }

    std::vector<juce::Slider*> gainSliders, freqSliders, qSliders;
    Node node;

    std::mutex mutex;

    static constexpr int nodeWidth = 10;
};








    // These structs have problems, but could be leveraged to work at a later date
    /*template<typename PathType>
    struct AnalyserPathGenerator
    {
        void generatePath(const std::vector<float>& renderData, juce::Rectangle<float> fftBounds, int fftSize, float binWidth, float negInfinity)
        {
            auto top = fftBounds.getY();
            auto bottom = fftBounds.getHeight();
            auto width = fftBounds.getWidth();

            int numBins = (int)fftSize / 2;

            PathType p;
            p.preallocateSpace(3 * (int)fftBounds.getWidth());

            auto map = [bottom, top, negInfinity](float v)
            {
                return juce::jmap(v, negInfinity, 0.0f, (float)bottom, top);
            };

            auto y = map(renderData[0]);

            jassert(!std::isnan(y) && !std::isinf(y));

            p.startNewSubPath(0, y);

            const int pathResolution = 1;

            for (int binNum = 1; binNum < numBins; binNum += pathResolution)
            {
                y = map(renderData[binNum]);

                jassert(!std::isnan(y) && !std::isinf(y));

                if (!std::isnan(y) && !std::isinf(y))
                {
                    auto binFreq = binNum * binWidth;
                    auto normalisedBinX = juce::mapFromLog10(binFreq, 20.0f, 20000.0f);
                    int binX = std::floor(normalisedBinX * width);
                    p.lineTo(binX, y);
                }
            }

            pathFifo.push(p);
        }

        int getNumPathsAvailable() const
        {
            return pathFifo.getNumAvailableForReading();
        }

        bool getPath(PathType& path)
        {
            return pathFifo.pull(path);
        }
    private:
        Fifo<PathType> pathFifo;
    };
    struct EqCurve : public juce::Timer, juce::AudioProcessorParameter::Listener, juce::Component
    {
    public:
        EqCurve(juce::AudioProcessor& inProcessor):  audioProcessor(inProcessor)
        {
            auto const &params = audioProcessor.getParameters();
            for (auto param: params)
                param->addListener(this);

            startTimerHz(60);

            updateMonoChain();
        }

        ~EqCurve()
        {
            auto const& params = audioProcessor.getParameters();
            for (auto param :params)
                param->removeListener(this);

            stopTimer();
        }

        void paint(juce::Graphics& g) override {
            //auto bounds = getLocalBounds();
            //auto w = bounds.getWidth();
            g.fillAll(juce::Colours::black);
            g.drawImage(background, getLocalBounds().toFloat());

            auto responseArea = getAnalysisArea();
            auto w = responseArea.getWidth();

            // This draws an EQ response curve. Currently working!!

            auto &lowShelf = monochain.get<ChainPositions::lowShelf>();
            auto &band1 = monochain.get<ChainPositions::band1>();
            auto &band2 = monochain.get<ChainPositions::band2>();
            auto &band3 = monochain.get<ChainPositions::band3>();
            auto &band4 = monochain.get<ChainPositions::band4>();
            auto &band5 = monochain.get<ChainPositions::band5>();
            auto &highShelf = monochain.get<ChainPositions::highShelf>();

            auto sampleRate = audioProcessor.getSampleRate();

            std::vector<double> mags;
            mags.resize(w);

            for (int i = 0; i < w; ++i) {
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
            auto map = [outputMin, outputMax](double input) {
                return juce::jmap(input, -36.0, 36.0, outputMin, outputMax);
            };

            responseCurve.startNewSubPath(responseArea.getX(), map(mags.front()));

            for (size_t i = 0; i < mags.size(); ++i) {
                responseCurve.lineTo(responseArea.getX() + i, map(mags[i]));
            }

            g.setColour(juce::Colours::white);
            g.strokePath(responseCurve, juce::PathStrokeType(2.0f));
        }

        void resized() override;

        void timerCallback() override;

        void parameterValueChanged(int parameterIndex, float newValue) override;
        void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    private:
        MonoChain monochain;
        juce::Atomic<bool> parametersChanged {false};

        void updateMonoChain();

        juce::Rectangle<int> getRenderArea();
        juce::Rectangle<int> getAnalysisArea();

        juce::Image background;

        juce::AudioProcessor& audioProcessor;

        juce::AudioBuffer<float> monoBuffer;
        AnalyserPathGenerator<juce::Path> pathProducer;
    };*/
}