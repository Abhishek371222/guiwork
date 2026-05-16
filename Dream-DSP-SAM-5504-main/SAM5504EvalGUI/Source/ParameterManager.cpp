#include "ParameterManager.h"

ParameterManager::ParameterManager()
{
    initializeParameters();
    factoryReset(); // Set all parameters to default values
}

// ============================================================================
// Parameter access
// ============================================================================

float ParameterManager::getParameterValue(const juce::String& id) const
{
    if (const Parameter* param = parameters[id])
        return param->currentValue;
    
    return 0.0f;
}

void ParameterManager::setParameterValue(const juce::String& id, float value, bool sendMidi)
{
    Parameter* param = parameters[id];
    if (!param)
        return;
    
    // Clamp value to valid range
    float clampedValue = juce::jlimit(param->minValue, param->maxValue, value);
    
    // Only update if value changed
    if (std::abs(param->currentValue - clampedValue) > 0.001f)
    {
        float oldValue = param->currentValue;
        param->currentValue = clampedValue;
        
        // Add to undo history
        addToHistory(id, oldValue, clampedValue);
        
        // Notify listeners
        if (onParameterChanged)
            onParameterChanged(id, clampedValue);
    }
}

const ParameterManager::Parameter* ParameterManager::getParameter(const juce::String& id) const
{
    return parameters[id];
}

juce::Array<juce::String> ParameterManager::getAllParameterIds() const
{
    juce::Array<juce::String> ids;
    for (auto& entry : parameters)
        ids.add(entry.getKey());
    
    ids.sortNatural();
    return ids;
}

// ============================================================================
// Preset management
// ============================================================================

bool ParameterManager::savePreset(const juce::File& file)
{
    juce::var presetJson;
    presetJson["version"] = "1.0";
    presetJson["name"] = file.getFileNameWithoutExtension();
    presetJson["timestamp"] = juce::Time::getCurrentTime().toISO8601(true);
    presetJson["parameters"] = parametersToJson();
    
    juce::FileOutputStream stream(file);
    if (stream.openedOk())
    {
        juce::JSON::writeToStream(stream, presetJson);
        return true;
    }
    
    return false;
}

bool ParameterManager::loadPreset(const juce::File& file)
{
    juce::var presetJson = juce::JSON::parse(file);
    if (presetJson.isObject())
    {
        if (presetJson["version"].toString() == "1.0")
        {
            if (jsonToParameters(presetJson["parameters"]))
            {
                if (onPresetLoaded)
                    onPresetLoaded();
                return true;
            }
        }
    }
    
    return false;
}

void ParameterManager::factoryReset()
{
    for (auto& entry : parameters)
    {
        Parameter& param = entry.getValue();
        param.currentValue = param.defaultValue;
    }
    
    undoStack.clear();
    redoStack.clear();
    historyIndex = -1;
}

// ============================================================================
// Undo/redo
// ============================================================================

bool ParameterManager::canUndo() const
{
    return !undoStack.isEmpty();
}

bool ParameterManager::canRedo() const
{
    return !redoStack.isEmpty();
}

void ParameterManager::undo()
{
    if (undoStack.isEmpty())
        return;
    
    HistoryEntry entry = undoStack.removeAndReturn(undoStack.size() - 1);
    Parameter* param = parameters[entry.paramId];
    if (param)
    {
        float currentValue = param->currentValue;
        param->currentValue = entry.oldValue;
        redoStack.add({entry.paramId, currentValue, entry.oldValue, juce::Time::getCurrentTime()});
        
        if (onParameterChanged)
            onParameterChanged(entry.paramId, entry.oldValue);
    }
}

void ParameterManager::redo()
{
    if (redoStack.isEmpty())
        return;
    
    HistoryEntry entry = redoStack.removeAndReturn(redoStack.size() - 1);
    Parameter* param = parameters[entry.paramId];
    if (param)
    {
        float currentValue = param->currentValue;
        param->currentValue = entry.newValue;
        undoStack.add({entry.paramId, currentValue, entry.newValue, juce::Time::getCurrentTime()});
        
        if (onParameterChanged)
            onParameterChanged(entry.paramId, entry.newValue);
    }
}

// ============================================================================
// Parameter groups
// ============================================================================

