// Copyright (c) Microsoft. All rights reserved.
#ifndef LIB_UTILS_HPP
#define LIB_UTILS_HPP

// This file is being used during both C++ native and C++/CX managed compilation with /cli flag
// Certain features, e.g. <mutex> or <thread> - cannot be used while building with /cli
// For this reason this header cannot include any other headers that rely on <mutex> or <thread>

#include "Version.hpp"
#include "Enums.hpp"

#include <chrono>
#include <algorithm>
#include <string>

#include "EventProperty.hpp"

/* Lean implementation of SLDC "Annex K" for non-Windows OS */
#include "annex_k.hpp"

#ifdef __unix__
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#if defined(WINDOWS_UWP) || defined(__cplusplus_winrt)
#include <Windows.h>
#define _WINRT
#endif

namespace ARIASDK_NS_BEGIN {

    // FIXME: [MG] - refactor this
    extern const char* getAriaSdkLogComponent();

    typedef std::chrono::milliseconds ms;

    /* Obtain a backtrace and print it to stdout. */
    void print_backtrace(void);

    void sleep(unsigned delayMs);

    long		GetCurrentProcessId();

    std::string	GetTempDirectory();

    std::string toString(char const*        value);
    std::string toString(bool               value);
    std::string toString(char               value);
    std::string toString(int                value);
    std::string toString(long               value);
    std::string toString(long long          value);
    std::string toString(unsigned char      value);
    std::string toString(unsigned int       value);
    std::string toString(unsigned long      value);
    std::string toString(unsigned long long value);
    std::string toString(float              value);
    std::string toString(double             value);
    std::string toString(long double        value);

    std::string to_string(GUID_t uuid);

    inline std::string toLower(const std::string& str)
    {
        std::string result = str;
        std::transform(str.begin(), str.end(), result.begin(), ::tolower);
        return result;
    }

    inline std::string toUpper(const std::string& str)
    {
        std::string result = str;
        std::transform(str.begin(), str.end(), result.begin(), ::toupper);
        return result;
    }

    inline std::string sanitizeIdentifier(std::string &str)
    {
#if 0
        // FIXME: [MG] - we have to add some sanitizing logic, but NOT replacing dots by underscores
        std::replace(str.begin(), str.end(), '.', '_');
#endif
        return str;
    }

    EventRejectedReason validateEventName(std::string const& name);

    EventRejectedReason validatePropertyName(std::string const& name);

    inline std::string tenantTokenToId(std::string const& tenantToken)
    {
        return tenantToken.substr(0, tenantToken.find('-'));
    }

    inline const char* priorityToStr(EventPriority priority)
    {
        switch (priority) {
        case EventPriority_Unspecified:
            return "Unspecified";

        case EventPriority_Off:
            return "Off";

        case EventPriority_Low:
            return "Low";

        case EventPriority_Normal:
            return "Normal";

        case EventPriority_High:
            return "High";

        case EventPriority_Immediate:
            return "Immediate";

        default:
            return "???";
        }
    }

    inline const char* latencyToStr(EventLatency latency)
    {
        switch (latency) {
        case EventLatency_Unspecified:
            return "Unspecified";

        case EventLatency_Off:
            return "Off";

        case EventLatency_Normal:
            return "Normal";

        case EventLatency_CostDeferred:
            return "CostDeferred";

        case EventLatency_RealTime:
            return "RealTime";

        case EventPriority_Immediate:
            return "Immediate";

        default:
            return "???";
        }
    }

    inline bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

#ifdef _WINRT

    Platform::String ^to_platform_string(const std::string& s);

    std::string from_platform_string(Platform::String ^ ps);

#endif

} ARIASDK_NS_END

#endif
