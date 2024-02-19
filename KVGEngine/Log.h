//#pragma once
//#include "pch.h"
//
//namespace KGV::Util
//{
//	class StringUtil;
//
//	/**
//	 * @brief An enumerator for the various channel designations of the {@link Log} class. Being a header only log class
//	 * this enumerator and the {@link Log#
//	*/
//	enum class LogChannel : U32 {
//		kRender = 0x01,
//		kHID = 0x02,
//		kProfiler = 0x04,
//		kSystem = 0x08,
//		kSound = 0x10,
//		kPhysics = 0x20,
//		kAllChannels = 0xFFFFFFFF
//	};
//
//	inline LogChannel operator | ( LogChannel lhs, LogChannel rhs ) {
//		return static_cast<LogChannel>(static_cast<U32>(lhs) | static_cast<U32>(rhs));
//	}
//
//	inline LogChannel operator & ( LogChannel lhs, LogChannel rhs ) {
//		return static_cast<LogChannel>(static_cast<U32>(lhs) & static_cast<U32>(rhs));
//	}
//
//	/**
//	 * @brief An enumerator for the various verbosity levels of the {@link Log} class.
//	*/
//	enum class LogVerbosity : U32 {
//		kFatal = 6,
//		kSevere = 5,
//		kError = 4,
//		kWarning = 3,
//		kInfo = 2,
//		kDebug = 1
//	};
//
//	/**
//	 * @brief This class is for internal logging of the application. Errors, warnings and other issues or information can be logged using this utility.
//	 *
//	 * <p>This class is for internal logging of the application. Errors, warnings and other issues or information can be logged using this utility.
//	 * Additionally, class has filtering for specific systems and for an verbosity level. These filter settings can be configured with Log::setFilters().
//	 * The verbosity level determines the minimum verbosity of the messages that can be sent and the channel is a bitmask of the channels that should be logged</p>.
//	*/
//	class Log {
//	public:
//		/**
//		 * @brief Constructs a new Log object with the minimum verbosity level and all channels selected.
//		*/
//		Log() : Log( LogVerbosity::kInfo, LogChannel::kAllChannels ) {};
//
//		/**
//		 * @brief Constructs a new log object with the desired verbosity and channel parameters.
//		 * See {@link #setFilters(LogVerbosity, LogChannel) Log::setFilters()} for further information in regards to channels and verbosity.
//		 * @param verbosity
//		 * @param channel
//		*/
//		Log( LogVerbosity verbosity, LogChannel channel ) {
//			verbosityLevel = verbosity;
//			channelFilter = channel;
//		}
//
//		/**
//		 * @brief A basic copy constructor.
//		 * @param log Log object to be copied
//		*/
//		Log( Log& log ) {
//			channelFilter = log.channelFilter;
//			verbosityLevel = log.verbosityLevel;
//		}
//
//		/**
//		* @brief A variadic method that outputs a formated message to the console and a log file.
//		* @param verbosity The verbosity of the message.
//		* @param channel The channel that the message is associated with.
//		* @param format A string that represents the desired formatting of the parameters.
//		* @param parameters Any data that you wish to be outputed according to the format string.
//		* @return The number of characters written to console.
//		*
//		* <p> This method expects that there is non ORed channel argument passed to and that it is passed a non null format string.
//		* Additionally, it should be passed at the matching number of variadic string arguments for the format string.</p>
//		*/
//		template<typename ... Args>
//		int writeToLog( LogVerbosity verbosity, LogChannel channel, const std::string& format, const Args& ... args ) {
//			// Check if we're going to log the message or not
//			if ( verbosity < verbosityLevel || ( channel & channelFilter ) != channel ) {
//				return 0;
//			}
//
//			std::string outStr( formatMsg( verbosity, channel, format, args ... ) );
//			std::cout << outStr;
//			std::ofstream logFile( "log.txt", std::ios_base::app );
//			logFile << outStr;
//			logFile.close();
//
//
//			return outStr.size();
//		}
//
//		/**
//		* @brief This method sets the desired output settings of this logging utility.
//		* @param verbosityFilter The desired maximum verbosity.
//		* @param channelFilter A bitmask of the desired channels
//		*/
//		virtual void setFilters( LogVerbosity verbosityLevel, LogChannel channelFilter ) {
//			this->channelFilter = channelFilter;
//			this->verbosityLevel = verbosityLevel;
//		}
//
//		/**
//		 * @brief A method for reporting/logging a failed condition of KGV_ASSERT. Flushes buffer immediately!
//		 * @param expr A string of the evaluated expression.
//		 * @param file The file in which the failure occured.
//		 * @param line The line number in which the KGV_ASSERT is located.
//		 *
//		 * All parameters must be not null.
//		*/
//		static void reportAssertionFailure( const std::string& expr, const std::string& file, S64 line ) {
//			std::string outStr( "ASSERTION FAILURE! Expression: " + expr + " File: " + file + " Line: " + std::to_string( line ) + '\n' );
//			std::cout << outStr;
//			std::cout.flush();
//			std::ofstream logFile( "log.txt", std::ios_base::app );
//			logFile << outStr;
//			logFile.close();
//		}
//
//	protected:
//		/**
//		 * @brief Formats a message according the format string and the arguments passed in.
//		 * @tparam ...Args Variadic template for passing into strings for message output.
//		 * @param verbosity The verbosity level of the message.
//		 * @param channel The channel that this message belongs too.
//		 * @param format The format string.
//		 * @param ...args The variadic arguments.
//		 * @return A string with the format delimeters replaced by the arguments.
//		*/
//		template<typename ... Args>
//		std::string formatMsg( LogVerbosity verbosity, LogChannel channel, const std::string& format, const Args& ... args ) {
//			KGV_ASSERT_FALSE( format.empty() );
//
//			std::string s_verb;
//			std::string s_chan;
//
//			// Add verbosity designation to the log.
//			switch ( verbosity ) {
//			case LogVerbosity::kFatal:
//				s_verb = "FATAL";
//				break;
//			case LogVerbosity::kSevere:
//				s_verb = "SEVERE";
//				break;
//			case LogVerbosity::kError:
//				s_verb = "ERROR";
//				break;
//			case LogVerbosity::kWarning:
//				s_verb = "WARNING";
//				break;
//			case LogVerbosity::kInfo:
//				s_verb = "INFO";
//				break;
//			case LogVerbosity::kDebug:
//				s_verb = "DEBUG";
//				break;
//			default:
//				s_verb = "LOG ERROR! Message has an incorrect VERBOSITY attribute. Refer to Log Documentation. Original Message:";
//				break;
//			}
//
//			// Add the channel designation to the log.
//			switch ( channel ) {
//			case LogChannel::kRender:
//				s_chan = "RENDER";
//				break;
//			case LogChannel::kHID:
//				s_chan = "HID";
//				break;
//			case LogChannel::kProfiler:
//				s_chan = "PROFILER";
//				break;
//			case LogChannel::kSystem:
//				s_chan = "SYSTEM";
//				break;
//			default:
//				s_chan = "LOG ERROR! Message has an incorrect CHANNEL attribute. Refer to Log Documentation. Original Message:";
//				break;
//			}
//
//			return StringUtil::formatString( "%s %s " + format + "\n", s_chan.c_str(), s_verb.c_str(), args ... );
//		}
//
//		LogChannel		channelFilter;
//		LogVerbosity	verbosityLevel;
//	};
//
//}
