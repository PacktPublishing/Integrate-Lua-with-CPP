#ifndef _LUA_EXECUTOR_H
#define _LUA_EXECUTOR_H

#include <lua.hpp>
#include <cassert>
#include <string>

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

    std::string call(const std::string &function, const std::string &param);

private:
    bool pcall(int nargs = 0, int nresults = 0);
    std::string popString();

    lua_State *const L;
    const LuaExecutorListener &listener;
};

#endif // _LUA_EXECUTOR_H
