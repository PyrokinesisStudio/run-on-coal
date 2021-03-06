#pragma once

#define ROC_LUA_METATABLE_USERDATA "roc_ud"

namespace ROC
{

class Core;
class EventManager;
class LuaArguments;
class LuaFunction;

class LuaManager final
{
    Core *m_core;

    lua_State *m_vm;
    EventManager *m_eventManager;

    unsigned int m_pulseCycles;

    LuaManager(const LuaManager &that);
    LuaManager& operator=(const LuaManager &that);
public:
    inline EventManager* GetEventManager() const { return m_eventManager; }

    bool LoadScript(const std::string &f_script, bool f_asFile = true);
protected:
    explicit LuaManager(Core *f_core);
    ~LuaManager();

    void DoPulse();
    void CallFunction(const LuaFunction &f_func, const LuaArguments *f_args);

    friend class Core;
    friend class EventManager;
};

}
