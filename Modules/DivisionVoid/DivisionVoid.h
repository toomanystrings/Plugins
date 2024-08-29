//
// Created by Conor on 08/03/2022.
//

#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

      ID:               divisionvoid
      vendor:           DivisionVoid
      version:          0.0.1
      name:             divisionvoid
      description:      divisionvoid
      license:          GPL/Commercial
      dependencies:     juce_audio_processors, juce_core, juce_events
                        juce_gui_basics

     END_JUCE_MODULE_DECLARATION

#endif

// JUCE Includes
#include <JuceHeader.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>

// Binary files to include.
#include <SharedBinaryResources.h>

// We will need to include files from source here
#include "Source/Utilities.h"

// Stylings
#include "Source/UI Components/DivisionVoidFonts.h"
#include "Source/UI Components/DivisionVoidColours.h"
#include "Source/UI Components/DivisionVoidLookAndFeel/Runes.h"
#include "Source/UI Components/DivisionVoidLookAndFeel/LookAndFeel.h"

// Components
#include "Source/UI Components/AudioMeter.h"
#include "Source/UI Components/SegmentedDisplayMeter.h"
#include "Source/UI Components/DivisionVoidLookAndFeel/Button.h"
#include "Source/UI Components/EqWidget.h"
#include "DivisionVoid/Source/UI Components/DoubleStackKnob.h"

// FX processors
#include "Source/Processors/FX/Compressor.h"
#include "Source/Processors/FX/Distortion.h"
#include "Source/Processors/LevelMeterProcessor.h"

// Utility Audio classes


// FFT
//#include "Source/FFT/*.h"

