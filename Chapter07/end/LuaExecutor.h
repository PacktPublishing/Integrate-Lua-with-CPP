#ifndef _LUA_EXECUTOR_H
#define _LUA_EXECUTOR_H

#include "LuaType.hpp"
#include "LuaModule.h"
#include <lua.hpp>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

class LuaExecutor;

class LuaExecutorListener
{
public:
    virtual void onExecutionError(
        const LuaExecutor &executor, const std::string &message) const = 0;
    virtual ~LuaExecutorListener() = default;
};

class LuaExecutor
{
public:
    LuaExecutor(const LuaExecutorListener &listener);
    virtual ~LuaExecutor();

    bool executeFile(const std::string &path);
    bool execute(const std::string &script);

    LuaValue getGlobal(const std::string &name);
    void setGlobal(const std::string &name, const LuaValue &value);

    LuaValue getTable(const std::string &table, const std::string &key);
    void setTable(const std::string &table, const std::string &key, const LuaValue &value);
    LuaValue getTable(const std::string &table, int index);
    void setTable(const std::string &table, int index, const LuaValue &value);

    LuaValue getRegistry(const LuaValue &key);
    void setRegistry(const LuaValue &key, const LuaValue &value);

    template <typename... Ts>
    LuaValue call(const std::string &function, const Ts &...params);

    template <typename... Ts>
    std::vector<LuaValue> vcall(const std::string &function, const Ts &...params);

    template <typename... Ts>
    LuaValue tcall(const std::string &table, const std::string &function,
                   bool shouldPassSelf, const Ts &...params);

    template <typename... Ts>
    std::vector<LuaValue> vtcall(const std::string &table, const std::string &function,
                                 bool shouldPassSelf, const Ts &...params);

    void registerModule(LuaModule &module);

    static const int versionCode = 7;

private:
    bool pcall(int nargs = 0, int nresults = 0);
    void pushValue(const LuaValue &value);
    LuaValue getValue(int index);
    LuaValue popValue();
    std::vector<LuaValue> popValues(int n);
    std::string popString();

    void dumpStack();

    lua_State *const L;
    const LuaExecutorListener &listener;
};

template <typename... Ts>
LuaValue LuaExecutor::call(const std::string &function, const Ts &...params)
{
    int type = lua_getglobal(L, function.c_str());
    assert(LUA_TFUNCTION == type);

    for (const auto &param : std::initializer_list<LuaValue>{params...})
    {
        pushValue(param);
    }

    pcall(sizeof...(params), 1);

    return popValue();
}

template <typename... Ts>
std::vector<LuaValue> LuaExecutor::vcall(const std::string &function, const Ts &...params)
{
    int stackSz = lua_gettop(L);

    int type = lua_getglobal(L, function.c_str());
    assert(LUA_TFUNCTION == type);

    for (const auto &param : std::initializer_list<LuaValue>{params...})
    {
        pushValue(param);
    }

    if (pcall(sizeof...(params), LUA_MULTRET))
    {
        int nresults = lua_gettop(L) - stackSz;
        return popValues(nresults);
    }
    return std::vector<LuaValue>();
}

template <typename... Ts>
LuaValue LuaExecutor::tcall(const std::string &table, const std::string &function,
                            bool shouldPassSelf, const Ts &...params)
{
    int type = lua_getglobal(L, table.c_str());
    assert(LUA_TTABLE == type);

    type = lua_getfield(L, -1, function.c_str());
    assert(LUA_TFUNCTION == type);

    if (shouldPassSelf)
    {
        lua_getglobal(L, table.c_str());
    }

    for (const auto &param : std::initializer_list<LuaValue>{params...})
    {
        pushValue(param);
    }

    int nparams = sizeof...(params) + (shouldPassSelf ? 1 : 0);
    pcall(nparams, 1);

    auto result = popValue();
    lua_pop(L, 1);

    return result;
}

template <typename... Ts>
std::vector<LuaValue> LuaExecutor::vtcall(const std::string &table, const std::string &function,
                                          bool shouldPassSelf, const Ts &...params)
{
    int type = lua_getglobal(L, table.c_str());
    assert(LUA_TTABLE == type);

    int stackSz = lua_gettop(L);

    type = lua_getfield(L, -1, function.c_str());
    assert(LUA_TFUNCTION == type);

    if (shouldPassSelf)
    {
        lua_getglobal(L, table.c_str());
    }

    for (const auto &param : std::initializer_list<LuaValue>{params...})
    {
        pushValue(param);
    }

    auto results = std::vector<LuaValue>();
    int nparams = sizeof...(params) + (shouldPassSelf ? 1 : 0);
    if (pcall(nparams, LUA_MULTRET))
    {
        int nresults = lua_gettop(L) - stackSz;
        results = popValues(nresults);
    }
    lua_pop(L, 1);
    return results;
}

#endif // _LUA_EXECUTOR_H
