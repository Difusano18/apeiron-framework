#pragma once

#ifdef _WIN32
    #ifdef APEIRON_DISTRIBUTED_BUILD
        #define APEIRON_DISTRIBUTED_API __declspec(dllexport)
    #else
        #define APEIRON_DISTRIBUTED_API __declspec(dllimport)
    #endif
    #pragma warning(disable: 4251)
#else
    #define APEIRON_DISTRIBUTED_API __attribute__((visibility("default")))
#endif
