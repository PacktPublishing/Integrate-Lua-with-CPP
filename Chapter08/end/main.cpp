#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"
#include "LuaModuleExporter.hpp"
#include "Destinations.h"

int main()
{
    auto listener = std::make_unique<LoggingLuaExecutorListener>();
    auto lua = std::make_unique<LuaExecutor>(*listener);

    auto module = LuaModuleExporter<Destinations>::make(DestinationsLuaModuleDef::def);
    lua->registerModule(module);

    lua->executeFile("script.lua");
    return 0;
}
