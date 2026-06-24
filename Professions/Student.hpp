#pragma once
#include "../Professions/Profession.hpp"

class Student : public Profession {
public:
    const std::string getProfessionName() const override { return "Student"; }
    bool canLiveInDorm() const override { return true; }
    Profession* clone() const override { return new Student(*this); }
    void applyMonthlyEffects(Citizen& citizen,const std::string& date) const override;

    ProfessionType getType() const override { return ProfessionType::Student; }
};
