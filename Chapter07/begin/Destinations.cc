#include "Destinations.h"
#include <iostream>

namespace
{
    inline Destinations *getObj(lua_State *L)
    {
        return reinterpret_cast<Destinations *>(lua_touserdata(L, lua_upvalueindex(1)));
    }

    inline int increaseCounterAndGet(lua_State *L)
    {
        int count = lua_tointeger(L, lua_upvalueindex(2)) + 1;
        lua_pushinteger(L, count);
        lua_copy(L, -1, lua_upvalueindex(2));
        lua_pop(L, 1);
        return count;
    }

    int luaWish(lua_State *L)
    {
        std::cout << "[Destinations] luaWish called " << increaseCounterAndGet(L) << " times" << std::endl;

        Destinations *obj = getObj(L);
        std::vector<std::string> places;
        int nArgs = lua_gettop(L);
        for (int i = 1; i <= nArgs; i++)
        {
            places.push_back(lua_tostring(L, i));
        }
        obj->wish(places);
        return 0;
    }

    int luaWent(lua_State *L)
    {
        std::cout << "[Destinations] luaWent called " << increaseCounterAndGet(L) << " times" << std::endl;

        Destinations *obj = getObj(L);
        std::vector<std::string> places;
        int nArgs = lua_gettop(L);
        for (int i = 1; i <= nArgs; i++)
        {
            places.push_back(lua_tostring(L, i));
        }
        obj->went(places);
        return 0;
    }

    int luaListVisited(lua_State *L)
    {
        std::cout << "[Destinations] luaListVisited called " << increaseCounterAndGet(L) << " times" << std::endl;

        Destinations *obj = getObj(L);
        auto places = obj->listVisited();
        for (const auto &place : places)
        {
            lua_pushstring(L, place.c_str());
        }
        return places.size();
    }

    int luaListUnvisited(lua_State *L)
    {
        std::cout << "[Destinations] luaListUnvisited called " << increaseCounterAndGet(L) << " times" << std::endl;

        Destinations *obj = getObj(L);
        auto places = obj->listUnvisited();
        for (const auto &place : places)
        {
            lua_pushstring(L, place.c_str());
        }
        return places.size();
    }

    const std::vector<luaL_Reg> REGS = {
        {"wish", luaWish},
        {"went", luaWent},
        {"list_visited", luaListVisited},
        {"list_unvisited", luaListUnvisited},
        {NULL, NULL}};
}

Destinations::Destinations(const std::string &name) : name(name), wishlist({}) {}

void Destinations::wish(const std::vector<std::string> &places)
{
    for (const auto &place : places)
    {
        wishlist[place] = false;
    }
}

void Destinations::went(const std::vector<std::string> &places)
{
    for (const auto &place : places)
    {
        wishlist[place] = true;
    }
}

std::vector<std::string> Destinations::listVisited() const
{
    std::vector<std::string> results;
    for (const auto &[place, visited] : wishlist)
    {
        if (visited)
        {
            results.push_back(place);
        }
    }
    return results;
}

std::vector<std::string> Destinations::listUnvisited() const
{
    std::vector<std::string> results;
    for (const auto &[place, visited] : wishlist)
    {
        if (not visited)
        {
            results.push_back(place);
        }
    }
    return results;
}

const std::string &Destinations::luaName() const
{
    return name;
}

const std::vector<luaL_Reg> &Destinations::luaRegs() const
{
    return REGS;
}

int Destinations::pushLuaUpvalues(lua_State *L)
{
    int nUpvalues = LuaModule::pushLuaUpvalues(L);
    lua_pushinteger(L, 0); // an integer counter
    return nUpvalues + 1;
}
