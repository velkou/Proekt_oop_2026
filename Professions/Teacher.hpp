#pragma once
#include "../Professions/Profession.hpp"

class Teacher : public Profession {
public:
    const std::string getProfessionName() const override { return "Teacher"; }
    bool canLiveInDorm() const override { return false; }
    Profession* clone() const override { return new Teacher(*this); }
    void applyMonthlyEffects(Citizen& citizen,const std::string& date) const override;

    ProfessionType getType() const override { return ProfessionType::Teacher; }
};



