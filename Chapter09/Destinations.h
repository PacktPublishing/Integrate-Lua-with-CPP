#ifndef _DESTINATIONS_H
#define _DESTINATIONS_H

#include "LuaModule.h"
#include <lua.hpp>
#include <map>
#include <vector>
#include <string>

class Destinations
{
public:
    Destinations();
    ~Destinations();
    void wish(const std::vector<std::string> &places);
    void went(const std::vector<std::string> &places);
    std::vector<std::string> listVisited() const;
    std::vector<std::string> listUnvisited() const;

private:
    std::map<std::string, bool> wishlist;
};

struct DestinationsLuaModuleDef
{
    static LuaModuleDef<Destinations> def;
};

#endif // _DESTINATIONS_H
