#include "../Entities/Citizen.hpp"
#include "../Professions/Teacher.hpp"

void Teacher::applyMonthlyEffects(Citizen& citizen,const std::string& date) const
{
    unsigned salary = 1200 + (std::rand() % 101);
    citizen.addMoney(salary,date);
    citizen.changeHappiness(1,date);
}
