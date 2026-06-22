#include "Programmer.hpp"
#include "Citizen.hpp"

void Programmer::applyMonthlyEffects(Citizen& citizen) const
{
    unsigned salary = 2000 + (std::rand() % 3001);

    citizen.addMoney(salary,"End of month");
    citizen.changeHappiness(-1,"End of month");
}
