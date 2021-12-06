/*
*	entrypoint.h sets up the application executable to run in engine when
*	the application uses the lib as a dependency
*/
#pragma once
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>
#include "logger.h"
#include "app-frame.h"

#ifdef _DEBUG
	#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
	#define NEW new
#endif

// Declare that the createApp function should be defined in the client application
extern engine::u_Ptr<engine::AppFrame> engine::createApp();

int main(int argc, char** argv) {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//Intro
	std::cout << "Engine is running ...\n";

	// Logger
	auto logger = NEW engine::Logger();		// Define logger
	ENGINE_INFO("Logger is running ...");	// Test logger

	// Define a base application for the client to run on
	auto app = engine::createApp();	// Creates unique ptr no need to manually delete
	app->run();
	delete logger;
	return 0;
}