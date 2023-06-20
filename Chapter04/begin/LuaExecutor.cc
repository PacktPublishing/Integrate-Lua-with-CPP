#include "LuaExecutor.h"

LuaExecutor::LuaExecutor(const LuaExecutorListener &listener)
    : L(luaL_newstate()), listener(listener)
{
    luaL_openlibs(L);
}

LuaExecutor::~LuaExecutor()
{
    lua_close(L);
}

bool LuaExecutor::executeFile(const std::string &path)
{
    if (luaL_loadfile(L, path.c_str()))
    {
        listener.onExecutionError(*this, popString());
        return false;
    }
    if (pcall())
    {
        return false;
    }
    return true;
}

bool LuaExecutor::execute(const std::string &script)
{
    if (luaL_loadstring(L, script.c_str()))
    {
        listener.onExecutionError(*this, popString());
        return false;
    }
    if (pcall())
    {
        return false;
    }
    return true;
}

void LuaExecutor::setGlobal(const std::string &name, const std::string &value)
{
    lua_pushstring(L, value.c_str());
    lua_setglobal(L, name.c_str());
}

std::string LuaExecutor::getGlobalString(const std::string &name)
{
    const int type = lua_getglobal(L, name.c_str());
    assert(LUA_TSTRING == type);
    return popString();
}

std::string LuaExecutor::call(const std::string &function, const std::string &param)
{
    int type = lua_getglobal(L, function.c_str());
    assert(LUA_TFUNCTION == type);
    lua_pushstring(L, param.c_str());
    pcall(1, 1);
    return popString();
}

bool LuaExecutor::pcall(int nargs, int nresults)
{
    if (lua_pcall(L, nargs, nresults, 0))
    {
        listener.onExecutionError(*this, popString());
        return false;
    }
    return true;
}

std::string LuaExecutor::popString()
{
    std::string result(lua_tostring(L, -1));
    lua_pop(L, 1);
    return result;
}
