#include "Unemployed.hpp"
#include "Citizen.hpp"

void Unemployed::applyMonthlyEffects(Citizen& citizen) const
{
    citizen.changeLifePoint(-1,"End of month");
    citizen.changeHappiness(-1,"End of month");
}
