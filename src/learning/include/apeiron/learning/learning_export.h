#pragma once

#ifdef _WIN32
    #ifdef APEIRON_LEARNING_BUILD
        #define APEIRON_LEARNING_API __declspec(dllexport)
    #else
        #define APEIRON_LEARNING_API __declspec(dllimport)
    #endif
    #pragma warning(disable: 4251)
#else
    #define APEIRON_LEARNING_API __attribute__((visibility("default")))
#endif
