#ifndef CRSF_SIMULATION_DATA_GENERATOR
#define CRSF_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

class CRSFAnalyzerSettings;

class CRSFSimulationDataGenerator
{
public:
    CRSFSimulationDataGenerator();
    ~CRSFSimulationDataGenerator();

    void Initialize(U32 simulation_sample_rate, CRSFAnalyzerSettings *settings);
    U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels);

protected:
    CRSFAnalyzerSettings *mSettings;
    U32 mSimulationSampleRateHz;

    U64 mMpModeAddressMask;
    U64 mMpModeDataMask;

protected: // CRSF specific
    void CreateSerialByte(U64 value);
    ClockGenerator mClockGenerator;
    /**
     * If we had more than one channel to simulate, they would need to be in an array
     */
    SimulationChannelDescriptor mCRSFSimulationData;
};

#endif // CRSF_SIMULATION_DATA_GENERATOR
