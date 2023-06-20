#include <iostream>
#include <lua.hpp>

int main()
{
    lua_State *L = luaL_newstate();
    std::cout << "Lua version number is " << lua_version(L) << std::endl;
    lua_close(L);
    return 0;
}
