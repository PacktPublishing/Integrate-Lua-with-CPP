#ifndef _LOGGING_LUA_EXECUTOR_LISTENER_H
#define _LOGGING_LUA_EXECUTOR_LISTENER_H

#include "LuaExecutor.h"

class LoggingLuaExecutorListener final : public LuaExecutorListener
{
public:
    void onExecutionError(
        const LuaExecutor &executor, const std::string &message) const override;
};

#endif // _LOGGING_LUA_EXECUTOR_LISTENER_H
