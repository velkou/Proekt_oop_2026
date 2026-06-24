#include "../Professions/Student.hpp"
#include "../Entities/Citizen.hpp"

void Student::applyMonthlyEffects(Citizen& citizen,const std::string& date) const
{
    citizen.changeHappiness(-1,date);
}
