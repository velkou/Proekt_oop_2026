#include "../Professions/Programmer.hpp"
#include "../Entities/Citizen.hpp"

void Programmer::applyMonthlyEffects(Citizen& citizen,const std::string& date) const
{
    unsigned salary = 2000 + (std::rand() % 3001);

    citizen.addMoney(salary,date);
    citizen.changeHappiness(-1,date);
}
