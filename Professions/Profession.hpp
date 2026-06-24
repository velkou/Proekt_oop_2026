#pragma once
#include <string>
#include "../Utilities/Enums.hpp"

class Citizen;
///Абстрактен базов клас. Наследниците му изчисляват ежедневните и месечните изменения на характеристиките на жителите.
class Profession
{
public:
    virtual const std::string getProfessionName() const = 0; // извеждане на професията
    virtual bool canLiveInDorm() const { return false; } // false по дефолт, но за студента е true
    virtual void applyMonthlyEffects(Citizen& citizen) const = 0; // ще променя данните за парите, живота и щастието на гражданина веднъж в месеца
    virtual Profession* clone() const = 0; // за да може лесно да се копира в контейнер
    virtual ~Profession() = default; // понеже е абстрактен базов клас

    virtual ProfessionType getType() const = 0;
};
