#include <iostream>
#include <lua.hpp>

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, "script.lua") || lua_pcall(L, 0, 0, 0))
    {
        std::cout << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
    lua_close(L);
    return 0;
}
