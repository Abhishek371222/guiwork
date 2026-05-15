#pragma once

#include <JuceHeader.h>

/**
 * Manages all DSP parameters, value conversions, and preset storage.
 */
class ParameterManager
{
public:
    struct Parameter {
        juce::String id;
        juce::String name;
        juce::String description;
        int nrpnAddress;
        int midiChannel;  // 0=DSP1, 1=DSP2, 2=DSP3, 3=DSP4
        float minValue;
        float maxValue;
        float defaultValue;
        float currentValue;
        juce::String unit;
        std::function<float(float)> toSamValue;  // Conversion to SAM units
        std::function<float(float)> fromSamValue; // Conversion from SAM units
    };
    
    ParameterManager();
    
    // Parameter access
    float getParameterValue(const juce::String& id) const;
    void setParameterValue(const juce::String& id, float value, bool sendMidi = true);
    
    // Parameter info
    const Parameter* getParameter(const juce::String& id) const;
    juce::Array<juce::String> getAllParameterIds() const;
    
    // Preset management
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);
    void factoryReset();
    
    // Undo/redo
    bool canUndo() const;
    bool canRedo() const;
    void undo();
    void redo();
    
    // Parameter groups
    juce::Array<juce::String> getGainParameters() const;
    juce::Array<juce::String> getEqParameters(int dspIndex) const;
    juce::Array<juce::String> getCompressorParameters() const;
    juce::Array<juce::String> getDelayParameters(int dspIndex) const;
    
    // Callbacks
    std::function<void(const juce::String& paramId, float newValue)> onParameterChanged;
    std::function<void()> onPresetLoaded;
    
    // Conversion functions (static for external use)
    static float dbToSamGain(float db);
    static float samGainToDb(float samValue);
    static float hzToSamFreq(float hz);
    static float samFreqToHz(float samValue);
    static float qToSamQ(float q);
    static float samQToQ(float samValue);
    static float msToSamTime(float ms);
    static float samTimeToMs(float samValue);
    static float ratioToSamRatio(float ratio);
    static float samRatioToRatio(float samValue);
    
private:
    juce::HashMap<juce::String, Parameter> parameters;
    juce::Array<juce::String> parameterHistory;
    int historyIndex = -1;
    
    struct HistoryEntry {
        juce::String paramId;
        float oldValue;
        float newValue;
        juce::Time timestamp;
    };
    juce::Array<HistoryEntry> undoStack;
    juce::Array<HistoryEntry> redoStack;
    
    void initializeParameters();
    void addParameter(const Parameter& param);
    void addToHistory(const juce::String& paramId, float oldValue, float newValue);
    
    // Parameter definition helpers
    void addGainParameters();
    void addEqParameters();
    void addCompressorParameters();
    void addDelayParameters();
    
    // JSON serialization
    juce::var parametersToJson() const;
    bool jsonToParameters(const juce::var& json);
};