#include <iostream>
#include "LuaExecutor.h"

void demoGlobalVariable()
{
    LuaExecutor lua;
    lua.executeFile("script.lua");
    std::cout << "Lua variable whom=" << lua.getGlobalString("whom") << std::endl;
    lua.execute("hello()");
    lua.setGlobal("whom", "Lua");
    std::cout << "Lua variable whom=" << lua.getGlobalString("whom") << std::endl;
    lua.execute("hello()");
}

void demoFunctionCall()
{
    LuaExecutor lua;
    lua.executeFile("script.lua");
    std::cout << lua.call("greetings", "next adventure") << std::endl;
}

int main()
{
    demoFunctionCall();
    return 0;
}
