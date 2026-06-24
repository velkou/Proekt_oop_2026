#pragma once
#include <iostream>
#include "Building.hpp"

class Dorm : public Building{
public:
    Dorm(unsigned maxCap) :
        Building(67.0f,maxCap) {}

    void applyBuildingEffect(std::vector<Citizen*>& currentCitizens, unsigned dayOfMonth, const std::string& currentDate, unsigned distanceFromCenter,
        unsigned n, unsigned m) override;

    Building* clone() const override { return new Dorm(*this); }

    BuildingType getType() const override { return BuildingType::Dorm; }
};

inline void Dorm::applyBuildingEffect(std::vector<Citizen*>& currentCitizens, unsigned dayOfMonth, const std::string& currentDate, unsigned distanceFromCenter,
    unsigned n, unsigned m)
{
    std::vector<Citizen*> studentsOnly;
    for (Citizen* citizen : currentCitizens)
    {
        if (citizen->getProfession() == "Student")
            studentsOnly.push_back(citizen);
        else
            std::cout << citizen->getName() + " is not a student -> cannot live in a dorm!";
    }

    Building::applyBuildingEffect(studentsOnly, dayOfMonth, currentDate, distanceFromCenter, n, m);
}