#pragma once
#include <string>
#include <vector>

#include "LogEntry.hpp"
#include "Profession.hpp"

class Citizen {
public:
    ///big 4
    Citizen(const std::string& name, Profession* prof, unsigned happiness, unsigned money, unsigned lifePoints);
    Citizen(const Citizen& other);
    ~Citizen();
    Citizen& operator=(const Citizen& other);

    void addMoney(unsigned amount, const std::string& currentDate);
    void removeMoney(unsigned amount, const std::string& currentDate);
    void changeHappiness(int amount, const std::string& currentDate);
    void changeLifePoint(int amount, const std::string& currentDate);

    ///getters
    const std::string getName() const { return name; }
    const std::string getProfession() const { return prof->getProfessionName(); }
    unsigned getHappiness() const { return happiness; }
    unsigned getMoney() const { return money; }
    unsigned getLifePoints() const { return lifePoints; }
    const std::vector<LogEntry>& getHistory() const { return history; }

    bool canLiveInDorm() const;
    void triggerMonthlyEffects(const std::string& date);

    ///Serialization
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
private:
    void setProfession(const Profession* prof);
    void clear();
private:
    std::string name;
    Profession* prof;
    unsigned happiness;
    unsigned money;
    unsigned lifePoints;
    std::vector<LogEntry> history; ///??

};
