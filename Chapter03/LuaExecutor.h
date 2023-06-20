#ifndef _LUA_EXECUTOR_H
#define _LUA_EXECUTOR_H

#include <lua.hpp>
#include <string>

class LuaExecutor
{
public:
    LuaExecutor();
    virtual ~LuaExecutor();

    void executeFile(const std::string &path);
    void execute(const std::string &script);

    void setGlobal(const std::string &name, const std::string &value);
    std::string getGlobalString(const std::string &name);

    std::string call(const std::string &function, const std::string &param);

private:
    void pcall(int nargs = 0, int nresults = 0);
    std::string popString();

    lua_State *const L;
};

#endif // _LUA_EXECUTOR_H
