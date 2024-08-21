#include "CRSFAnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <sstream>
#include <cstring>

#pragma warning(disable: 4800) // warning C4800: 'U32' : forcing value to bool 'true' or 'false' (performance warning)

CRSFAnalyzerSettings::CRSFAnalyzerSettings() 
    :   channel(UNDEFINED_CHANNEL),
        baudRate(CRSFEnums::Baud_1_87M)
{
    channelInterface.reset(new AnalyzerSettingInterfaceChannel());
    channelInterface->SetTitleAndTooltip("Channel", "Select the channel");
    channelInterface->SetChannel(channel);

    baudRateInterface.reset(new AnalyzerSettingInterfaceNumberList());
    baudRateInterface->SetTitleAndTooltip("Baud Rate", "Select a baud rate");
    baudRateInterface->AddNumber(CRSFEnums::Baud_1_87M, "1.87M", "");
    baudRateInterface->SetNumber(baudRate);
    
    AddInterface(channelInterface.get());
    AddInterface(baudRateInterface.get());

    AddExportOption(0, "Export as CSV file");
    AddExportExtension(0, "CSV file", "csv");

    ClearChannels();
    AddChannel(channel, "Channel", false);
}

CRSFAnalyzerSettings::~CRSFAnalyzerSettings()
{}

bool CRSFAnalyzerSettings::SetSettingsFromInterfaces()
{
    channel = channelInterface->GetChannel();
    baudRate = baudRateInterface->GetNumber();

    ClearChannels();
    AddChannel(channel, "Channel", true);

    return true;
}

void CRSFAnalyzerSettings::UpdateInterfacesFromSettings()
{
    channelInterface->SetChannel(channel);
    baudRateInterface->SetNumber(baudRate);
}

void CRSFAnalyzerSettings::LoadSettings(const char *settings)
{
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    const char *name_string;
    text_archive >> &name_string;

    if (strcmp(name_string, "CRSFAnalyzer") != 0) {
        AnalyzerHelpers::Assert("CRSFAnalyzer: unknown settings");
    }

    text_archive >> channel;
    
    CRSFEnums::BaudRate _baudRate;
    if(text_archive >> *(U32*)&_baudRate) {
        baudRate = _baudRate;
    }

    ClearChannels();
    AddChannel(channel, "Channel", true);

    UpdateInterfacesFromSettings();
}

const char *CRSFAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;

    text_archive << "CRSFAnalyzer";
    text_archive << channel;
    text_archive << baudRate;

    return SetReturnString(text_archive.GetString());
}
