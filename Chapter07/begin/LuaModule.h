#ifndef _LUA_MODULE_H
#define _LUA_MODULE_H

#include <lua.hpp>
#include <string>
#include <vector>

class LuaModule
{
public:
    /// @brief Name of the table.
    virtual const std::string &luaName() const = 0;

    /// @brief Lua functions for the module. End with {NULL, NULL}.
    virtual const std::vector<luaL_Reg> &luaRegs() const = 0;

    /// @brief Called to push module shared upvalues.
    /// @return The number of upvalues pushed.
    virtual int pushLuaUpvalues(lua_State *L)
    {
        lua_pushlightuserdata(L, this);
        return 1;
    }

    virtual ~LuaModule() = default;
};

#endif // _LUA_MODULE_H
