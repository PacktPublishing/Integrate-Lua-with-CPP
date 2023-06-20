#include "LoggingLuaExecutorListener.h"
#include <iostream>

void LoggingLuaExecutorListener::onExecutionError(
    const LuaExecutor &executor, const std::string &message) const
{
    std::cerr << "[LuaExecutor] Failed to execute: " << message << std::endl;
}
