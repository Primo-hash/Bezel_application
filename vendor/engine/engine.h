#pragma once

#ifdef _DEBUG
	#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
	#define NEW new
#endif

/*
*	engine.h is the static lib header file to access engine and STL tools
*/

// STL
#include <iostream>

// Core
#include "engine/include/core.h"

// Application frame
#include "engine/include/app-frame.h"

// Application definition runs engine side through an entrypoint
#include "engine/include/entrypoint.h"

// Logging
#include "engine/include/logger.h"

// Time
#include "engine/include/time.h"

// Layers
#include "engine/include/layer.h"

// Input
#include "engine/include/input.h"
#include "engine/include/graphics/camera/camera-controller.h"

// Graphics
#include "engine/include/graphics/renderer.h"
#include "engine/include/graphics/renderAPI.h"
		  				 
#include "engine/include/graphics/buffer.h"
#include "engine/include/graphics/shader.h"
#include "engine/include/graphics/texture.h"
#include "engine/include/graphics/vertex-array.h"
		  				 
#include "engine/include/graphics/camera/orthographic-camera.h"
#include "engine/include/graphics/camera/perspective-camera.h"