juce::Array<juce::String> ParameterManager::getGainParameters() const
{
    juce::Array<juce::String> ids;
    
    // DSP1 and DSP3 gain parameters
    ids.add("dsp1_gain_value");
    ids.add("dsp1_gain_phase");
    ids.add("dsp3_gain_value");
    ids.add("dsp3_gain_phase");
    
    return ids;
}

juce::Array<juce::String> ParameterManager::getEqParameters(int dspIndex) const
{
    juce::Array<juce::String> ids;
    juce::String prefix;
    
    switch (dspIndex)
    {
        case 1: prefix = "dsp1_eq"; break;
        case 2: prefix = "dsp2_eq"; break;
        case 3: prefix = "dsp3_eq"; break;
        case 4: prefix = "dsp4_eq"; break;
        default: return ids;
    }
    
    // Get all EQ parameters for this DSP
    for (auto& entry : parameters)
    {
        if (entry.getKey().startsWith(prefix))
            ids.add(entry.getKey());
    }
    
    ids.sortNatural();
    return ids;
}

juce::Array<juce::String> ParameterManager::getCompressorParameters() const
{
    juce::Array<juce::String> ids;
    
    // DSP1 and DSP3 compressor parameters
    ids.add("dsp1_comp_enable");
    ids.add("dsp1_comp_threshold");
    ids.add("dsp1_comp_ratio");
    ids.add("dsp1_comp_attack");
    ids.add("dsp1_comp_release");
    ids.add("dsp1_comp_makeup");
    ids.add("dsp3_comp_enable");
    ids.add("dsp3_comp_threshold");
    ids.add("dsp3_comp_ratio");
    ids.add("dsp3_comp_attack");
    ids.add("dsp3_comp_release");
    ids.add("dsp3_comp_makeup");
    
    return ids;
}

juce::Array<juce::String> ParameterManager::getDelayParameters(int dspIndex) const
{
    juce::Array<juce::String> ids;
    juce::String prefix;
    
    switch (dspIndex)
    {
        case 2: prefix = "dsp2_delay"; break;
        case 4: prefix = "dsp4_delay"; break;
        default: return ids;
    }
    
    // Get all delay parameters for this DSP
    for (auto& entry : parameters)
    {
        if (entry.getKey().startsWith(prefix))
            ids.add(entry.getKey());
    }
    
    ids.sortNatural();
    return ids;
}

// ============================================================================
// Conversion functions
// ============================================================================

float ParameterManager::dbToSamGain(float db)
{
    // 0x4000 = 0 dB, 512 units = 1 dB
    constexpr float zeroDb = 0x4000;
    constexpr float unitsPerDb = 512.0f;
    constexpr float minValue = 0x1000;  // -24 dB
    constexpr float maxValue = 0x5e00;  // +15 dB
    
    float samValue = zeroDb + db * unitsPerDb;
    return juce::jlimit(minValue, maxValue, samValue);
}

float ParameterManager::samGainToDb(float samValue)
{
    constexpr float zeroDb = 0x4000;
    constexpr float unitsPerDb = 512.0f;
    
    return (samValue - zeroDb) / unitsPerDb;
}

float ParameterManager::hzToSamFreq(float hz)
{
    // Logarithmic mapping: 20Hz-20kHz to 0x0000-0x7FFF
    constexpr float minHz = 20.0f;
    constexpr float maxHz = 20000.0f;
    
    // Convert to logarithmic scale
    float logMin = std::log10(minHz);
    float logMax = std::log10(maxHz);
    float logValue = std::log10(juce::jlimit(minHz, maxHz, hz));
    
    // Map to 0-1 range
    float normalized = (logValue - logMin) / (logMax - logMin);
    
    // Convert to 14-bit SAM value
    return normalized * 0x7FFF;
}

float ParameterManager::samFreqToHz(float samValue)
{
    constexpr float minHz = 20.0f;
    constexpr float maxHz = 20000.0f;
    
    float normalized = samValue / 0x7FFF;
    float logMin = std::log10(minHz);
    float logMax = std::log10(maxHz);
    float logValue = logMin + normalized * (logMax - logMin);
    
    return std::pow(10.0f, logValue);
}

float ParameterManager::qToSamQ(float q)
{
    // Q range: 0.1 to 10.0, map linearly to 0x0000-0x7FFF
    constexpr float minQ = 0.1f;
    constexpr float maxQ = 10.0f;
    
    float normalized = (juce::jlimit(minQ, maxQ, q) - minQ) / (maxQ - minQ);
    return normalized * 0x7FFF;
}

