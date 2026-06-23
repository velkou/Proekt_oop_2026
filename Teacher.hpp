#pragma once
#include "Profession.hpp"

class Teacher : public Profession {
public:
    const std::string getProfessionName() const override { return "Teacher"; }
    bool canLiveInDorm() const override { return false; }
    Profession* clone() const override { return new Teacher(*this); }
    void applyMonthlyEffects(Citizen& citizen) const override;

    ProfessionType getType() const override { return ProfessionType::Teacher; }
};



