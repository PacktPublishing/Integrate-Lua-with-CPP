#include "Destinations.h"
#include <iostream>

namespace
{
    const std::string NAME("Destinations");
    const std::string METATABLE_NAME("Destinations.Metatable");

    inline Destinations *getObj(lua_State *L)
    {
        luaL_checkudata(L, 1, METATABLE_NAME.c_str());
        return *reinterpret_cast<Destinations **>(lua_touserdata(L, 1));
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

    int luaNew(lua_State *L);
    const std::vector<luaL_Reg> FACTORY_REGS = {
        {"new", luaNew},
        {NULL, NULL}};

    const std::vector<luaL_Reg> REGS = {
        {"wish", luaWish},
        {"went", luaWent},
        {"list_visited", luaListVisited},
        {"list_unvisited", luaListUnvisited},
        {NULL, NULL}};

    int luaDelete(lua_State *L)
    {
        Destinations *obj = getObj(L);
        delete obj;
        return 0;
    }

    int luaNew(lua_State *L)
    {
        Destinations *obj = new Destinations();
        Destinations **userdata = reinterpret_cast<Destinations **>(lua_newuserdatauv(L, sizeof(obj), 0));
        *userdata = obj;

        int type = luaL_getmetatable(L, METATABLE_NAME.c_str());
        if (type == LUA_TNIL)
        {
            lua_pop(L, 1); // remove nil resulted from luaL_getmetatable

            luaL_newmetatable(L, METATABLE_NAME.c_str());
            lua_pushvalue(L, -1);           // copy metatable
            lua_setfield(L, -2, "__index"); // set mt.__index metatable as mt
            luaL_setfuncs(L, REGS.data(), 0);

            lua_pushcfunction(L, luaDelete);
            lua_setfield(L, -2, "__gc"); // provide a finalizer for garbage collection
        }
        lua_setmetatable(L, 1);

        return 1;
    }
}

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

const std::string &DestinationsFactory::luaName() const
{
    return NAME;
}

const std::vector<luaL_Reg> &DestinationsFactory::luaRegs() const
{
    return FACTORY_REGS;
}