float ParameterManager::samQToQ(float samValue)
{
    constexpr float minQ = 0.1f;
    constexpr float maxQ = 10.0f;
    
    float normalized = samValue / 0x7FFF;
    return minQ + normalized * (maxQ - minQ);
}

float ParameterManager::msToSamTime(float ms)
{
    // 0-5.33ms maps to 0x0000-0x7FFF (512 samples @ 96kHz = 5.33ms)
    constexpr float maxMs = 5.33f;
    
    float normalized = juce::jlimit(0.0f, maxMs, ms) / maxMs;
    return normalized * 0x7FFF;
}

float ParameterManager::samTimeToMs(float samValue)
{
    constexpr float maxMs = 5.33f;
    
    float normalized = samValue / 0x7FFF;
    return normalized * maxMs;
}

float ParameterManager::ratioToSamRatio(float ratio)
{
    // Ratio range: 1.0 to 20.0, map linearly to 0x0000-0x7FFF
    constexpr float minRatio = 1.0f;
    constexpr float maxRatio = 20.0f;
    
    float normalized = (juce::jlimit(minRatio, maxRatio, ratio) - minRatio) / (maxRatio - minRatio);
    return normalized * 0x7FFF;
}

float ParameterManager::samRatioToRatio(float samValue)
{
    constexpr float minRatio = 1.0f;
    constexpr float maxRatio = 20.0f;
    
    float normalized = samValue / 0x7FFF;
    return minRatio + normalized * (maxRatio - minRatio);
}

// ============================================================================
// Private methods
// ============================================================================

void ParameterManager::initializeParameters()
{
    addGainParameters();
    addEqParameters();
    addCompressorParameters();
    addDelayParameters();
}

void ParameterManager::addParameter(const Parameter& param)
{
    parameters.set(param.id, param);
}

void ParameterManager::addToHistory(const juce::String& paramId, float oldValue, float newValue)
{
    undoStack.add({paramId, oldValue, newValue, juce::Time::getCurrentTime()});
    redoStack.clear(); // Clear redo stack when new action is performed
}

void ParameterManager::addGainParameters()
{
    // DSP1 Gain (Left Input Chain)
    addParameter({
        "dsp1_gain_value",
        "Left Gain",
        "Input gain for left channel",
        0x0100, 0,  // NRPN 0x0100, MIDI channel 0
        -24.0f, 15.0f, 0.0f, 0.0f, "dB",
        dbToSamGain,
        samGainToDb
    });
    
    addParameter({
        "dsp1_gain_phase",
        "Left Gain Phase",
        "Phase adjustment for left input gain",
        0x0101, 0,
        0.0f, 360.0f, 0.0f, 0.0f, "°",
        [](float deg) { return static_cast<int>(deg * 182.04f) & 0x7FFF; },
        [](float sam) { return sam / 182.04f; }
    });
    
    // DSP3 Gain (Right Input Chain) - identical to DSP1 but channel 2
    addParameter({
        "dsp3_gain_value",
        "Right Gain",
        "Input gain for right channel",
        0x0100, 2,  // NRPN 0x0100, MIDI channel 2
        -24.0f, 15.0f, 0.0f, 0.0f, "dB",
        dbToSamGain,
        samGainToDb
    });
    
    addParameter({
        "dsp3_gain_phase",
        "Right Gain Phase",
        "Phase adjustment for right input gain",
        0x0101, 2,
        0.0f, 360.0f, 0.0f, 0.0f, "°",
        [](float deg) { return static_cast<int>(deg * 182.04f) & 0x7FFF; },
        [](float sam) { return sam / 182.04f; }
    });
}

