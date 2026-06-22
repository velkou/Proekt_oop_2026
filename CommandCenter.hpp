#pragma once
#include "CityMap.hpp"

class CommandCenter {
public:
    void run();
private:
    //pomoshtna funkciq za izchistvane na potoka pri greshni danni
    void clearInputStream();
    Profession* createProfessionFromString(const std::string& str);
    //vrushta true ako potrebitelq iska da produlji
private:
    CityMap map;
};
