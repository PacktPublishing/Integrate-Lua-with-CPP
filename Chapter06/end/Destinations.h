#ifndef _DESTINATIONS_H
#define _DESTINATIONS_H

#include "LuaModule.h"
#include <lua.hpp>
#include <map>
#include <vector>
#include <string>

class Destinations : public LuaModule
{
public:
    Destinations(const std::string &name);
    void wish(const std::vector<std::string> &places);
    void went(const std::vector<std::string> &places);
    std::vector<std::string> listVisited() const;
    std::vector<std::string> listUnvisited() const;

    const std::string &luaName() const override;
    const std::vector<luaL_Reg> &luaRegs() const override;

private:
    std::string name;
    std::map<std::string, bool> wishlist;
};

#endif // _DESTINATIONS_H
