#include "Destinations.h"

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
