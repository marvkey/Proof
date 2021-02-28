#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <thread>
#include <stdio.h>
#include <array>


//user defined
#include "Proof/Core/Log.h"
#include "Proof/Core/Core.h"
#include "Proof/Core/GUI.h"
#include "Proof/Input/Input.h"

#if defined(_WIN64)  // if platform is windows
		#include <windows.h>
#endif