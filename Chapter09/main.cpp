#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"

int main()
{
    auto listener = std::make_unique<LoggingLuaExecutorListener>();
    auto lua = std::make_unique<LuaExecutor>(*listener);
    lua->executeFile("script.lua");
    return 0;
}
