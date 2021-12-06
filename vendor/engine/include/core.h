#pragma once
#include <signal.h>
#include <memory>

/*
	core.h is used for predefinition of:
		- Function and variable macros
		- Platform specific variables
		- Simplification of std library casts (e.g. smart pointer calls)
*/

/*
	Logging assertions based on condition parameter x
*/
#ifdef ENABLE_ASSERTS
	#if defined PLATFORM_WINDOWS
		#define APP_ASSERT(x, ...) { if(!(x)) { APP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#define ENGINE_ASSERT(x, ...) { if(!(x)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#elif defined PLATFORM_UNIX
		#if defined SIGTRAP
			#define APP_ASSERT(x, ...) { if(!(x)) { APP_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
			#define ENGINE_ASSERT(x, ...) { if(!(x)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
		#else
			#define APP_ASSERT(x, ...) { if(!(x)) { APP_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGABRT); } }
			#define ENGINE_ASSERT(x, ...) { if(!(x)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); raise(SIGABRT); } }
		#endif
	#else
		#error The program might be running on an unsupported platform, supported platforms(WINDOWS, UNIX)
	#endif
#else	// Compile code without assertions
	#define APP_ASSERT(x, ...)
	#define ENGINE_ASSERT(x, ...)
#endif

	// Usage of bind here a bit complicated, but simplified, this macro
	// is binding a default set of parameters with a custom one when called
	// Allowing us to bind event functions based on the event class that fits them
	// And running that custom event with a callback function until it returns a result
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


namespace engine {

	/*
		Custom pointer references for engine assets (can be used application side)
	*/
	// Custom unique_ptr to u_Ptr
	template<typename T>
	using u_Ptr = std::unique_ptr<T>;

	// Custom std::make_unique function to m_UPtr()
	template<typename F, typename ... Args>
	constexpr u_Ptr<F> m_UPtr(Args&& ... args) {
		return std::make_unique<F>(std::forward<Args>(args)...);
	}

	// Custom shared_ptr to s_Ptr
	template<typename T>
	using s_Ptr = std::shared_ptr<T>;

	// Custom std::make_shared function to m_SPtr()
	template<typename F, typename ... Args>
	constexpr s_Ptr<F> m_SPtr(Args&& ... args) {
		return std::make_shared<F>(std::forward<Args>(args)...);
	}
}
