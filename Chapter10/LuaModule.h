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

    /// @brief Indicates if C++ (true) or Lua (false) should manage the C++ instance memory.
    const bool isManagingMemory;

    /// @brief Creates a new instance returns the object pointer.
    /// If isManagingMemory is false, the memory for an instance of T is allocated by Lua,
    // and you should use placement new to create the instance.
    const std::function<T *(lua_State *, void *)> createInstance = [this](lua_State *, void *addr) -> T *
    {
        if (isManagingMemory)
        {
            return new T();
        }
        else
        {
            return new (addr) T();
        }
    };

    /// @brief Destroys the provided instance.
    /// If isManagingMemory is false, the memory for an instance of T is deallocated by Lua,
    /// and you should just call ~T().
    const std::function<void(T *)> destroyInstance = [this](T *obj)
    {
        if (isManagingMemory)
        {
            delete obj;
        }
        else
        {
            obj->~T();
        }
    };

    const std::string metatableName() const
    {
        return std::string(moduleName).append(".Metatable");
    }
};

#endif // _LUA_MODULE_H
