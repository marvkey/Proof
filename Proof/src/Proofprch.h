#pragma once
#include <iostream>
#include <algorithm>
#include <utility>
#include <memory>
#include <thread>
#include <stdio.h>


//user defined
#include "Proof/Core/Log.h"
#include "Proof/Core/Core.h"
#include "Proof/Core/GUI.h"
#include "Proof/Input/KeyBoard.h"
#include "Proof/Input/Mouse.h"

// DSA
#include <vector>
#include <array>
#if defined(_WIN64)  // if platform is windows
		#include <windows.h>
#endif