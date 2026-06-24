#pragma once
#include "../Professions/Profession.hpp"

class Miner : public Profession {
public:
    const std::string getProfessionName() const override { return "Miner"; }
    bool canLiveInDorm() const override { return false; }
    Profession* clone() const override { return new Miner(*this); }
    void applyMonthlyEffects(Citizen& citizen,const std::string& date) const override;

    ProfessionType getType() const override { return ProfessionType::Miner; }
};


