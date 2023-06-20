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

bool LuaExecutor::pcall(int nargs, int nresults)
{
    if (lua_pcall(L, nargs, nresults, 0))
    {
        listener.onExecutionError(*this, popString());
        return false;
    }
    return true;
}

void LuaExecutor::pushValue(const LuaValue &value)
{
    switch (getLuaType(value))
    {
    case LuaType::nil:
        lua_pushnil(L);
        break;
    case LuaType::boolean:
        lua_pushboolean(L, std::get<LuaBoolean>(value).value ? 1 : 0);
        break;
    case LuaType::number:
        lua_pushnumber(L, std::get<LuaNumber>(value).value);
        break;
    case LuaType::string:
        lua_pushstring(L, std::get<LuaString>(value).value.c_str());
        break;
    }
}

LuaValue LuaExecutor::getValue(int index)
{
    switch (lua_type(L, index))
    {
    case LUA_TNIL:
        return LuaNil::make();
    case LUA_TBOOLEAN:
        return LuaBoolean::make(lua_toboolean(L, index) == 1);
    case LUA_TNUMBER:
        return LuaNumber::make((double)lua_tonumber(L, index));
    case LUA_TSTRING:
        return LuaString::make(lua_tostring(L, index));
    default:
        return LuaNil::make();
    }
}

LuaValue LuaExecutor::popValue()
{
    auto value = getValue(-1);
    lua_pop(L, 1);
    return value;
}

std::vector<LuaValue> LuaExecutor::popValues(int n)
{
    std::vector<LuaValue> results;
    for (int i = n; i > 0; --i)
    {
        results.push_back(getValue(-i));
    }
    lua_pop(L, n);
    return results;
}

std::string LuaExecutor::popString()
{
    auto result = std::get<LuaString>(popValue());
    return result.value;
}
