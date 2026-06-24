#pragma once
#include "Building.hpp"

class ModernBuilding : public Building {
public:
    ModernBuilding(unsigned maxCap) :
        Building(1000.0f,maxCap) {}

    void applyBuildingEffect(std::vector<Citizen*>& currentCitizens, unsigned dayOfMonth, const std::string& currentDate, unsigned distanceFromCenter,
        unsigned n, unsigned m) override;

    Building* clone() const override { return new ModernBuilding(*this); }

    BuildingType getType() const override { return BuildingType::Modern; }
};

inline void ModernBuilding::applyBuildingEffect(std::vector<Citizen*>& currentCitizens, unsigned dayOfMonth, const std::string& currentDate,
    unsigned distanceFromCenter, unsigned n, unsigned m)
{
    Building::applyBuildingEffect(currentCitizens, dayOfMonth, currentDate, distanceFromCenter, n, m);
}
