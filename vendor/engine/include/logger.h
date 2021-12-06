/*
	logger.h is used as a precompileed header in engine/precompiled.h
*/
#pragma once

#include <memory>

#include "core.h"

#include <spdlog/spdlog.h>						// Logger header
#include <spdlog/sinks/stdout_color_sinks.h>	// Log formatting
#include <spdlog/fmt/ostr.h>					// Logging of custom classes

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {

	/*
		Logger utilizes the logging capabilities provided by the spdlog library
	*/
	class Logger {
	public:
		Logger();	// Set logger settings before using macros
		
		inline static s_Ptr<spdlog::logger>& getEngineLogger() { return s_EngineLogger; };
		inline static s_Ptr<spdlog::logger>& getAppLogger() { return s_AppLogger; };

	private:
		static s_Ptr<spdlog::logger> s_EngineLogger;
		static s_Ptr<spdlog::logger> s_AppLogger;
	};

	/*
		MACRO DEFINITIONS ENGINE
	*/
	#define ENGINE_ERROR(...)	::engine::Logger::getEngineLogger()->error(__VA_ARGS__)		// ERROR LOGGING
	#define ENGINE_TRACE(...)	::engine::Logger::getEngineLogger()->trace(__VA_ARGS__)		// RUNTIME EXECUTION LOGGING
	#define ENGINE_FATAL(...)	::engine::Logger::getEngineLogger()->critical(__VA_ARGS__)	// FATAL ERROR LOGGING
	#define ENGINE_INFO(...)	::engine::Logger::getEngineLogger()->info(__VA_ARGS__)		// INFORMATION LOGGING
	#define ENGINE_WARN(...)	::engine::Logger::getEngineLogger()->warn(__VA_ARGS__)		// WARNING LOGGING            
								
	/*							
		MACRO DEFINITIONS APP	
	*/							
	#define APP_ERROR(...)		::engine::Logger::getAppLogger()->error(__VA_ARGS__)		// ERROR LOGGING
	#define APP_TRACE(...)		::engine::Logger::getAppLogger()->trace(__VA_ARGS__)		// RUNTIME EXECUTION LOGGING
	#define APP_FATAL(...)		::engine::Logger::getAppLogger()->critical(__VA_ARGS__)		// FATAL ERROR LOGGING
	#define APP_INFO(...)		::engine::Logger::getAppLogger()->info(__VA_ARGS__)			// INFORMATION LOGGING
	#define APP_WARN(...)		::engine::Logger::getAppLogger()->warn(__VA_ARGS__)			// WARNING LOGGING             
}