// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once
#ifndef CODA_OSS_pch_h_INCLUDED_
#define CODA_OSS_pch_h_INCLUDED_

#pragma warning(disable: 4619) // #pragma warning: there is no warning number '...'

#pragma warning(disable: 4820) // '...': '...' bytes padding added after data member '...'
#pragma warning(disable: 4668) // '...' is not defined as a preprocessor macro, replacing with '...' for '...'
#pragma warning(disable: 5045) // Compiler will insert Spectre mitigation for memory load if / Qspectre switch specified
#pragma warning(disable: 4514) //	'...': unreferenced inline function has been removed

#pragma warning(push)
#pragma warning(disable: 4710) // '...': function not inlined
#pragma warning(disable: 5027) // '...': move assignment operator was implicitly defined as deleted
#pragma warning(disable: 5026) // '...': move constructor was implicitly defined as deleted
#pragma warning(disable: 4626) // '...': assignment operator was implicitly defined as deleted
#pragma warning(disable: 4625) // '...': copy constructor was implicitly defined as deleted
#pragma warning(disable: 4355) // '...': used in base member initializer list
#pragma warning(disable: 5220) // '...': a non-static data member with a volatile qualified type no longer implies that compiler generated copy/move constructors and copy/move assignment operators are not trivial
#pragma warning(disable: 5204) // '...': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
#pragma warning(disable: 5264)  // '...': '...' variable is not used

// add headers that you want to pre-compile here
#include "framework.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "import/std.h"

#pragma warning(pop)

#if defined(USE_XERCES)
CODA_OSS_disable_warning_system_header_push
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/ContentHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>

#include <xercesc/util/XMLUni.hpp>

#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/validators/schema/SchemaValidator.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>

CODA_OSS_disable_warning_pop
#endif // USE_XERCES

#include <net/net_config.h>
#include <zlib.h>
#include <xml/lite/xml_lite_config.h>

#pragma warning(disable: 4251) // '...': class '...' needs to have dll-interface to be used by clients of class '...'
#pragma warning(disable: 5105) // macro expansion producing '...' has undefined behavior


// Yes, these are our files ... but they don't change very often, and if they do change we want
// to rebuild everything anyway.
#include "gsl/gsl.h"
#include "config/Exports.h"
#include "except/Throwable.h"
#include "sys/Conf.h"
#include "sys/filesystem.h"

#endif //CODA_OSS_pch_h_INCLUDED_
