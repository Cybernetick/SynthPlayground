cmake_minimum_required(VERSION 3.15)

project(SynthPlayground VERSION 0.0.1) #creates PROJECT_NAME variable for later use

find_package(JUCE CONFIG REQUIRED) #look for JUCE install folder to add as dependency instead of putting JUCE inside the working dir as submodule. Don't forget the CMAKE_PREFIX_PATH argument

juce_add_plugin(${PROJECT_NAME}
        PLUGIN_MANUFACTURER_CODE Juce
        PLUGIN_CODE Dem0
        FORMATS AU VST3 Standalone
        NEEDS_MIDI_INPUT TRUE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PRODUCT_NAME "Synth Demo") #this creates the "target" for later linking the sources and stuff.

juce_generate_juce_header(SynthPlayground)

target_sources(${PROJECT_NAME}
        PRIVATE
        ADSRVisualiserComponent.cpp
        ADSRVisualizerSlider.cpp
        ADSRVisualizerSliderLaF.h
        MainLayoutDrawer.cpp
        OscVoice.cpp
        OscSound.h
        SynthParameters.h
        UIEventsListener.h
        Waveforms.h
        WaveSelectionButtonLaF.h
        PluginEditor.cpp
        PluginProcessor.cpp)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils
        juce::juce_dsp
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)