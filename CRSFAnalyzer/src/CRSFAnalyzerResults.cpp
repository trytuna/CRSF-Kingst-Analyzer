#include "CRSFAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "CRSFAnalyzer.h"
#include "CRSFAnalyzerSettings.h"
#include <iostream>
#include <sstream>
#include <stdio.h>

#define FRAMING_ERROR_FLAG (1 << 0)

CRSFAnalyzerResults::CRSFAnalyzerResults(CRSFAnalyzer *analyzer, CRSFAnalyzerSettings *settings)
    :   AnalyzerResults(),
        settings(settings),
        analyzer(analyzer)
{}

CRSFAnalyzerResults::~CRSFAnalyzerResults()
{}

void CRSFAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel & /*channel*/, DisplayBase display_base)
{
    ClearResultStrings();
    Frame frame = GetFrame(frame_index);

    char number_str[128];
    AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);
    printf("bubble text %llu data => %llu number_str => %s\n", frame_index, frame.mData1, number_str);

    char result_str[128];

    // switch (frame.mData1)
    // {
    // case 0xEE:
    //     // sync
    //     break;
    // case 
    // default:
    //     break;
    // }

    snprintf(result_str, sizeof(result_str), "%s", number_str);
    AddResultString(result_str);
}

void CRSFAnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 /*export_type_user_id*/)
{
    //export_type_user_id is only important if we have more than one export type.
    std::stringstream string_stream;

    U64 trigger_sample = analyzer->GetTriggerSample();
    U32 sample_rate = analyzer->GetSampleRate();
    U64 num_frames = GetNumFrames();

    void *f = AnalyzerHelpers::StartFile(file);

    // Add CSV header
    string_stream << "Time [s],Value,Framing Error" << std::endl;

    // Add actual data
    for (U32 i = 0; i < num_frames; i++) {
        Frame frame = GetFrame(i);

        //static void GetTimeString( U64 sample, U64 trigger_sample, U32 sample_rate_hz, char* result_string, U32 result_string_max_length );
        char time_str[128];
        AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);

        char number_str[128];
        AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);

        string_stream << time_str << "," << number_str << ",";


        if ((frame.mFlags & FRAMING_ERROR_FLAG) != 0) {
            string_stream << "Error";
        }

        string_stream << std::endl;

        AnalyzerHelpers::AppendToFile((U8 *)string_stream.str().c_str(), string_stream.str().length(), f);
        string_stream.str(std::string());

        if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true) {
            AnalyzerHelpers::EndFile(f);
            return;
        }
    }

    UpdateExportProgressAndCheckForCancel(num_frames, num_frames);
    AnalyzerHelpers::EndFile(f);
}

void CRSFAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
    ClearTabularText();
    Frame frame = GetFrame(frame_index);

    char number_str[128];
    AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);

    char result_str[128];

    snprintf(result_str, sizeof(result_str), "%s", number_str);
    AddTabularText(result_str);
}

void CRSFAnalyzerResults::GeneratePacketTabularText(U64 /*packet_id*/, DisplayBase /*display_base*/)
{
    ClearResultStrings();
    AddResultString("not supported");
}

void CRSFAnalyzerResults::GenerateTransactionTabularText(U64 /*transaction_id*/, DisplayBase /*display_base*/)
{
    ClearResultStrings();
    AddResultString("not supported");
}