void ParameterManager::addEqParameters()
{
    // Helper function to add EQ band parameters
    auto addEqBand = [this](const juce::String& prefix, const juce::String& name, 
                           int nrpnBase, int midiChannel, int bandIndex) {
        // Enable/disable
        addParameter({
            prefix + "_enable",
            name + " Enable",
            "Enable/disable this EQ band",
            nrpnBase + 0x0000, midiChannel,
            0.0f, 1.0f, 1.0f, 1.0f, "",
            [](float enabled) { return enabled > 0.5f ? 0x7FFF : 0x0000; },
            [](float sam) { return sam > 0x3FFF ? 1.0f : 0.0f; }
        });
        
        // Frequency
        addParameter({
            prefix + "_freq",
            name + " Frequency",
            "Center frequency for this EQ band",
            nrpnBase + 0x0041 + (bandIndex * 0x001F), midiChannel,
            20.0f, 20000.0f, bandIndex == 0 ? 1000.0f : (bandIndex == 1 ? 3000.0f : 8000.0f),
            bandIndex == 0 ? 1000.0f : (bandIndex == 1 ? 3000.0f : 8000.0f), "Hz",
            hzToSamFreq,
            samFreqToHz
        });
        
        // Gain
        addParameter({
            prefix + "_gain",
            name + " Gain",
            "Gain adjustment for this EQ band",
            nrpnBase + 0x0060 + (bandIndex * 0x001F), midiChannel,
            -12.0f, 12.0f, 0.0f, 0.0f, "dB",
            dbToSamGain,
            samGainToDb
        });
        
        // Q factor
        addParameter({
            prefix + "_q",
            name + " Q",
            "Q factor (resonance) for this EQ band",
            nrpnBase + 0x0022 + (bandIndex * 0x001F), midiChannel,
            0.1f, 10.0f, 1.0f, 1.0f, "",
            qToSamQ,
            samQToQ
        });
        
        // Filter type (simplified - just low/high/band)
        addParameter({
            prefix + "_type",
            name + " Type",
            "Filter type for this EQ band",
            nrpnBase + 0x0003 + (bandIndex * 0x001F), midiChannel,
            0.0f, 2.0f, 1.0f, 1.0f, "",
            [](float type) { return static_cast<int>(type * 0x3FFF) & 0x7FFF; },
            [](float sam) { return sam / 0x3FFF; }
        });
    };
    
    // DSP1 EQ (3 bands, Left Input)
    for (int band = 0; band < 3; ++band)
    {
        addEqBand("dsp1_eq" + juce::String(band),
                 "Left EQ Band " + juce::String(band + 1),
                 0x0200, 0, band);
    }
    
    // DSP2 EQ (2 bands per channel, Left Output)
    for (int band = 0; band < 2; ++band)
    {
        // Channel A (Process 2)
        addEqBand("dsp2_eqA" + juce::String(band),
                 "Left Output A EQ Band " + juce::String(band + 1),
                 0x0200, 1, band);
        
        // Channel B (Process 4)
        addEqBand("dsp2_eqB" + juce::String(band),
                 "Left Output B EQ Band " + juce::String(band + 1),
                 0x0400, 1, band);
    }
    
    // DSP3 EQ (3 bands, Right Input) - identical to DSP1 but channel 2
    for (int band = 0; band < 3; ++band)
    {
        addEqBand("dsp3_eq" + juce::String(band),
                 "Right EQ Band " + juce::String(band + 1),
                 0x0200, 2, band);
    }
    
    // DSP4 EQ (2 bands per channel, Right Output)
    for (int band = 0; band < 2; ++band)
    {
        // Channel A (Process 2)
        addEqBand("dsp4_eqA" + juce::String(band),
                 "Right Output A EQ Band " + juce::String(band + 1),
                 0x0200, 3, band);
        
        // Channel B (Process 4)
        addEqBand("dsp4_eqB" + juce::String(band),
                 "Right Output B EQ Band " + juce::String(band + 1),
                 0x0400, 3, band);
    }
}

