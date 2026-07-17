#include "../Buildings/Building.hpp"
#include "../Utilities/BinaryUtils.hpp"

void Building::applyBuildingEffect(std::vector<Citizen*>& currentCitizens,unsigned dayOfMonth, const std::string& currentDate, unsigned distanceFromCenter,
                                   unsigned n, unsigned m)
{
    for (Citizen* citizen : currentCitizens) ///Ako nameri problemi, hvurlqme oshte predi da se promeni neshto
    {
        if (citizen == nullptr) throw std::runtime_error("Nullptr citizen");
    }

    for (Citizen* citizen : currentCitizens)
    {
        if (dayOfMonth == 1)
        {
            float currRent = getRent();
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

float Building::getRent() const
{
    return location->calculateRent(baseRent);
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
