#include "../Professions/Miner.hpp"
#include "../Entities/Citizen.hpp"

void Miner::applyMonthlyEffects(Citizen& citizen) const
{
    unsigned salary = 1000 + (std::rand() % 2001);

    citizen.addMoney(salary,"End of month");
    citizen.changeLifePoint(-2,"End of month");
}