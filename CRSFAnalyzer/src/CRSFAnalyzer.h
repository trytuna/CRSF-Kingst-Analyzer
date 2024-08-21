#ifndef CRSF_ANALYZER_H
#define CRSF_ANALYZER_H

#include <Analyzer.h>
#include "CRSFAnalyzerResults.h"
#include "CRSFSimulationDataGenerator.h"

class CRSFAnalyzerSettings;

class ANALYZER_EXPORT CRSFAnalyzer : public Analyzer
{
public:
    CRSFAnalyzer();
    virtual ~CRSFAnalyzer();
    
    virtual void WorkerThread();
    virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels);
    virtual U32 GetMinimumSampleRateHz();
    virtual const char *GetAnalyzerName() const;
    virtual bool NeedsRerun();

    virtual void SetupResults();

#pragma warning( push )
#pragma warning( disable : 4251 ) //warning C4251: 'CRSFAnalyzer::<...>' : class <...> needs to have dll-interface to be used by clients of class

protected: //functions
    void ComputeSampleOffsets();

protected: //vars
    std::auto_ptr< CRSFAnalyzerSettings > mSettings;
    std::auto_ptr< CRSFAnalyzerResults > mResults;

    CRSFSimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitilized;

    AnalyzerChannelData *crsf;

    //Serial analysis vars:
    U32 mSampleRateHz;
    std::vector<U32> mSampleOffsets;
    U32 mParityBitOffset;
    U32 mStartOfStopBitOffset;
    U32 mEndOfStopBitOffset;
    BitState mBitLow;
    BitState mBitHigh;

#pragma warning( pop )
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //CRSF_ANALYZER_H
