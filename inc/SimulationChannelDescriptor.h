#ifndef SIMULATION_CHANNEL_DESCRIPTOR
#define SIMULATION_CHANNEL_DESCRIPTOR

#include "LogicPublicTypes.h"

//PIMPL implementation can be changed, non-virtual functions can be added at the end, no vars can be added/removed/reordered/changed.
struct SimulationChannelDescriptorData;
class LOGICAPI SimulationChannelDescriptor
{
public:
    /**
     * This toggles the channel. BIT_LOW becomes BIT_HIGH, BIT_HIGH becomes BIT_LOW. The current 
     * Sample Number will become the new BitState (BIT_LOW or BIT_HIGH), and all samples after that will 
     * also be the new BitState, untill we toggle again.
     */
    void Transition();

    /**
     * Often we don’t want to keep track of the current BitState, which toggles every time we call Transition. 
     * TransitionIfNeeded checks the current BitState, and only transitions if the current BitState doesn’t 
     * match the one we provide. In other words “Change to this bit_state, if we’re not already”.
     */
    void TransitionIfNeeded(BitState bit_state);
    /**
     * We can move forward in the stream by a specific number of samples. This function will return how
many times the input toggled (changed from a high to a low, or low to a high) to make this move.
     */
    void Advance(U32 num_samples_to_advance);

    /**
     * This function lets you directly ask what the current BitState is.
     */
    BitState GetCurrentBitState();

    /**
     * This function lets you ask what the current SampleNumber is.
     */
    U64 GetCurrentSampleNumber();

public:  //don't use
    SimulationChannelDescriptor();
    SimulationChannelDescriptor(const SimulationChannelDescriptor &other);
    ~SimulationChannelDescriptor();
    SimulationChannelDescriptor &operator=(const SimulationChannelDescriptor &other);

    void SetChannel(Channel &channel);
    void SetSampleRate(U32 sample_rate_hz);
    void SetInitialBitState(BitState intial_bit_state);

    Channel GetChannel();
    U32 GetSampleRate();
    BitState GetInitialBitState();
    void *GetData();

protected:
    struct SimulationChannelDescriptorData *mData;
};


struct SimulationChannelDescriptorGroupData;
class LOGICAPI SimulationChannelDescriptorGroup
{
public:
    SimulationChannelDescriptorGroup();
    ~SimulationChannelDescriptorGroup();

    SimulationChannelDescriptor *Add(Channel &channel, U32 sample_rate, BitState intial_bit_state);   //do not delete this pointer

    void AdvanceAll(U32 num_samples_to_advance);

public:
    SimulationChannelDescriptor *GetArray();
    U32 GetCount();

protected:
    struct SimulationChannelDescriptorGroupData *mData;
};

#endif //SIMULATION_CHANNEL_DESCRIPTOR