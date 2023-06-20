#include "Destinations.h"
#include <iostream>

namespace
{
    inline Destinations *getObj(lua_State *L)
    {
        luaL_checkudata(L, 1, DestinationsLuaModuleDef::def.metatableName().c_str());
        if (DestinationsLuaModuleDef::def.isManagingMemory)
        {
            return *reinterpret_cast<Destinations **>(lua_touserdata(L, 1));
        }
        else
        {
            return reinterpret_cast<Destinations *>(lua_touserdata(L, 1));
        }
    }

    int luaWish(lua_State *L)
    {
        Destinations *obj = getObj(L);
        std::vector<std::string> places;
        int nArgs = lua_gettop(L);
        for (int i = 2; i <= nArgs; i++)
        {
            places.push_back(lua_tostring(L, i));
        }
        obj->wish(places);
        return 0;
    }

    int luaWent(lua_State *L)
    {
        Destinations *obj = getObj(L);
        std::vector<std::string> places;
        int nArgs = lua_gettop(L);
        for (int i = 2; i <= nArgs; i++)
        {
            places.push_back(lua_tostring(L, i));
        }
        obj->went(places);
        return 0;
    }

    int luaListVisited(lua_State *L)
    {
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
        Destinations *obj = getObj(L);
        auto places = obj->listUnvisited();
        for (const auto &place : places)
        {
            lua_pushstring(L, place.c_str());
        }
        return places.size();
    }
}

LuaModuleDef DestinationsLuaModuleDef::def = LuaModuleDef<Destinations>{
    "Destinations",
    {{"wish", luaWish},
     {"went", luaWent},
     {"list_visited", luaListVisited},
     {"list_unvisited", luaListUnvisited},
     {NULL, NULL}},
    false,
};

Destinations::Destinations() : wishlist({})
{
    std::cout << "Destinations instance created: " << std::hex << this << std::endl;
}

Destinations::~Destinations()
{
    std::cout << "Destinations instance destroyed: " << std::hex << this << std::endl;
}

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
