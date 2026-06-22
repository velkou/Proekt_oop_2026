#include "Citizen.hpp"
#include <iostream>

#include "BinaryUtils.hpp"
#include "Miner.hpp"
#include "Programmer.hpp"
#include "Student.hpp"
#include "Teacher.hpp"
#include "Unemployed.hpp"

Citizen::Citizen(const std::string& name, Profession* prof, unsigned happiness, unsigned money, unsigned lifePoints) :
    name(name), happiness(happiness), money(money), lifePoints(lifePoints)
{
    if (prof) this->prof = prof->clone();
    else this->prof = nullptr;
}

Citizen::Citizen(const Citizen& other) : name(other.name), happiness(other.happiness), money(other.money), lifePoints(other.lifePoints), history(other.history)
{
    setProfession(other.prof);
}

Citizen::~Citizen()
{
    clear();
}

Citizen& Citizen::operator=(const Citizen& other)
{
    if (this == &other) return *this;

    name = other.name;
    clear();
    setProfession(other.prof);
    happiness = other.happiness;
    money = other.money;
    lifePoints = other.lifePoints;
    history = other.history;

    return *this;
}

void Citizen::addMoney(unsigned amount, const std::string& currentDate)
{
    money += amount;
    std::string description = "Earned " + std::to_string(amount) + " money. New balance is: " + std::to_string(money); //to_string прави от инт string

    history.push_back(LogEntry{currentDate,description}); // добавябе записа в историята
}

void Citizen::removeMoney(unsigned amount, const std::string& currentDate)
{
    if (amount > money) money = 0;
    else money -= amount;

    std::string description = "Spent " + std::to_string(amount) + " money. New balance is: " + std::to_string(money); //to_string прави от инт string

    history.push_back(LogEntry{currentDate,description}); // добавябе записа в историята // тук дали да бъде push_back или emplace_back
}

void Citizen::changeHappiness(int amount, const std::string& currentDate)
{
    int newHappiness = static_cast<int>(happiness) + amount; //изчисляваме временно в инт, за да не превъртим unsigned
    if (newHappiness > 100) newHappiness = 100;
    else if (newHappiness < 0)  newHappiness = 0;

    happiness = static_cast<unsigned>(newHappiness);
    std::string direction = (amount >= 0) ? "increased by " : "decreased by ";
    std::string description = "Happiness " + direction + std::to_string(std::abs(amount)) + "." + " Current: " + std::to_string(newHappiness);

    history.push_back(LogEntry{currentDate,description});
}

void Citizen::changeLifePoint(int amount, const std::string& currentDate)
{
    int newLifePoints = static_cast<int>(lifePoints) + amount;
    if (newLifePoints > 100) newLifePoints = 100;
    else if (newLifePoints < 0) newLifePoints = 0;

    lifePoints = static_cast<unsigned>(newLifePoints);
    std::string direction = (amount >= 0) ? "increased by " : "decreased by ";
    std::string description = "Life points " + direction + std::to_string(std::abs(amount)) + "." + " Current: " + std::to_string(newLifePoints);

    history.push_back(LogEntry{currentDate,description});
}

bool Citizen::canLiveInDorm() const
{
    if (!prof) return false;
    return prof->canLiveInDorm();
}

void Citizen::triggerMonthlyEffects(const std::string& date)
{
    if (prof != nullptr)
    {
        prof->applyMonthlyEffects(*this);
        history.push_back(LogEntry{date,"Monthly effects applied"});
    }
}

void Citizen::save(std::ofstream& out) const
{
    //zapisvame prostite poleta
    BinaryUtils::writeString(out,name);
    BinaryUtils::writePrimitive(out,happiness);
    BinaryUtils::writePrimitive(out,money);
    BinaryUtils::writePrimitive(out,lifePoints);

    //zapisvame profesiqta kato string
    std::string profName = "None";
    if (prof != nullptr)
        profName = getProfession();

    BinaryUtils::writeString(out,profName);

    //zapisvame istoriqta
    size_t histSize = history.size();
    BinaryUtils::writePrimitive(out,histSize);
    for (const LogEntry& entry : history)
        entry.save(out);
}

void Citizen::load(std::ifstream& in)
{
    //chetem prostite poleta
    name = BinaryUtils::readString(in);
    BinaryUtils::readPrimitive(in,happiness);
    BinaryUtils::readPrimitive(in,money);
    BinaryUtils::readPrimitive(in,lifePoints);

    //vuzstanovqvame profesiqta
    std::string profName = BinaryUtils::readString(in);

    if (prof != nullptr)
    {
        delete prof;
        prof = nullptr;
    }

    if (profName == "Teacher") prof = new Teacher();
    else if (profName == "Programmer") prof = new Programmer();
    else if (profName == "Miner") prof = new Miner();
    else if (profName == "Student") prof = new Student();
    else if (profName == "Unemployed") prof = new Unemployed();

    //vuzstanovqvame istoriqta
    size_t histSize = 0;
    BinaryUtils::readPrimitive(in,histSize);

    history.clear(); //izchistvame tekushtata istoriq
    for (size_t i = 0; i < histSize; ++i)
    {
        LogEntry entry("",""); //suzdavame prazen obekt (podaavame prazni stringove zaradi konstruktora)
        entry.load(in);//vika load na LogEntry i si chete dannite of faila
        history.push_back(entry);
    }
}

void Citizen::setProfession(const Profession* prof)
{
    clear();
    if (prof) this->prof = prof->clone();
    else this->prof = nullptr; //za da ne sochi kum garbage adres i realno se izpolzva za da znaem dali da prilojim efekt
}

void Citizen::clear()
{
    delete prof;
}
