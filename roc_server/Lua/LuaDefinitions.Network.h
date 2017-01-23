#pragma once

namespace ROC
{
namespace Lua
{

int networkDisconnectClient(lua_State *f_vm);
int networkSendDataToClient(lua_State *f_vm);
int networkGetClientID(lua_State *f_vm);
int networkGetClientAddress(lua_State *f_vm);
int networkGetClientPing(lua_State *f_vm);

}
}
