// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
// STD
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <cstdarg>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <random>
#include <queue>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>

// Internal
#include "Types.h"
#include "Platform.h"
#include "utility/Error.h"



#endif //PCH_H
