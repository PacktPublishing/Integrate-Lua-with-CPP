#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"
#include "Destinations.h"
#include <iostream>

int main()
{
    auto listener = std::make_unique<LoggingLuaExecutorListener>();
    auto lua = std::make_unique<LuaExecutor>(*listener);

    // Test registry
    auto key = LuaString::make("darabumba");
    lua->setRegistry(key, LuaString::make("gwentuklutar"));
    auto v1 = lua->getRegistry(key);
    lua->setRegistry(key, LuaString::make("wanghaoran"));
    auto v2 = lua->getRegistry(key);
    std::cout << getLuaValueString(key)
              << " -> " << getLuaValueString(v1)
              << " -> " << getLuaValueString(v2)
              << std::endl;
    auto regkey = LuaLightUserData::make(listener.get());
    lua->setRegistry(regkey, LuaString::make("a LuaExecutorListener implementation"));
    auto regValue = lua->getRegistry(regkey);
    std::cout << std::hex << listener.get() << " is " << getLuaValueString(regValue) << std::endl
              << std::endl;

    // Test Destinations
    auto wishlist = std::make_unique<Destinations>();
    lua->registerModule(*wishlist);
    lua->executeFile("script.lua");
    return 0;
}
