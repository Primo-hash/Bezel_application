#include "engine/precompiled.h"
#include "engine/include/logger.h"

namespace engine {

	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_AppLogger;

	engine::Logger::Logger() {
		// Possible message formats can be found here https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		// Sets format to Color>>Time>>LoggerName>>LogMessage for log messages
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
		s_EngineLogger->set_level(spdlog::level::trace);

		s_AppLogger = spdlog::stdout_color_mt("APP");
		s_AppLogger->set_level(spdlog::level::trace);
	}
}