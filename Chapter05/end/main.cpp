#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"
#include <memory>
#include <iostream>

void dumpPositon(LuaExecutor *lua)
{
    auto x = lua->getTable("position", "x");
    auto y = lua->getTable("position", "y");
    std::cout << "x=" << std::get<LuaNumber>(x).value
              << ", y=" << std::get<LuaNumber>(y).value
              << std::endl;
}

void dumpSeq(LuaExecutor *lua)
{
    auto v1 = lua->getTable("seq", 1);
    auto v2 = lua->getTable("seq", 2);
    auto v3 = lua->getTable("seq", 3);
    std::cout << "seq={"
              << std::get<LuaNumber>(v1).value << ", "
              << std::get<LuaNumber>(v2).value << ", "
              << std::get<LuaNumber>(v3).value << "}"
              << std::endl;
}

int main()
{
    auto listener = std::make_unique<LoggingLuaExecutorListener>();
    auto lua = std::make_unique<LuaExecutor>(*listener);
    lua->executeFile("script.lua");

    std::cout << "Test table with string key:" << std::endl;
    dumpPositon(lua.get());
    lua->setTable("position", "x", LuaNumber::make(3));
    lua->setTable("position", "y", LuaNumber::make(4));
    dumpPositon(lua.get());
    std::cout << std::endl;

    std::cout << "Test table with integer index:" << std::endl;
    dumpSeq(lua.get());
    lua->setTable("seq", 1, LuaNumber::make(3));
    lua->setTable("seq", 2, LuaNumber::make(9));
    lua->setTable("seq", 3, LuaNumber::make(27));
    dumpSeq(lua.get());
    std::cout << std::endl;

    std::cout << "Test calling table functions:" << std::endl;
    std::cout << std::endl;
    lua->tcall("Destinations", "new", false, LuaString::make("dst"));
    lua->tcall("dst", "wish", true, LuaString::make("London"), LuaString::make("Paris"), LuaString::make("Amsterdam"));
    lua->tcall("dst", "went", true, LuaString::make("Paris"));
    auto visited = lua->vtcall("dst", "list_visited", true);
    auto unvisited = lua->vtcall("dst", "list_unvisited", true);
    std::cout << "Visited:" << std::endl;
    for (const auto &place : visited)
    {
        std::cout << std::get<LuaString>(place).value << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Unvisited:" << std::endl;
    for (const auto &place : unvisited)
    {
        std::cout << std::get<LuaString>(place).value << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
