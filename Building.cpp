#include "Building.hpp"

#include "BinaryUtils.hpp"

void Building::applyBuildingEffect(std::vector<Citizen*>& currentCitizens,unsigned dayOfMonth, const std::string& currentDate, unsigned distanceFromCenter,
                                   unsigned n, unsigned m)
{
    for (Citizen* citizen : currentCitizens) ///Ako nameri problemi, hvurlqme oshte predi da se promeni neshto
    {
        if (citizen == nullptr) throw std::runtime_error("Nullptr citizen");
    }
    /// ne znam vse oshte dali e hubavo zaradi bavene
    for (Citizen* citizen : currentCitizens)
    {
        if (dayOfMonth == 1)
        {
            float currRent = getRent(distanceFromCenter,n,m);
            if (citizen->getMoney() >= currRent)
            {
                citizen->removeMoney(currRent,currentDate);
            }
            else
            {
                citizen->removeMoney(currRent,currentDate);
                citizen->changeHappiness(-20,currentDate);
                citizen->changeLifePoint(-15,currentDate);
            }
        }

        if (citizen->getMoney() >= 50)
        {
            citizen->removeMoney(50,currentDate);
        }
        else
        {
            citizen->removeMoney(50,currentDate);
            citizen->changeHappiness(-25,currentDate);
            citizen->changeLifePoint(-30,currentDate);
        }
    }
}

float Building::getRent(unsigned distanceFromCenter, unsigned n, unsigned m) const
{
    float limitCentral = std::min(n,m)/8.0f;
    float limitEdge = 6 * std::min(n,m)/8.0f;

    if (distanceFromCenter <= limitCentral)
        return baseRent * 2.5;

    if (distanceFromCenter > limitEdge)
        return baseRent * 0.8;

    return baseRent;
}

void Building::save(std::ofstream& out) const
{
    BinaryUtils::writePrimitive(out,baseRent);
    BinaryUtils::writePrimitive(out,maxCap);
}

void Building::load(std::ifstream& in)
{
    BinaryUtils::readPrimitive(in,baseRent);
    BinaryUtils::readPrimitive(in,maxCap);
}
