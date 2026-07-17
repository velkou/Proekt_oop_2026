#pragma once
#include "Location.hpp"

class EndLoc : public Location {
public:
    float calculateRent(float baseRent) override { return baseRent * 0.8f; }
};
