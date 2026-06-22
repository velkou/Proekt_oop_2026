#pragma once
#include "Cell.hpp"
class CityMap {
public:
    CityMap() :
        cityName("unknown"), n(0), m(0),
        startDate("01.01.2026"),currentDate("01.01.2026"),
        day(1), month(1), year(2026) {}

    ///komandi
    void generate(unsigned rows, unsigned cols);
    void addCitizen(unsigned x, unsigned y, Citizen* citizen);
    void removeCitizen(unsigned x, unsigned y, const std::string& name);
    void step(int count = 1); //poddurja step i step <n>
    void printCityInfo() const;
    void printLocationInfo(unsigned x, unsigned y) const;
    void printCitizenInfo(unsigned x, unsigned y, const std::string& name) const;
    void printStatistics(const std::string& option) const;
    void printCityHistory() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
private:
    void performSingleStep();
    void updateCurrDateStr();
    //vrushta true ako potrebitelq iska da produlji
    bool printLine(const std::string& text, unsigned& lineCounter, unsigned maxLines = 20) const;
private:
    std::string cityName;
    unsigned n, m;
    std::string startDate;
    std::string currentDate;
    std::vector<std::vector<Cell>> grid;
    std::vector<LogEntry> cityHistory;
    unsigned day, month, year;

    std::vector<CityMap> historyOfStates; //pazi minalite sustoqniq na grada
};
