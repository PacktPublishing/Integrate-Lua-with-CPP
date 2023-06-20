#ifndef _LUA_MODULE_EXPORTER_HPP
#define _LUA_MODULE_EXPORTER_HPP

#include "LuaModule.h"
#include <lua.hpp>
#include <string>
#include <vector>

/// @brief Exports C++ class as Lua module.
/// @tparam T The C++ class to export.
template <typename T>
class LuaModuleExporter final : public LuaModule
{
public:
    LuaModuleExporter(const LuaModuleExporter &) = delete;
    ~LuaModuleExporter() = default;

    static LuaModuleExporter<T> make(const LuaModuleDef<T> &luaModuleDef)
    {
        return LuaModuleExporter<T>(luaModuleDef);
    }

    const std::string &luaName() const override
    {
        return luaModuleDef.moduleName;
    }

    const std::vector<luaL_Reg> &luaRegs() const override
    {
        return factoryRegs;
    }

private:
    LuaModuleExporter(const LuaModuleDef<T> &luaModuleDef) : luaModuleDef(luaModuleDef) {}

    const LuaModuleDef<T> luaModuleDef;
    const std::vector<luaL_Reg> factoryRegs = {
        {"new", luaNew},
        {NULL, NULL}};

    static LuaModuleExporter<T> *getExporter(lua_State *L)
    {
        return reinterpret_cast<LuaModuleExporter<T> *>(lua_touserdata(L, lua_upvalueindex(1)));
    }

    static int luaNew(lua_State *L)
    {
        auto exporter = getExporter(L);
        auto luaModuleDef = exporter->luaModuleDef;
        T *obj = luaModuleDef.createInstance(L);
        lua_settop(L, 0); // clear stack after instance creation to simplify index usage below

        T **userdata = reinterpret_cast<T **>(lua_newuserdatauv(L, sizeof(obj), 0));
        *userdata = obj;

        auto metatableName = luaModuleDef.metatableName();
        int type = luaL_getmetatable(L, metatableName.c_str());
        if (type == LUA_TNIL)
        {
            lua_pop(L, 1); // remove nil resulted from luaL_getmetatable

            luaL_newmetatable(L, metatableName.c_str());
            lua_pushvalue(L, -1);           // copy metatable
            lua_setfield(L, -2, "__index"); // set mt.__index metatable as mt
            luaL_setfuncs(L, luaModuleDef.moduleRegs.data(), 0);

            lua_pushlightuserdata(L, exporter);
            lua_pushcclosure(L, luaDelete, 1);
            lua_setfield(L, -2, "__gc"); // provide a finalizer for garbage collection
        }
        lua_setmetatable(L, 1);
        return 1;
    }

    static int luaDelete(lua_State *L)
    {
        auto luaModuleDef = getExporter(L)->luaModuleDef;
        T *obj = *reinterpret_cast<T **>(lua_touserdata(L, 1));
        luaModuleDef.destroyInstance(obj);
        return 0;
    }
};

#endif // _LUA_MODULE_EXPORTER_HPP
