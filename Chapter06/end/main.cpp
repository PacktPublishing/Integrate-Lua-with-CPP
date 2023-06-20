#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"
#include "Destinations.h"

int main()
{
    auto listener = std::make_unique<LoggingLuaExecutorListener>();
    auto lua = std::make_unique<LuaExecutor>(*listener);
    auto wishlist = std::make_unique<Destinations>("destinations");
    lua->registerModule(*wishlist);
    lua->executeFile("script.lua");
    return 0;
}
