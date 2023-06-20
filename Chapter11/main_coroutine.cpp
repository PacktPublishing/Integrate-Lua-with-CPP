#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"
#include <iostream>

int main()
{
    auto listener = std::make_unique<LoggingLuaExecutorListener>();
    auto lua = std::make_unique<LuaExecutor>(*listener);
    lua->executeFile("script.lua");
    auto result = lua->resume("squares");
    if (getLuaType(result) == LuaType::number)
    {
        std::cout << "Coroutine yields " << std::get<LuaNumber>(result).value << std::endl;
    }
    return 0;
}
