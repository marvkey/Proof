#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <thread>
#include <stdio.h>
#include <array>
#include <iterator> 
#define FMT_HEADER_ONLY
#include <fmt/args.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/format-inl.h>
#include <fmt/core.h>

#include <magic_enum.hpp>
#include "Proof/Resources/EnumReflection.h"
//Proof defined
#include "Proof/Core/Log.h"
#include "Proof/Core/Core.h"
#include "Proof/Input/Input.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Core/Debug/Instrumentor.h"
#if defined(_WIN64)  // if platform is windows
	#include <windows.h>
#endif

#include "Proof/Core/SceneCoreClasses.h"
