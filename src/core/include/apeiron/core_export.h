#pragma once

#ifdef _WIN32
    #ifdef APEIRON_CORE_BUILD
        #define APEIRON_API __declspec(dllexport)
    #else
        #define APEIRON_API __declspec(dllimport)
    #endif
#else
    #define APEIRON_API __attribute__((visibility("default")))
#endif
