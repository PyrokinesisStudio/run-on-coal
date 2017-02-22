#pragma once
#include "Core/Core.h"
#include "Managers/ConfigManager.h"
#include "Managers/ElementManager.h"
#include "Managers/EventManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Managers/SfmlManager.h"
#include "Managers/SoundManager.h"

#include "Elements/Animation/Animation.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Model/Model.h"
#include "Elements/Camera.h"
#include "Elements/Collision.h"
#include "Elements/File.h"
#include "Elements/Font.h"
#include "Elements/Light.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader.h"
#include "Elements/Sound.h"
#include "Elements/Texture.h"

#include "Lua/ArgReader.h"
#include "Lua/LuaArguments.h"

#include "Utils/Pool.h"
#include "Utils/SystemTick.h"
#include "Utils/Utils.h"
