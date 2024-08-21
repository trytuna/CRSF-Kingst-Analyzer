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

void CRSFAnalyzer::ComputeSampleOffsets()
{
    ClockGenerator clock_generator;
    clock_generator.Init(mSettings->baudRate, mSampleRateHz);

    mSampleOffsets.clear();

    U32 num_bits = mSettings->baudRate;

    mSampleOffsets.push_back(clock_generator.AdvanceByHalfPeriod(1.5));  //point to the center of the 1st bit (past the start bit)
    num_bits--;  //we just added the first bit.

    for (U32 i = 0; i < num_bits; i++) {
        mSampleOffsets.push_back(clock_generator.AdvanceByHalfPeriod());
    }

    //to check for framing errors, we also want to check
    //1/2 bit after the beginning of the stop bit
    mStartOfStopBitOffset = clock_generator.AdvanceByHalfPeriod(1.0);   //i.e. moving from the center of the last data bit (where we left off) to 1/2 period into the stop bit

    //and 1/2 bit before end of the stop bit period
    // mEndOfStopBitOffset = clock_generator.AdvanceByHalfPeriod(mSettings->mStopBits - 1.0);  //if stopbits == 1.0, this will be 0
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
    ComputeSampleOffsets();
    U32 num_bits = 8;

    U64 bit_mask = 0;
    U64 mask = 0x1ULL;
    for (U32 i = 0; i < num_bits; i++) {
        bit_mask |= mask;
        mask <<= 1;
    }

    crsf = GetAnalyzerChannelData(mSettings->channel);
    crsf->TrackMinimumPulseWidth();
    if (crsf->GetBitState() == mBitLow) {
        crsf->AdvanceToNextEdge();
    }

    for (; ;) {
        // we're starting high.  (we'll assume that we're not in the middle of a byte.)

        crsf->AdvanceToNextEdge();

        // we're now at the beginning of the start bit.  We can start collecting the data.
        U64 frame_starting_sample = crsf->GetSampleNumber();

        U64 data = 0;

        DataBuilder data_builder;
        data_builder.Reset(&data, AnalyzerEnums::LsbFirst, num_bits);
        U64 marker_location = frame_starting_sample;

        for (U32 i = 0; i < num_bits; i++) {
            crsf->Advance(mSampleOffsets[i]);
            data_builder.AddBit(crsf->GetBitState());

            marker_location += mSampleOffsets[i];
            mResults->AddMarker(marker_location, AnalyzerResults::Dot, mSettings->channel);
        }

        crsf->Advance(mStartOfStopBitOffset);

        if (crsf->GetBitState() != mBitHigh) {
        } else {
            U32 num_edges = crsf->Advance(mEndOfStopBitOffset);
            if (num_edges != 0) {
            }
        }

        // ok now record the value!
        // note that we're not using the mData2 or mType fields for anything, so we won't bother to set them.
        Frame frame;
        frame.mStartingSampleInclusive = frame_starting_sample;
        frame.mEndingSampleInclusive = crsf->GetSampleNumber();
        frame.mData1 = data;
        frame.mFlags = 0;

        mResults->AddFrame(frame);

        mResults->CommitResults();

        ReportProgress(frame.mEndingSampleInclusive);
        CheckIfThreadShouldExit();
    }
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
