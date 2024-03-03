#include "pch.h"
#include "ApplicationWin32.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/cfg/argv.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

int main(int argc, const char **argv) {
    spdlog::init_thread_pool(8192, 1);
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_level((spdlog::level::warn));
    auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log", 5 * 1024 * 1024, 5);
    fileSink->set_level(spdlog::level::trace);
    std::vector<spdlog::sink_ptr> sinks{consoleSink, fileSink};
    auto engineLogger = std::make_shared<spdlog::async_logger>(
            "engine",
            sinks.begin(),
            sinks.end(),
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block);

    auto renderLogger = std::make_shared<spdlog::async_logger>(
            "render",
            sinks.begin(),
            sinks.end(),
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block);

    spdlog::register_logger(engineLogger);
    spdlog::register_logger(renderLogger);

    spdlog::cfg::load_env_levels();
    spdlog::cfg::load_argv_levels(argc, argv);

    KGV::System::ApplicationWin32 app;
    if (!app.init())
        return 1;

    app.run();

    return 0;
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShowCmd)
//{
//    spdlog::
////	g_log = std::make_shared<KGV::Util::Log>();
//	g_log->setFilters( KGV::Util::LogVerbosity::kInfo, KGV::Util::LogChannel::kAllChannels );
//	g_log->writeToLog( KGV::Util::LogVerbosity::kError, KGV::Util::LogChannel::kSystem, "%s", "Hello World!" );
//
//	KGV::System::ApplicationWin32 app;
//	if (!app.init())
//        return 1;
//
//	app.run();
//
//	return 0;
//}