#pragma once
#include "../Entities/Citizen.hpp"
#include "../Utilities/Enums.hpp"
///Абстрактен базов клас. Наследниците му изчисляват специфичните за тях данни
class Building {
public:
    Building(float baseRent, unsigned maxCap) :
        baseRent(baseRent), maxCap(maxCap) {}
    virtual ~Building() = default;

    float getBaseRent() const { return baseRent; }

    virtual void applyBuildingEffect(std::vector<Citizen*>& currentCitizens,unsigned dayOfMonth, const std::string& currentDate,
            unsigned distanceFromCenter, unsigned n, unsigned m);

    float getRent(unsigned distanceFromCenter, unsigned n, unsigned m) const;

    unsigned getMaxCap() const { return maxCap; }

    virtual Building* clone() const = 0;

    virtual BuildingType getType() const = 0;

    //virtualni zashtoto tipovete sgradi sa polimorfni tipove i zashtoto kato zarejdame faila
    //programata trqbva da znae dali tezi chisla prinadlejat na panelka ili obshtak
    virtual void save(std::ofstream& out) const;
    virtual void load(std::ifstream& in);
protected:
    float baseRent;
    unsigned maxCap;
    ///std::vector<Citizen*> listOfCitizens;
};
