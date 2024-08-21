#include "CRSFSimulationDataGenerator.h"
#include "CRSFAnalyzerSettings.h"

CRSFSimulationDataGenerator::CRSFSimulationDataGenerator()
{}

CRSFSimulationDataGenerator::~CRSFSimulationDataGenerator()
{}

void CRSFSimulationDataGenerator::Initialize(U32 simulation_sample_rate, CRSFAnalyzerSettings *settings)
{
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    mClockGenerator.Init(settings->baudRate, simulation_sample_rate);
    mCRSFSimulationData.SetChannel(mSettings->channel);
    mCRSFSimulationData.SetSampleRate(simulation_sample_rate);

    mCRSFSimulationData.SetInitialBitState(BIT_HIGH);
    mCRSFSimulationData.Advance(mClockGenerator.AdvanceByHalfPeriod(10.0)); // insert 10 bit-periods of idle
}

U32 CRSFSimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels)
{
    U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested, sample_rate, mSimulationSampleRateHz);

    /*
        0xEE 0x18 0x16 0xE0 0xBB 0x5E 0x2B 0xC0 0x07 0x3E 0xF0 0x81 0x0F 0x7C 0xE0 0x03 0x1F 0xF8 0xC0 0x07 0x3E 0xF0 0x81 0x0F 0x7C 0x14
          |    |    |  |  Payload => 16 Channel with 11bits each (16 x 11 = 176 => 22 Bytes)                                       |   |
          |    |    |  |  These 11 bits values representing the PWM signal length. Decimal values from 0 - 1984 are possible       |   |
          |    |    |  |                                                                                                           |   |
          |    |    |  |  hex   | decimal | signal length                                                                          |   |
          |    |    |  |  ------|---------|--------------                                                                          |   |
          |    |    |  |  0x0AC | 172     | 988us                                                                                  |   |
          |    |    |  |  0x3E0 | 922     | 1500us                                                                                 |   |
          |    |    |  |  0x713 | 1811    | 2012us                                                                                 |   |
          |    |    |   \_________________________________________________________________________________________________________/     \__ CRC8 (with Polynomial 0xD5; bytes from <type> to the end of <payload> will be calculated)
          |    |    |
          |    |     \__ Type 0x16 => CRSF_FRAMETYPE_RC_CHANNELS_PACKED
          |    |
          |     \__ Length 0x18 => decimal 24 => 24 bytes length
          |
           \__ Synchonization 

        Channel values will decode to this:

            channel_0 => 992 channel_1 => 983 channel_2 => 173 channel_3 => 992
            channel_4 => 992 channel_5 => 992 channel_6 => 992 channel_7 => 992
            channel_8 => 992 channel_9 => 992 channel_10 => 992 channel_11 => 992
            channel_12 => 992 channel_13 => 992 channel_14 => 992 channel_15 => 992
        
    */
    uint8_t frame_rc_channels_packed[] = {
        0xEE, 0x18, 0x16, // [sync] [length] [type]
        0xE0, 0xBB, 0x5E, 0x2B, 0xC0, 0x07, 0x3E, 0xF0, 0x81, 0x0F, 0x7C, 0xE0, 0x03, 0x1F, 0xF8, 0xC0, 0x07, 0x3E, 0xF0, 0x81, 0x0F, 0x7C, // [payload (22bytes)]
        0x14 // [crc8]
    };

    unsigned packed_size = sizeof(frame_rc_channels_packed) / sizeof(frame_rc_channels_packed[0]);

    while (mCRSFSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested) {
        for(const uint8_t &word : frame_rc_channels_packed) {
            CreateSerialByte(word);
            mCRSFSimulationData.Advance(mClockGenerator.AdvanceByHalfPeriod(10.0)); // insert 10 bit-periods of idle
        }
        // let time pass between each data frame
        mCRSFSimulationData.Advance(mClockGenerator.AdvanceByHalfPeriod(1000.0));
    }

    *simulation_channels = &mCRSFSimulationData;
    return 1; // we are retuning the size of the SimulationChannelDescriptor array. In our case, the "array" is length 1.
}

void CRSFSimulationDataGenerator::CreateSerialByte(U64 value)
{
    // assume we start high
    mCRSFSimulationData.Transition(); // low-going edge for start bit
    mCRSFSimulationData.Advance(mClockGenerator.AdvanceByHalfPeriod()); // add start bit time

    U32 num_bits = 8;

    BitExtractor bit_extractor(value, AnalyzerEnums::LsbFirst, num_bits);

    for (U32 i = 0; i < num_bits; i++) {
        mCRSFSimulationData.TransitionIfNeeded(bit_extractor.GetNextBit());
        mCRSFSimulationData.Advance(mClockGenerator.AdvanceByHalfPeriod());
    }

    if (AnalyzerHelpers::IsEven(AnalyzerHelpers::GetOnesCount(value)) == true) {
        mCRSFSimulationData.TransitionIfNeeded(BIT_LOW); // we want to add a zero bit
    } else {
        mCRSFSimulationData.TransitionIfNeeded(BIT_HIGH); // we want to add a one bit
    }

    mCRSFSimulationData.Advance(mClockGenerator.AdvanceByHalfPeriod());

    mCRSFSimulationData.TransitionIfNeeded(BIT_HIGH); // we need to end high

    // lets pad the end a bit for the stop bit:
    mCRSFSimulationData.Advance(mClockGenerator.AdvanceByHalfPeriod(1));
}
