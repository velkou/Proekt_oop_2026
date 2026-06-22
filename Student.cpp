#include "Student.hpp"
#include "Citizen.hpp"

void Student::applyMonthlyEffects(Citizen& citizen) const
{
    citizen.changeHappiness(-1,"End of month");
}
