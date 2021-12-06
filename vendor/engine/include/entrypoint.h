/*
*	entrypoint.h sets up the application executable to run in engine when
*	the application uses the lib as a dependency
*/
#pragma once
#include <iostream>
#include "logger.h"
#include "app-frame.h"

// Declare that the createApp function should be defined in the client application
extern engine::u_Ptr<engine::AppFrame> engine::createApp();

int main(int argc, char** argv) {
	//Intro
	std::cout << "Engine is running ...\n";

	// Logger
	auto logger = new engine::Logger();		// Define logger
	ENGINE_INFO("Logger is running ...");	// Test logger

	// Define a base application for the client to run on
	auto app = engine::createApp();	// Creates unique ptr no need to manually delete
	app->run();
	delete logger;
	return 0;
}