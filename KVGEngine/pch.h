// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
// STD
#include <cstdarg>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <memory>
#include <fstream>
#include <vector>

// Internal
#include "Types.h"
#include "Platform.h"
#include "Error.h"
#include "StringUtil.h"
#include "Log.h"

extern std::shared_ptr<KGV::Util::Log> g_log;
#endif //PCH_H