#pragma once
#include "Profession.hpp"

class Miner : public Profession {
public:
    const std::string getProfessionName() const override { return "Miner"; }
    bool canLiveInDorm() const override { return false; }
    Profession* clone() const override { return new Miner(*this); }
    void applyMonthlyEffects(Citizen& citizen) const override;

    ProfessionType getType() const override { return ProfessionType::Miner; }
};


