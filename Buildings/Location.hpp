#pragma once
#include <string>

class Location {
public:
    virtual ~Location() = default;
    virtual float calculateRent(float baseRent) = 0;
};
