#include "../Professions/Unemployed.hpp"
#include "../Entities/Citizen.hpp"

void Unemployed::applyMonthlyEffects(Citizen& citizen,const std::string& date) const
{
    citizen.changeLifePoint(-1,date);
    citizen.changeHappiness(-1,date);
}
