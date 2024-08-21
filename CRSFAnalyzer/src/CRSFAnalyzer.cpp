#include "CRSFAnalyzer.h"
#include "CRSFAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

CRSFAnalyzer::CRSFAnalyzer()
    :   Analyzer(),
        mSettings(new CRSFAnalyzerSettings()),
        mSimulationInitilized(false)
{
    // This disables the stdout buffer. This can be useful if you want to printf something for debugging purposes.
    setbuf(stdout, NULL);
    
    SetAnalyzerSettings(mSettings.get());
}

CRSFAnalyzer::~CRSFAnalyzer()
{
    KillThread();
}

void CRSFAnalyzer::SetupResults()
{
    mResults.reset(new CRSFAnalyzerResults(this, mSettings.get()));
    SetAnalyzerResults(mResults.get());
    mResults->AddChannelBubblesWillAppearOn(mSettings->channel);
}

// Decoding happens here
void CRSFAnalyzer::WorkerThread()
{
    mSampleRateHz = GetSampleRate();
    crsf = GetAnalyzerChannelData(mSettings->channel);

    // U64 GetSampleNumber();

    // determines if signal is high or low
    // BitState GetBitState();
}

bool CRSFAnalyzer::NeedsRerun()
{
    return false;
}

U32 CRSFAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor **simulation_channels)
{
    if (mSimulationInitilized == false) {
        mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 CRSFAnalyzer::GetMinimumSampleRateHz()
{
    return mSettings->baudRate * 4;
}

const char *CRSFAnalyzer::GetAnalyzerName() const
{
    return "CRSF";
}

const char *GetAnalyzerName()
{
    return "CRSF";
}

Analyzer *CreateAnalyzer()
{
    return new CRSFAnalyzer();
}

void DestroyAnalyzer(Analyzer *analyzer)
{
    delete analyzer;
}
