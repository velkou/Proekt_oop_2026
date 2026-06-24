#pragma once
#include "../Professions/Profession.hpp"

class Programmer : public Profession {
public:
    const std::string getProfessionName() const override { return "Programmer"; }
    bool canLiveInDorm() const override { return false; }
    Profession* clone() const override { return new Programmer(*this); }
    void applyMonthlyEffects(Citizen& citizen) const override;

    ProfessionType getType() const override { return ProfessionType::Programmer; }
};


