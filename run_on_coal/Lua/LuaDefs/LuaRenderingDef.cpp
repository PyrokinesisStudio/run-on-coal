#include "stdafx.h"

#include "Lua/LuaDefs/LuaRenderingDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/RenderTarget.h"
#include "Elements/Scene.h"
#include "Elements/Shader/Shader.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"

namespace ROC
{

const std::vector<std::string> g_PolygonFillTable
{
    "point", "line", "fill"
};

}

void ROC::LuaRenderingDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "clearRenderArea", ClearRenderArea);
    lua_register(f_vm, "setClearColor", SetClearColor);
    lua_register(f_vm, "setRenderArea", SetRenderArea);
    lua_register(f_vm, "setPolygonMode", SetPolygonMode);
    lua_register(f_vm, "drawPhysics", DrawPhysics);
}

int ROC::LuaRenderingDef::ClearRenderArea(lua_State *f_vm)
{
    // bool clearRenderArea( [bool depth = true, bool color = true] )
    bool l_depth = true;
    bool l_color = true;
    ArgReader argStream(f_vm);
    argStream.ReadNextBoolean(l_depth);
    argStream.ReadNextBoolean(l_color);
    if(!argStream.HasErrors())
    {
        Core::GetCore()->GetRenderManager()->ClearRenderArea(l_depth, l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaRenderingDef::SetClearColor(lua_State *f_vm)
{
    // bool setClearColor(float colorR, float colorG, float colorB, float colorA)
    glm::vec4 l_color;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        Core::GetCore()->GetRenderManager()->SetClearColour(l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaRenderingDef::SetRenderArea(lua_State *f_vm)
{
    // bool setRenderArea(int x, int y, int width, int height)
    glm::ivec4 l_area;
    ArgReader argStream(f_vm);
    for(int i = 0; i < 4; i++) argStream.ReadInteger(l_area[i]);
    if(!argStream.HasErrors())
    {
        Core::GetCore()->GetRenderManager()->SetViewport(l_area);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaRenderingDef::SetPolygonMode(lua_State *f_vm)
{
    // bool setPolygonMode(str mode)
    std::string l_mode;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_mode);
    if(!argStream.HasErrors() && !l_mode.empty())
    {
        int l_type = EnumUtils::ReadEnumVector(l_mode, g_PolygonFillTable);
        if(l_type != -1)
        {
            Core::GetCore()->GetRenderManager()->SetPolygonMode(l_type);
            argStream.PushBoolean(true);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaRenderingDef::DrawPhysics(lua_State *f_vm)
{
    // bool drawPhysics()
    float l_width = 1.f;
    ArgReader argStream(f_vm);
    argStream.ReadNextNumber(l_width);
    Core::GetCore()->GetRenderManager()->DrawPhysics(l_width);
    argStream.PushBoolean(true);
    return argStream.GetReturnValue();
}
