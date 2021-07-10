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

//Proof defined
#include "Proof/Core/Log.h"
#include "Proof/Core/Core.h"
#include "Proof/Input/Input.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Core/Debug/Instrumentor.h"

#if defined(_WIN64)  // if platform is windows
	#include <windows.h>
#endif