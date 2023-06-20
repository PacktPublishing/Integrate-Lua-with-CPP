#ifndef _LUA_MODULE_H
#define _LUA_MODULE_H

#include <lua.hpp>
#include <string>
#include <vector>

/// @brief Represents a Lua C++ module.
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

/// @brief Represents a Lua C++ module definition.
template <typename T>
struct LuaModuleDef
{
    const std::string moduleName;
    const std::vector<luaL_Reg> moduleRegs;

    /// @brief Creates a new instance returns the object pointer.
    const std::function<T *(lua_State *)> createInstance = [](lua_State *) -> T *
    { return new T(); };

    /// @brief Destroys the provided instance.
    const std::function<void(T *)> destroyInstance = [](T *obj)
    { delete obj; };

    const std::string metatableName() const
    {
        return std::string(moduleName).append(".Metatable");
    }
};

#endif // _LUA_MODULE_H
