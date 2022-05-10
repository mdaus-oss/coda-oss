#ifndef CODA_OSS_config_Exports_h_INCLUDED_
#define CODA_OSS_config_Exports_h_INCLUDED_
#pragma once

// Need to specify how this module will be consumed, either CODA_OSS_LIBRARY_LIB (static library)
// or CODA_OSS_LIBRARY_DLL (aka "shared" library).
//
// Use Windows naming conventions (DLL, LIB) because this really only matters for _MSC_VER, see below.
#if !defined(CODA_OSS_LIBRARY_LIB) && !defined(CODA_OSS_LIBRARY_DLL)
    //#define CODA_OSS_LIBRARY_LIB 1
    //#define CODA_OSS_LIBRARY_DLL 1
#endif
#if defined(CODA_OSS_LIBRARY_EXPORTS)
    #if defined(CODA_OSS_LIBRARY_LIB)
        #error "Can't export from a LIB'"
    #endif

    #if !defined(CODA_OSS_LIBRARY_DLL)
        #define CODA_OSS_LIBRARY_DLL 1
    #endif
#endif
#if defined(CODA_OSS_LIBRARY_LIB) && defined(CODA_OSS_LIBRARY_DLL)
    #error "Both CODA_OSS_LIBRARY_LIB and CODA_OSS_LIBRARY_DLL are #define'd'"
#endif

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the NITRO_NITFCPP_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// NITRO_NITFCPP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
// https://www.gnu.org/software/gnulib/manual/html_node/Exported-Symbols-of-Shared-Libraries.html
#ifdef CODA_OSS_LIBRARY_EXPORTS
    #if defined(__GNUC__) // && HAVE_VISIBILITY 
        #define CODA_OSS_LIBRARY_API __attribute__((visibility("default")))
    #elif defined(_MSC_VER) // && (defined(_WINDLL) && !defined(_LIB))
        #define CODA_OSS_LIBRARY_API __declspec(dllexport)
    #else
        // https://stackoverflow.com/a/2164853/8877
        #define CODA_OSS_LIBRARY_API /* do nothing and hope for the best? */
        #pragma warning Unknown dynamic link export semantics.
    #endif
#else
    // Either building a static library (no NITRO_NITFCPP_EXPORTS) or
    // importing (not building) a shared library.
    #if defined(_MSC_VER)
        // We need to know whether we're consuming (importing) a DLL or static LIB
        // The default is a static LIB as that's what existing code/builds expect.
        #ifdef CODA_OSS_LIBRARY_DLL
            // Actually, it seems that the linker is able to figure this out from the .LIB, so 
            // there doesn't seem to be a need for __declspec(dllimport).  Clients don't
            // need to #define NITRO_NITFCPP_DLL ... ?  Well, almost ... it looks
            // like __declspec(dllimport) is needed to get virtual "inline"s (e.g., 
            // destructors) correct.
            #define CODA_OSS_LIBRARY_API __declspec(dllimport)
        #else
            #define CODA_OSS_LIBRARY_API /* "importing" a static LIB */
        #endif
    #elif defined(__GNUC__)
        // For GCC, there's no difference in consuming ("importing") an .a or .so
        #define CODA_OSS_LIBRARY_API /* no __declspec(dllimport) for GCC */
    #else
        // https://stackoverflow.com/a/2164853/8877
        #define CODA_OSS_LIBRARY_API /* do nothing and hope for the best? */
        #pragma warning Unknown dynamic link import semantics.
    #endif
#endif

#ifndef CODA_OSS_building_SWIG_
#define CODA_OSS_building_SWIG_ (defined(SWIG) || defined(SWIGPYTHON) || defined(HAVE_PYTHON_H)  )
#endif 
#if CODA_OSS_building_SWIG_
#undef CODA_OSS_LIBRARY_API // causes errors with SWIG
#define CODA_OSS_LIBRARY_API /*SWIG build*/ // __declspec() or __attribute__ breaks SWIG
#endif

#if defined(_MSC_VER) && !CODA_OSS_building_SWIG_
#pragma warning(disable: 4251) // '...' : class '...' needs to have dll-interface to be used by clients of struct '...'
#endif

#endif // CODA_OSS_config_Exports_h_INCLUDED_