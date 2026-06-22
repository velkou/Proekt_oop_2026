#include "Citizen.hpp"
#include "Teacher.hpp"

void Teacher::applyMonthlyEffects(Citizen& citizen) const
{
    unsigned salary = 1200 + (std::rand() % 101);
    citizen.addMoney(salary,"End of month");

    citizen.changeHappiness(1,"End of month");
}
