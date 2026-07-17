#pragma once
#include "Location.hpp"

class StandartLoc : public Location {
public:
    float calculateRent(float baseRent) override { return baseRent; }
};
