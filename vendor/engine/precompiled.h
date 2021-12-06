/*
	Precompiled header file for an easier include life
	 -	These includes are predictions of what needs to be used
		unused includes will be removed over time.
*/
#pragma once

#ifdef _DEBUG
	#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
	#define NEW new
#endif

#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <stack>
#include <cstdint>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif 
