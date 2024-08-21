#ifndef CRSF_ANALYZER_SETTINGS
#define CRSF_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>

namespace CRSFEnums
{
    enum BaudRate { Baud_1_87M = 1870000 };
};


class CRSFAnalyzerSettings : public AnalyzerSettings
{
public:
    CRSFAnalyzerSettings();
    virtual ~CRSFAnalyzerSettings();
    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    virtual void LoadSettings(const char *settings);
    virtual const char *SaveSettings();

    Channel channel;
    double baudRate;

protected:
    std::auto_ptr< AnalyzerSettingInterfaceChannel > channelInterface;
    std::auto_ptr< AnalyzerSettingInterfaceNumberList > baudRateInterface;
};

#endif //CRSF_ANALYZER_SETTINGS
