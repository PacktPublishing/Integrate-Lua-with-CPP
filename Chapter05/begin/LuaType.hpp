#ifndef _LUA_TYPE_HPP
#define _LUA_TYPE_HPP

#include <cstddef>
#include <string>
#include <variant>

enum class LuaType
{
    nil,
    boolean,
    number,
    string,
    function,
    table,
};

struct LuaNil final
{
    const LuaType type = LuaType::nil;
    const std::nullptr_t value = nullptr;

    static LuaNil make() { return LuaNil(); }

private:
    LuaNil() = default;
};

struct LuaBoolean final
{
    const LuaType type = LuaType::boolean;
    const bool value;

    static LuaBoolean make(const bool value) { return LuaBoolean(value); }

private:
    LuaBoolean(const bool value) : value(value) {}
};

struct LuaNumber final
{
    const LuaType type = LuaType::number;
    const double value;

    static LuaNumber make(const double value) { return LuaNumber(value); }

private:
    LuaNumber(const double value) : value(value) {}
};

struct LuaString final
{
    const LuaType type = LuaType::string;
    const std::string value;

    static LuaString make(const std::string &value) { return LuaString(value); }

private:
    LuaString(const std::string &value) : value(value) {}
};

struct LuaFunction final
{
    const LuaType type = LuaType::function;
    const std::nullptr_t value = nullptr;

    static LuaFunction make() { return LuaFunction(); }

private:
    LuaFunction() = default;
};

struct LuaTable final
{
    const LuaType type = LuaType::table;
    const std::nullptr_t value = nullptr;

    static LuaTable make() { return LuaTable(); }

private:
    LuaTable() = default;
};

using LuaValue = std::variant<LuaNil, LuaBoolean, LuaNumber, LuaString, LuaFunction, LuaTable>;

inline LuaType getLuaType(const LuaValue &value)
{
    return std::visit(
        [](const auto &v)
        { return v.type; },
        value);
}

inline std::string getLuaValueString(const LuaValue &value)
{
    switch (getLuaType(value))
    {
    case LuaType::nil:
        return "nil";
    case LuaType::boolean:
        return std::get<LuaBoolean>(value).value ? "true" : "false";
    case LuaType::number:
        return std::to_string(std::get<LuaNumber>(value).value);
    case LuaType::string:
        return std::get<LuaString>(value).value;
    case LuaType::function:
        return "function";
    case LuaType::table:
        return "table";
    }
}

#endif // _LUA_TYPE_HPP
