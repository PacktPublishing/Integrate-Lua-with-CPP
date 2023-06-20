#include "LuaExecutor.h"
#include "LoggingLuaExecutorListener.h"
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

auto listener = std::make_unique<LoggingLuaExecutorListener>();
std::mutex coutMutex;

void threadFunc(int threadNo, int a, int b, int c)
{
    auto lua = std::make_unique<LuaExecutor>(*listener);
    lua->execute("function add_params(a, b, c) return a + b + c end");
    auto result = lua->call("add_params", LuaNumber::make(a), LuaNumber::make(b), LuaNumber::make(c));

    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[Thread " << threadNo << "] "
              << a << "+" << b << "+" << c << "="
              << std::get<LuaNumber>(result).value << std::endl;
}

// Each C++ thread creates its own LuaExecutor instance.
int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; i++)
    {
        int a = i * 3 + 1;
        // Constructs at the end of the vector with:
        // std::thread t(threadFunc, i, a, a + 1, a + 2);
        threads.emplace_back(threadFunc, i + 1, a, a + 1, a + 2);
    }
    for (auto &t : threads)
    {
        t.join();
    }
    return 0;
}
