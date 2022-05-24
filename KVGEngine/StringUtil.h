#pragma once
#include "pch.h"

namespace KGV::Util {
	class Log; // Circular dependency so we need forward declaration.

	class StringUtil {
	public:
		/*
		 * @brief This function acts much like printf(). It takes a string that specifies the format and a variadic list of arguments.
		 * @param format A string specifying the format of the desired string.
		 * @param
		 * @return The formatted string.
		 * 
		 * Inspired by https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
		*/
		template<typename ... Args>
		static std::string formatString( const std::string& format, const Args& ... args ) {
			KGV_ASSERT_FALSE( format.empty() );

			// snprintf returns the number of characters in a string if the buffer size is 0. So we can exploit this as a strlen() function but for variadic strings.
			// We need this length so that we can allocate a buffer that matches the size needed by the string. 
			long length = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Add one to the length for the '\0' terminator.
			KGV_ASSERT_TRUE( length >= 0 );

			auto size = static_cast< size_t >( length ); // Cast to size_t so we avoid compiler warnings.
			auto buffer = std::make_unique<char[]>( size ); // Generate a unique pointer for automatic cleanup.
			memset( buffer.get(), 0, size );

			// Format the string.
			std::snprintf( buffer.get(), size, format.c_str(), args ... );

			return std::string( buffer.get(), buffer.get() + size - 1 ); // Get rid of the terminator.
		}
	};
}