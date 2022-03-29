project(BassDivision VERSION 0.0.1)

set (BaseTargetName ${PROJECT_NAME})

juce_add_plugin("${BaseTargetName}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        #ICON_SMALL "../../Assets/LEVEL/LEVEL_app_icons/LEVEL_icon_32(28).png"
        #ICON_BIG   "../../Assets/LEVEL/LEVEL_app_icons/LEVEL_icon_64(52).png"
        COMPANY_NAME "Division Void"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Divv
        PLUGIN_CODE Bsdv
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "Bass Division")

juce_generate_juce_header(${BaseTargetName})

target_sources(${BaseTargetName} PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/GUI/MainPanel.cpp
        Source/GUI/Panels/BottomPanel.cpp)

target_compile_definitions(${BaseTargetName}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0)

target_link_libraries(${BaseTargetName} PRIVATE
        shared_resources
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_opengl
        juce::juce_dsp
        DivisionVoid)

if(APPLE)
    add_custom_command(TARGET ${BaseTargetName}_AU
            PRE_BUILD
            COMMAND rm -rf $ENV{HOME}/Library/Audio/Plug-Ins/Components/LEVEL.component)

    add_custom_command(TARGET ${PROJECT_NAME}_All
            POST_BUILD
            COMMAND python3 ~/PluginDevelopment/CoalescentSounds/CoalescentSoundsPlugins/Scripts/copy-binaries-to-gdrive.py)
endif()