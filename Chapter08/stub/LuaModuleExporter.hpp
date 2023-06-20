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

    static LuaModuleExporter<T> make()
    {
        return LuaModuleExporter<T>();
    }

    const std::string &luaName() const override
    {
        return name;
    }

    const std::vector<luaL_Reg> &luaRegs() const override
    {
        return factoryRegs;
    }

private:
    LuaModuleExporter() {}

    const std::string name = "TODO";
    const std::vector<luaL_Reg> factoryRegs = {
        {"new", luaNew},
        {NULL, NULL}};

    static int luaNew(lua_State *L)
    {
        return 0;
    }
};

#endif // _LUA_MODULE_EXPORTER_HPP
