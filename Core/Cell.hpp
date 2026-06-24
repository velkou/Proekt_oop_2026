/**
* \mainpage City simulation Documentation
 * \image html neo.jpeg "Welcome to the Matrix experience"
 */
#pragma once
#include "../Buildings/Building.hpp"
///Клетка от градската решетка. Съдържа една сграда и жителите в нея
class Cell {
public:
    //Голяма четворка, подсигуряваща жизнения цикъл на сграда и жители
    Cell();
    Cell(const Cell& other);
    Cell& operator=(const Cell& other);
    ~Cell();

    void addPerson(Citizen* person);
    void removePerson(const std::string& name);
    void build(Building* newBuilding);
    void triggerDailyRoutine(unsigned day, const std::string& date, unsigned dist, unsigned n, unsigned m);
    const Building* getBuilding() const;
    const std::vector<Citizen*>& getCitizens() const { return citizens; }

    void save(std::ofstream& out) const;
    void load(std::ifstream& in);

private:
    void free();
private:
    Building* building; /// ukazatel kum sgradite; moje da e nullptr
    std::vector<Citizen*> citizens; ///horata na suotvetnata kletka
};
