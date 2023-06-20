#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"
#include <memory>
#include <iostream>

int main()
{
    auto listener = std::make_unique<LoggingLuaExecutorListener>();
    auto lua = std::make_unique<LuaExecutor>(*listener);
    lua->executeFile("script.lua");

    auto value1 = lua->call("greetings", LuaString::make("C++"));
    std::cout << getLuaValueString(value1) << std::endl;

    auto value2 = lua->call("greetings", LuaNumber::make(3.14));
    std::cout << getLuaValueString(value2) << std::endl;

    return 0;
}
