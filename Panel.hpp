#pragma once
#include "Building.hpp"

class Panel : public Building {
public:
    Panel(unsigned maxCap) :
        Building(500.0f,maxCap) {}

    void applyBuildingEffect(std::vector<Citizen*>& currentCitizens, unsigned dayOfMonth, const std::string& currentDate, unsigned distanceFromCenter,
        unsigned n, unsigned m) override;

    Building* clone() const override { return new Panel(*this); }
};

inline void Panel::applyBuildingEffect(std::vector<Citizen*>& currentCitizens, unsigned dayOfMonth, const std::string& currentDate, unsigned distanceFromCenter,
    unsigned n, unsigned m)
{
    Building::applyBuildingEffect(currentCitizens,dayOfMonth, currentDate, distanceFromCenter, n, m);
}
