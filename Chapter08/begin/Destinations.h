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

class DestinationsFactory : public LuaModule
{
public:
    const std::string &luaName() const override;
    const std::vector<luaL_Reg> &luaRegs() const override;
};

#endif // _DESTINATIONS_H