void ParameterManager::addCompressorParameters()
{
    // Helper function to add compressor parameters
    auto addCompressor = [this](const juce::String& prefix, const juce::String& name, int midiChannel) {
        // Enable/disable
        addParameter({
            prefix + "_enable",
            name + " Enable",
            "Enable/disable compressor",
            0x0501, midiChannel,
            0.0f, 1.0f, 1.0f, 1.0f, "",
            [](float enabled) { return enabled > 0.5f ? 0x7FFF : 0x0000; },
            [](float sam) { return sam > 0x3FFF ? 1.0f : 0.0f; }
        });
        
        // Threshold
        addParameter({
            prefix + "_threshold",
            name + " Threshold",
            "Compression threshold level",
            0x0502, midiChannel,
            -60.0f, 0.0f, -20.0f, -20.0f, "dB",
            dbToSamGain,
            samGainToDb
        });
        
        // Ratio
        addParameter({
            prefix + "_ratio",
            name + " Ratio",
            "Compression ratio",
            0x0503, midiChannel,
            1.0f, 20.0f, 4.0f, 4.0f, ":1",
            ratioToSamRatio,
            samRatioToRatio
        });
        
        // Attack (LevelDetect Attack)
        addParameter({
            prefix + "_attack",
            name + " Attack",
            "Compressor attack time",
            0x0400, midiChannel,
            0.1f, 100.0f, 10.0f, 10.0f, "ms",
            [](float ms) { return static_cast<int>(ms * 655.35f) & 0x7FFF; },
            [](float sam) { return sam / 655.35f; }
        });
        
        // Release (LevelDetect Release)
        addParameter({
            prefix + "_release",
            name + " Release",
            "Compressor release time",
            0x0401, midiChannel,
            10.0f, 1000.0f, 100.0f, 100.0f, "ms",
            [](float ms) { return static_cast<int>(ms * 65.535f) & 0x7FFF; },
            [](float sam) { return sam / 65.535f; }
        });
        
        // Makeup gain
        addParameter({
            prefix + "_makeup",
            name + " Makeup Gain",
            "Output gain after compression",
            0x0504, midiChannel,
            -12.0f, 12.0f, 0.0f, 0.0f, "dB",
            dbToSamGain,
            samGainToDb
        });
    };
    
    // DSP1 Compressor (Left Input)
    addCompressor("dsp1_comp", "Left Compressor", 0);
    
    // DSP3 Compressor (Right Input)
    addCompressor("dsp3_comp", "Right Compressor", 2);
}

void ParameterManager::addDelayParameters()
{
    // Helper function to add delay parameters
    auto addDelayChannel = [this](const juce::String& prefix, const juce::String& name, 
                                 int nrpnBase, int midiChannel) {
        // Enable/disable
        addParameter({
            prefix + "_enable",
            name + " Enable",
            "Enable/disable delay",
            nrpnBase + 0x0000, midiChannel,
            0.0f, 1.0f, 1.0f, 1.0f, "",
            [](float enabled) { return enabled > 0.5f ? 0x7FFF : 0x0000; },
            [](float sam) { return sam > 0x3FFF ? 1.0f : 0.0f; }
        });
        
        // Time
        addParameter({
            prefix + "_time",
            name + " Time",
            "Delay time",
            nrpnBase + 0x0001, midiChannel,
            0.0f, 5.33f, 0.0f, 0.0f, "ms",
            msToSamTime,
            samTimeToMs
        });
        
        // Output gain
        addParameter({
            prefix + "_gain",
            name + " Output Gain",
            "Delay output gain",
            nrpnBase + 0x0002, midiChannel,
            -24.0f, 15.0f, 0.0f, 0.0f, "dB",
            dbToSamGain,
            samGainToDb
        });
        
        // Output phase
        addParameter({
            prefix + "_phase",
            name + " Output Phase",
            "Delay output phase adjustment",
            nrpnBase + 0x0003, midiChannel,
            0.0f, 360.0f, 0.0f, 0.0f, "°",
            [](float deg) { return static_cast<int>(deg * 182.04f) & 0x7FFF; },
            [](float sam) { return sam / 182.04f; }
        });
    };
    
    // DSP2 Delay (Left Output)
    addDelayChannel("dsp2_delayA", "Left Delay A", 0x0100, 1); // Process 1
    addDelayChannel("dsp2_delayB", "Left Delay B", 0x0300, 1); // Process 3
    
    // DSP4 Delay (Right Output)
    addDelayChannel("dsp4_delayA", "Right Delay A", 0x0100, 3); // Process 1
    addDelayChannel("dsp4_delayB", "Right Delay B", 0x0300, 3); // Process 3
}

juce::var ParameterManager::parametersToJson() const
{
    juce::DynamicObject::Ptr obj = new juce::DynamicObject();
    
    for (auto& entry : parameters)
    {
        obj->setProperty(entry.getKey(), entry.getValue().currentValue);
    }
    
    return juce::var(obj.get());
}

bool ParameterManager::jsonToParameters(const juce::var& json)
{
    if (!json.isObject())
        return false;
    
    juce::DynamicObject* obj = json.getDynamicObject();
    if (!obj)
        return false;
    
    juce::NamedValueSet& properties = obj->getProperties();
    
    for (auto& property : properties)
    {
        juce::String paramId = property.name.toString();
        float value = static_cast<float>(property.value);
        
        Parameter* param = parameters[paramId];
        if (param)
        {
            param->currentValue = juce::jlimit(param->minValue, param->maxValue, value);
        }
    }
    
    return true;
}