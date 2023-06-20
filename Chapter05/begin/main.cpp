#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"
#include <memory>
#include <iostream>

int main()
{
    auto listener = std::make_unique<LoggingLuaExecutorListener>();
    auto lua = std::make_unique<LuaExecutor>(*listener);
    lua->executeFile("script.lua");

    std::cout << "Testing LuaExecutor::call:" << std::endl;
    auto result = lua->call("greetings", LuaString::make("C++"), LuaString::make("Lua"));
    std::cout << getLuaValueString(result) << std::endl;
    std::cout << std::endl;

    std::cout << "Testing LuaExecutor::vcall:" << std::endl;
    auto results = lua->vcall(
        "dump_params",
        LuaString::make("C++"),
        LuaString::make("Lua"),
        LuaNumber::make(3.14),
        LuaBoolean::make(true),
        LuaNil::make());
    for (const auto &result : results)
    {
        std::cout << getLuaValueString(result) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Chapter 4 - Question 1:" << std::endl;
    auto func = lua->call("get_greeter");
    std::cout << getLuaValueString(func) << std::endl;
    std::cout << std::endl;

    std::cout << "Chapter 4 - Question 2:" << std::endl;
    auto table = lua->call(
        "table_params",
        LuaString::make("C++"),
        LuaString::make("Lua"),
        LuaNumber::make(3.14),
        LuaBoolean::make(true),
        LuaNil::make());
    std::cout << getLuaValueString(table) << std::endl;
    std::cout << std::endl;

    std::cout << "Chapter 4 - Question 3:" << std::endl;
    std::cout << "Lua variable whom=" << getLuaValueString(lua->getGlobal("whom")) << std::endl;
    lua->execute("hello()");
    lua->setGlobal("whom", LuaNumber::make(1.618));
    std::cout << "Lua variable whom=" << getLuaValueString(lua->getGlobal("whom")) << std::endl;
    lua->execute("hello()");
    std::cout << std::endl;

    return 0;
}
