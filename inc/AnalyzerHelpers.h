#ifndef ANALYZER_HELPERS_H
#define ANALYZER_HELPERS_H

#include "Analyzer.h"

class LOGICAPI AnalyzerHelpers
{
public:
    static bool IsEven(U64 value);
    static bool IsOdd(U64 value);
    static U32 GetOnesCount(U64 value);
    static U32 Diff32(U32 a, U32 b);

    static void GetNumberString(U64 number, DisplayBase display_base, U32 num_data_bits, char *result_string, U32 result_string_max_length);
    static void GetTimeString(U64 sample, U64 trigger_sample, U32 sample_rate_hz, char *result_string, U32 result_string_max_length);

    static void Assert(const char *message);
    static U64 AdjustSimulationTargetSample(U64 target_sample, U32 sample_rate, U32 simulation_sample_rate);

    static bool DoChannelsOverlap(const Channel *channel_array, U32 num_channels);
    static void SaveFile(const char *file_name, const U8 *data, U32 data_length, bool is_binary = false);

    static S64 ConvertToSignedNumber(U64 number, U32 num_bits);

    //These save functions should not be used with SaveFile, above. They are a better way to export data (don't waste memory), and should be used from now on.
    static void *StartFile(const char *file_name, bool is_binary = false);
    static void AppendToFile(const U8 *data, U32 data_length, void *file);
    static void EndFile(void *file);
};


struct ClockGeneratorData;

/**
 * ClockGenerator is a class provided in AnalyzerHelpers.h which will let you enter time values, rather than
 * numbers-of-samples.
 * 
 * First, create an object using the ClockGenerator class, and then call the init function to initialize it.
 */
class LOGICAPI ClockGenerator
{
public:
    ClockGenerator();
    ~ClockGenerator();
    /**
     * You’ll need to call this before using the class. For sample_rate_hz, enter the sample rate we’ll be
     * generating data at. For target_frequency, enter the frequency (in Hz) you will most commonly be using.
     * For example, the bit rate of a SPI clock, etc.
     */
    void Init(double target_frequency, U32 sample_rate_hz);

    /**
     * This function returns how many samples are needed to move forward by one half of the period (for
     * example, the low time for a perfect square wave). You can also enter a multiple. For example, to get the
     * number of samples to move forward for a full period, enter 2.0.
     */
    U32 AdvanceByHalfPeriod(double multiple = 1.0);

    /**
     * This functions provides number of samples needed to advance by the arbitrary time, time_s. Note that
     * this is in seconds, so enter 1e-6 for for one microsecond, etc
     */
    U32 AdvanceByTimeS(double time_s);

protected:
    struct ClockGeneratorData *mData;
};


struct BitExtractorData;

/**
 * Some protocols have variable number of bits per word and settings for if the most significant bit is
 * first or last. This can be a pain to manage, so we made the BitExtraxtor class. This can be done by hand
 * of course if you like, but this class tends to tidy up the code quite a bit in our experiece.
 */
class LOGICAPI BitExtractor
{
public:
    BitExtractor(U64 data, AnalyzerEnums::ShiftOrder shift_order, U32 num_bits);
    ~BitExtractor();

    BitState GetNextBit();

protected:
    struct BitExtractorData *mData;
};


struct DataBuilderData;
class LOGICAPI DataBuilder
{
public:
    DataBuilder();
    ~DataBuilder();

    void Reset(U64 *data, AnalyzerEnums::ShiftOrder shift_order, U32 num_bits);
    void AddBit(BitState bit);

protected:
    struct DataBuilderData *mData;
};


struct SimpleArchiveData;
class LOGICAPI SimpleArchive
{
public:
    SimpleArchive();
    ~SimpleArchive();

    void SetString(const char *archive_string);
    const char *GetString();

    bool operator<<(U64 data);
    bool operator<<(U32 data);
    bool operator<<(S64 data);
    bool operator<<(S32 data);
    bool operator<<(double data);
    bool operator<<(bool data);
    bool operator<<(const char *data);
    bool operator<<(Channel &data);

    bool operator>>(U64 &data);
    bool operator>>(U32 &data);
    bool operator>>(S64 &data);
    bool operator>>(S32 &data);
    bool operator>>(double &data);
    bool operator>>(bool &data);
    bool operator>>(char const **data);
    bool operator>>(Channel &data);

protected:
    struct SimpleArchiveData *mData;
};

#endif //ANALYZER_HELPERS_H
