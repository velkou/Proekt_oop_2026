#pragma once
#include "Profession.hpp"

class Unemployed : public Profession {
public:
    const std::string getProfessionName() const override { return "Unemployed"; }
    bool canLiveInDorm() const override { return false; }
    Profession* clone() const override { return new Unemployed(*this); }
    void applyMonthlyEffects(Citizen& citizen) const override;

    ProfessionType getType() const override { return ProfessionType::Unemployed; }
};
