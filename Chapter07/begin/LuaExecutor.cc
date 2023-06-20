#include "LuaExecutor.h"
#include <iostream>

namespace
{
    int luaGetExecutorVersionCode(lua_State *L)
    {
        lua_pushinteger(L, LuaExecutor::versionCode);
        return 1;
    }

    void registerHostFunctions(lua_State *L)
    {
        lua_pushcfunction(L, luaGetExecutorVersionCode);
        lua_setglobal(L, "host_version");
    }
}

namespace
{
    int luaPrintOverride(lua_State *L)
    {
        int nArgs = lua_gettop(L);
        std::cout << "[Lua]";
        for (int i = 1; i <= nArgs; i++)
        {
            std::cout << " " << luaL_tolstring(L, i, NULL);
        }
        std::cout << std::endl;
        return 0;
    }

    void overrideLuaFunctions(lua_State *L)
    {
        const struct luaL_Reg overrides[] = {
            {"print", luaPrintOverride},
            {NULL, NULL}};

        lua_getglobal(L, "_G");
        luaL_setfuncs(L, overrides, 0);
        lua_pop(L, 1);
    }
}

LuaExecutor::LuaExecutor(const LuaExecutorListener &listener)
    : L(luaL_newstate()), listener(listener)
{
    luaL_openlibs(L);

    registerHostFunctions(L);
    overrideLuaFunctions(L);
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

LuaValue LuaExecutor::getGlobal(const std::string &name)
{
    lua_getglobal(L, name.c_str());
    return popValue();
}

void LuaExecutor::setGlobal(const std::string &name, const LuaValue &value)
{
    pushValue(value);
    lua_setglobal(L, name.c_str());
}

LuaValue LuaExecutor::getTable(const std::string &table, const std::string &key)
{
    int type = lua_getglobal(L, table.c_str());
    assert(LUA_TTABLE == type);
    lua_getfield(L, -1, key.c_str());
    auto value = popValue();
    lua_pop(L, 1);
    return value;
}

void LuaExecutor::setTable(const std::string &table, const std::string &key, const LuaValue &value)
{
    int type = lua_getglobal(L, table.c_str());
    assert(LUA_TTABLE == type);
    pushValue(value);
    lua_setfield(L, -2, key.c_str());
    lua_pop(L, 1);
}

LuaValue LuaExecutor::getTable(const std::string &table, int index)
{
    int type = lua_getglobal(L, table.c_str());
    assert(LUA_TTABLE == type);
    lua_geti(L, -1, index);
    auto value = popValue();
    lua_pop(L, 1);
    return value;
}

void LuaExecutor::setTable(const std::string &table, int index, const LuaValue &value)
{
    int type = lua_getglobal(L, table.c_str());
    assert(LUA_TTABLE == type);
    pushValue(value);
    lua_seti(L, -2, index);
    lua_pop(L, 1);
}

void LuaExecutor::registerModule(LuaModule &module)
{
    lua_createtable(L, 0, module.luaRegs().size() - 1);
    int nUpvalues = module.pushLuaUpvalues(L);
    luaL_setfuncs(L, module.luaRegs().data(), nUpvalues);
    lua_setglobal(L, module.luaName().c_str());
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
    case LuaType::function:
        assert(false && "pushing a Lua function is not supported");
        break;
    case LuaType::table:
        assert(false && "pushing a Lua table is not supported");
        break;
    case LuaType::lightuserdata:
        lua_pushlightuserdata(L, std::get<LuaLightUserData>(value).value);
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
    case LUA_TFUNCTION:
        return LuaFunction::make();
    case LUA_TTABLE:
        return LuaTable::make();
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

void LuaExecutor::dumpStack()
{
    std::cout << "The Lua Stack:" << std::endl;
    for (int i = lua_gettop(L); i >= 1; i--)
    {
        std::cout << getLuaValueString(getValue(i))
                  << " [" << lua_typename(L, lua_type(L, i)) << "]"
                  << std::endl;
    }
}
