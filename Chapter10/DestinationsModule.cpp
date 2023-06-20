#include "Destinations.h"
#include "LuaModuleExporter.hpp"
#include <lua.hpp>

namespace
{
    LuaModuleExporter module = LuaModuleExporter<Destinations>::make(DestinationsLuaModuleDef::def);
}

extern "C"
{
    int luaopen_destinations(lua_State *L)
    {
        lua_createtable(L, 0, module.luaRegs().size() - 1);
        int nUpvalues = module.pushLuaUpvalues(L);
        luaL_setfuncs(L, module.luaRegs().data(), nUpvalues);
        return 1;
    }
}
