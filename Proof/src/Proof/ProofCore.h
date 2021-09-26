#pragma once
#include "Proof/Core/Log.h"
#include "Proof/Core/Core.h"
#include "Proof/Input/Input.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Core/CurrentWindow.h"
#include "Proof/Input/MousePicker.h"


// Render
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/Texture.h"

// Camera
#include "Proof/Scene//Camera/OrthagraphicCamera.h"
#include "Proof/Scene//World.h"
#include "Proof/Scene/Camera/PerspectiveCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"
// scene
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Model.h"
// c++ default
#include <vector>