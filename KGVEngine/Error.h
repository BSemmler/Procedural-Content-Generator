#include "pch.h"

#ifdef _WIN32
#include <intrin.h>
// TODO: Look into why this causes an exception.
#define KGV_debugBreak()  asm { INT 3 }// __debugbreak() // X86 software breakpoint. Equalivalent to INT 3 in assembly
#else
#define KGV_debugBreak()
#endif

// Derived from Section 3.2 of Game Engine Architecture (3rd Edition) By Jason Gregory
#ifdef KGV_ASSERTIONS_ENABLED

#define KGV_ASSERT_FALSE(expr) \
if (expr) \
{ \
spdlog::debug("DEBUG Assertion tripped: {}, file: {}, line: {}", #expr, __FILE__, __LINE__); \
 KGV_debugBreak(); \
}

#define KGV_ASSERT_TRUE(expr) \
if (expr) {}\
else \
{ \
spdlog::debug("DEBUG Assertion tripped: {}, file: {}, line: {}", #expr, __FILE__, __LINE__); \
 KGV_debugBreak(); \
}


#else

#define KGV_ASSERT_TRUE(expr)
#define KGV_ASSERT_FALSE(expr)

#endif

// Derived from Section 3.2 of Game Engine Architecture (3rd Edition) By Jason Gregory
#ifdef KGV_SLOW_ASSERTIONS_ENABLED

#define KGV_SLOW_ASSERT_TRUE(expr) \
if (expr) {} \
else \
{ \
spdlog::debug("DEBUG Assertion tripped: {}, file: {}, line: {}", #expr, __FILE__, __LINE__); \
 KGV_debugBreak(); \
}

#define KGV_SLOW_ASSERT_FALSE(expr) \
if (expr) {} \
else \
{ \
spdlog::debug("DEBUG Assertion tripped: {}, file: {}, line: {}", #expr, __FILE__, __LINE__); \
 KGV_debugBreak(); \
}

#else
#define KGV_SLOW_ASSERT_TRUE(expr)
#define KGV_SLOW_ASSERT_FALSE(expr)
#endif