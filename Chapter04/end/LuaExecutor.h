#ifndef _LUA_EXECUTOR_H
#define _LUA_EXECUTOR_H

#include "LuaType.hpp"
#include <lua.hpp>
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

    void setGlobal(const std::string &name, const std::string &value);
    std::string getGlobalString(const std::string &name);

    template <typename... Ts>
    LuaValue call(const std::string &function, const Ts &...params);

    template <typename... Ts>
    std::vector<LuaValue> vcall(const std::string &function, const Ts &...params);

private:
    bool pcall(int nargs = 0, int nresults = 0);
    void pushValue(const LuaValue &value);
    LuaValue getValue(int index);
    LuaValue popValue();
    std::vector<LuaValue> popValues(int n);
    std::string popString();

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

#endif // _LUA_EXECUTOR_H
