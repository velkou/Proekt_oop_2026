#include "../Professions/Miner.hpp"
#include "../Entities/Citizen.hpp"

void Miner::applyMonthlyEffects(Citizen& citizen,const std::string& date) const
{
    unsigned salary = 1000 + (std::rand() % 2001);

    citizen.addMoney(salary,date);
    citizen.changeLifePoint(-2,date);
}