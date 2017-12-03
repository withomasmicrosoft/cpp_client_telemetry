#include "Version.hpp"
#include <collection.h>

#include "ISystemInformation.hpp"
#include "pal/SystemInformationImpl.hpp"
#include "pal/win32/WindowsEnvironmentInfo.h"
#include "PlatformHelpers.h"

namespace Microsoft {
    namespace Applications {
        namespace Telemetry {
            namespace PAL
            {
				using namespace std;
                using namespace ::Windows::ApplicationModel;
                using namespace ::Windows::System::UserProfile;
                using namespace ::Windows::System::Profile;
                using namespace ::Windows::Globalization;

                using namespace Microsoft::Applications::Telemetry::Windows;

                const string WindowsOSName = "Windows";
                const string WindowsPhoneOSName = "Windows for Phones";
                const string DeviceFamily_Mobile = "Windows.Mobile";

				ISystemInformation* SystemInformationImpl::Create()
                {
                    return new SystemInformationImpl();
                }

                /**
                * Get App Id
                */
                std::string getAppId()
                {
                    return FromPlatformString(Package::Current->Id->Name);;
                }

                /**
                * Get app module version
                */
                std::string getAppVersion()
                {
                    auto version = Package::Current->Id->Version;
                    std::string appversion = std::to_string(version.Major) + "." + std::to_string(version.Minor)
                        + "." + std::to_string(version.Build) + "." + std::to_string(version.Revision);
                    return appversion;
                }

                /**
                * Get OS major and full version strings
                */
                void getOsVersion(std::string& osMajorVersion, std::string& osFullVersion)
                {
                    // The DeviceFamilyVersion is a decimalized form of the ULONGLONG hex form. For example:
                    // 2814750430068736 = 000A000027840000 = 10.0.10116.0
                    auto versionDec = std::stoull(AnalyticsInfo::VersionInfo->DeviceFamilyVersion->Data());
                    if (versionDec != 0ull)
                    {
                        osMajorVersion = std::to_string(versionDec >> 16 * 3) + "." + std::to_string(versionDec >> 16 * 2 & 0xFFFF);
                        osFullVersion = osMajorVersion +
                            "." + std::to_string(versionDec >> 16 & 0xFFFF) + "." + std::to_string(versionDec & 0xFFFF);
                    }
                    else
                    {
                        osMajorVersion = "10.0";
                    }
                }

                SystemInformationImpl::SystemInformationImpl()
                    : m_info_helper()
                {
                    m_app_id = getAppId();
                    m_app_version = getAppVersion();

                    m_user_language = FromPlatformString(GlobalizationPreferences::Languages->GetAt(0));
                    m_user_timezone = WindowsEnvironmentInfo::GetTimeZone();

                    try
                    {
                        m_user_advertising_id = FromPlatformString(AdvertisingManager::AdvertisingId);
                    }
                    catch (Exception^)
                    {
                        // This throws FileNotFoundException on Windows 10 v10033/UAP v22623.
                    }
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
                    // Only works for Windows Phone 8.1. A run-time check (below) is required in Threshold.
                    m_os_name = WindowsPhoneOSName;
#else
                    m_os_name = WindowsOSName;
#endif

#ifdef _WIN32_WINNT_WIN10

                    getOsVersion(m_os_major_version, m_os_full_version);      
                    m_device_family = FromPlatformString(AnalyticsInfo::VersionInfo->DeviceFamily);

                    if (FromPlatformString(AnalyticsInfo::VersionInfo->DeviceFamily) == DeviceFamily_Mobile)
                    {
                        m_os_name = WindowsPhoneOSName;
                    }

#else // Windows 8.1 SDK
                    m_os_major_version = "8.1";
                    // There is no API to get full version in Windows 8.1.
#endif
                    String ^primaryLanguage = "en";
                    try {
                        if (ApplicationLanguages::Languages->Size) {
                            primaryLanguage = ApplicationLanguages::Languages->GetAt(0);
                        }
                        if (ApplicationLanguages::PrimaryLanguageOverride != nullptr) {
                            primaryLanguage = ApplicationLanguages::PrimaryLanguageOverride;
                        }
                    }
                    catch (...) {
                        //CTDEBUGLOG("Language detection may not be reliable!");
                    }
                    m_app_language = FromPlatformString(primaryLanguage);
                   // CTDEBUGLOG("m_app_language=%s", m_app_language.c_str());
                }

                SystemInformationImpl::~SystemInformationImpl()
                {
                }

                int SystemInformationImpl::RegisterInformationChangedCallback(IPropertyChangedCallback* pCallback)
                {
                    return m_info_helper.RegisterInformationChangedCallback(pCallback);
                }

                void SystemInformationImpl::UnRegisterInformationChangedCallback(int callbackToken)
                {
                    m_info_helper.UnRegisterInformationChangedCallback(callbackToken);
                }
            }
        }
    }
}