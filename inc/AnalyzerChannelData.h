#ifndef ANALYZERCHANNELDATA
#define ANALYZERCHANNELDATA

#include "LogicPublicTypes.h"

struct AnalyzerChannelDataData;
class ChannelData;

class LOGICAPI AnalyzerChannelData
{
public:
    AnalyzerChannelData(ChannelData *channel_data);
    ~AnalyzerChannelData();

    //State
    U64 GetSampleNumber();
    BitState GetBitState();

    //Basic:
    /**
     * Move forward the specified number of samples.
     * Returns the number of times the bit changed state during the move.
     */
    U32 Advance(U32 num_samples);

    /**
     * Move forward to the specified sample number.
     * Returns the number of times the bit changed state during the move.
     */
    U32 AdvanceToAbsPosition(U64 sample_number);

    /**
     * Move forward until the bit state changes from what it is now.
     * 
     * After calling this function you might want to call GetSampleNumber() to find out how far you’ve come.
     */
    void AdvanceToNextEdge();

    // Fancier
    /**
     * Without moving, get the sample of the next transition.
     */
    U64 GetSampleOfNextEdge();

    /**
     * If we advanced, would we encounter any transitions?
     */
    bool WouldAdvancingCauseTransition(U32 num_samples);

    /**
     * If we advanced to a absolute position, would we encounter any transitions?
     */
    bool WouldAdvancingToAbsPositionCauseTransition(U64 sample_number);

    /**
     * Minimum pulse tracking. The serial analyzer uses this for auto-baud.
     * 
     * Normally this is not enabled.
     */
    void TrackMinimumPulseWidth();
    U64 GetMinimumPulseWidthSoFar();

    // Fancier, part II
    /**
     * Use this when you have a situation where you have multiple lines, and you need to handle the case where one or the other of them may never change again, and you don't know which.
     */
    bool DoMoreTransitionsExistInCurrentData();

protected:
    AnalyzerChannelDataData *mData;
};

#endif //ANALYZERCHANNELDATA