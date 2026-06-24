#include "../Core/Cell.hpp"
#include <iostream>
#include "../Utilities/BinaryUtils.hpp"
#include "../Buildings/Dorm.hpp"
#include "../Buildings/ModernBuilding.hpp"
#include "../Buildings/Panel.hpp"

Cell::Cell() : building(nullptr) {} ///vektorut citizens sam se inicializira prazen

Cell::Cell(const Cell& other)
{
    if (other.building != nullptr)
        building = other.building->clone();
    else
        building = nullptr;

    for (Citizen* citizen : other.citizens)
        this->citizens.push_back(new Citizen(*citizen));
    ///заделя се нова памет.Подавайки гражданина от other в скобите, извикваме копи конструктора на Citizen
}

Cell& Cell::operator=(const Cell& other)
{
    if (this == &other) return *this;

    Cell temp(other);
    std::swap(building,temp.building);
    std::swap(citizens,temp.citizens);

    return *this;
}

Cell::~Cell()
{
    free();
}

void Cell::addPerson(Citizen* person)
{
    if (person == nullptr) throw std::invalid_argument("A person cannot be nullptr");

    citizens.push_back(person);
}

void Cell::removePerson(const std::string& name)
{
    for (auto it = citizens.begin(); it != citizens.end();++it)
    {
        if (name == (*it)->getName())
        {
            ///it ne e kopie a e pointer ot samiq vektor; purvonachalnata ideq beshe da izpolzvam erase-remove idiom no ne stana zaradi kopieto
            delete *it;
            citizens.erase(it); //vsichko shte se mrudne s 1 nalqvo
            return;
        }
    }
    throw std::runtime_error("No such citizen exists at this location!");
}

void Cell::build(Building* newBuilding)
{
    if (building == nullptr)
    {
        building = newBuilding;
        std::cout << "Building successful! " ;
    }
    else
        throw std::runtime_error("Cannot build over an existing building!");
}

const Building* Cell::getBuilding() const
{
    return building;
}

void Cell::save(std::ofstream& out) const
{
    //zapazvane na sgradata
    if (building == nullptr)
        BinaryUtils::writePrimitive(out,BuildingType::None);
    else
    {
        // direktno zapisvame enuma kato chislo, koeto e po memory eficient, zashtoto sega enuma e samo 4 byte-a
        BinaryUtils::writePrimitive(out,building->getType());
        building->save(out); //sled tova samata sgradata si zapisva chislata
    }

    //zapazvane na grajdanite
    size_t citCount = citizens.size();
    BinaryUtils::writePrimitive(out,citCount); //zapisvame broikata hora

    for (const Citizen* c : citizens)
        c->save(out); //vseki grajdanin se zapisva sam
}

void Cell::load(std::ifstream& in)
{
    //zarejdane na sgrada
    BuildingType type;
    BinaryUtils::readPrimitive(in,type);

    //chistim starata sgrada, ako prezapisvame kletkata
    if (building != nullptr)
    {
        delete building;
        building = nullptr;
    }

    if (type != BuildingType::None)
    {
        switch (type)
        {
            case BuildingType::Modern: building = new ModernBuilding(0); break;
            case BuildingType::Panel: building = new Panel(0); break;
            case BuildingType::Dorm: building = new Dorm(0); break;
            default:
                throw std::runtime_error("Corrupt file: Invalid building type!");
        }
        //tuk sme sigurni che building sochi kum sushtestvuvashta sgrada
        building->load(in);
    }

    //zarejdane na grajdanite
    size_t citCount = 0;
    BinaryUtils::readPrimitive(in,citCount);

    //chistim starite grajdani
    for (Citizen* c : citizens)
        delete c;
    citizens.clear(); //trie vsichki elementi vuv vektor

    //suzdavame novite
    for (size_t i = 0; i < citCount; ++i)
    {
        Citizen* newCit = new Citizen("",nullptr,0,0,0);
        newCit->load(in);
        citizens.push_back(newCit);
    }
}

void Cell::triggerDailyRoutine(unsigned day, const std::string& date, unsigned dist, unsigned n, unsigned m)
{
    if (building != nullptr)
    {
        building->applyBuildingEffect(citizens,day,date,dist,n,m);
    }
    else
    {
        for (Citizen* citizen : citizens)
        {
           citizen->changeHappiness(-10,date);
            citizen->changeLifePoint(-5,date);

            if (citizen->getMoney() >= 50)
            {
                citizen->removeMoney(50,date);
            }
            else
            {
                citizen->removeMoney(50,date);
                //nakazanie za glad
                citizen->changeHappiness(-15,date);
                citizen->changeLifePoint(-20,date);
            }
        }
    }
}

void Cell::free()
{
    delete building;
    for (Citizen* citizen : citizens)
    {
        delete citizen;
    }
    citizens.clear();
}