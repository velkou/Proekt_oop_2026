#include "Location.hpp"
class CentralLoc : public Location {
public:
    float calculateRent(float baseRent) override { return baseRent * 2.5f; }
};

