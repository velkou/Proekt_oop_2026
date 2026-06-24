#pragma once
#include <string>
#include "../Utilities/Enums.hpp"

class Citizen;
///Абстрактен базов клас. Наследниците му изчисляват ежедневните и месечните изменения на характеристиките на жителите.
class Profession
{
public:
    ///Използваме функцията за отпечатването на екрана
    virtual const std::string getProfessionName() const = 0; /// извеждане на професията
    virtual bool canLiveInDorm() const { return false; } /// false по дефолт, но за студента е true
    virtual void applyMonthlyEffects(Citizen& citizen,const std::string& date) const = 0; /// ще променя данните за парите, живота и щастието на гражданина веднъж в месеца
    virtual Profession* clone() const = 0; /// за да може лесно да се копира в контейнер
    virtual ~Profession() = default; /// понеже е абстрактен базов клас

    ///Вместо да записва във файлове дълги стрингове (които в нашия случай могат да са повече от 4 байта) тази функция помага за писане само на 4 байта
    virtual ProfessionType getType() const = 0;
};
