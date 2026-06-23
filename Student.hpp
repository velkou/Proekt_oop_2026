#pragma once
#include "Profession.hpp"

class Student : public Profession {
public:
    const std::string getProfessionName() const override { return "Student"; }
    bool canLiveInDorm() const override { return true; }
    Profession* clone() const override { return new Student(*this); }
    void applyMonthlyEffects(Citizen& citizen) const override;

    ProfessionType getType() const override { return ProfessionType::Student; }
};
