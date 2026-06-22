#include "CommandCenter.hpp"
#include <iostream>
#include "Miner.hpp"
#include "Programmer.hpp"
#include "Student.hpp"
#include "Teacher.hpp"
#include "Unemployed.hpp"

void CommandCenter::run()
{
    std::cout << "The Matrix is a system, Neo. Enter a command from the list below to start your Matrix experience ^&^" << std::endl;
    std::cout << "generate; "
              << "add; "
              << "remove; "
              << "step; "
              << "info; " << std::endl;

    std::string command;

    while (std::cin >> command)
    {
        try
        {
            if (command == "generate")
            {
                unsigned n, m;
                std::cin >> n >> m;

                if (n == 0 || m == 0)
                    throw std::invalid_argument("City dimensions must be at least 1x1!");

                if (std::cin.fail())
                {
                    clearInputStream();
                    throw std::invalid_argument("Coordinates must be positive numbers!");
                }
                map.generate(n, m);
            }
            else if (command == "add")
            {
                unsigned x,y,happiness,money,life;
                std::string name, job;
                std::cin >> x >> y >> name >> job >> happiness >> money >> life;

                if (std::cin.fail())
                {
                    clearInputStream();
                    throw std::invalid_argument("Invalid input formats of add command!");
                }

                //validacii
                if (happiness > 100 || life > 100)
                    throw std::invalid_argument("Happiness and life points must be between 0 and 100!");

                //suzdavame si profesiq
                Profession* prof = createProfessionFromString(job);
                if (prof == nullptr)
                    throw std::invalid_argument("Unknown profession: " + job);
                //suzdavame grajdanina
                Citizen* newCitizen = new Citizen(name,prof,happiness,money,life);

                delete prof; //triem vremennata profesiq, zashtoto konstruktora na Citizen si e napravil kopie

                //dobavqme go v grada
                try
                {
                    map.addCitizen(x,y,newCitizen);
                    std::cout << "Citizen " << name << " added successfully!" << std::endl;
                }
                catch (const std::exception& e)
                {
                    delete newCitizen;
                    throw;
                }
            }
            else if (command == "remove")
            {
                unsigned x,y;
                std::string name;

                std::cin >> x >> y >> name;
                if (std::cin.fail())
                {
                    clearInputStream();
                    throw std::invalid_argument("Invalid input for remove!");
                }

                map.removeCitizen(x,y,name);
                std::cout << "Citizen removed successfully!" << std::endl;
            }
            else if (command == "step")
            {
                //ignorirame eventualni prazni mesta sled step (primer step     5\n)
                while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
                    std::cin.get();

                //ako sledvashtoto neshto e nov red znachi nqma nishto
               if (std::cin.peek() == '\n' || std::cin.peek() == EOF)
                   map.step(1);
               else
               {
                    int count;
                    std::cin >> count;
                    if (std::cin.fail())
                    {
                        clearInputStream();
                        throw std::invalid_argument("Invalid input for step!");
                    }
                    map.step(count);
               }
            }
            else if (command == "exit")
            {
                std::cout << "Do you want to save the current simulation before exiting? (y/n): ";
                char answer;
                std::cin >> answer;
                if (answer == 'y' || answer == 'Y')
                {
                    std::cout << "Enter filename to save: ";
                    std::string filename;
                    std::cin >> filename;
                    map.saveToFile(filename);
                }
                std::cout << "Disconnecting from the Matrix.." << std::endl;
                break;
            }
            else if (command == "info")
            {
                while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
                    std::cin.get();

                //ako sledva nov red, znachi e samo "info"
                if (std::cin.peek() == '\n' || std::cin.peek() == EOF)
                {
                    if (std::cin.peek() == '\n') std::cin.get(); //razkarvame '\n' ot bufera za da nqmame problemi sus straniciraneto
                    map.printCityInfo();
                }
                else
                {
                    //sus sigurnost imame koordinati i gi chetem
                    unsigned x,y;
                    std::cin >> x >> y;

                    if (std::cin.fail())
                    {
                        clearInputStream();
                        throw std::invalid_argument("Invalid coordinates for info command!");
                    }

                    while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
                        std::cin.get();

                    //ako sled koordinatite ima nov red znachi e "info <x> <y>"
                    if (std::cin.peek() == '\n' || std::cin.peek() == EOF)
                    {
                        if (std::cin.peek() == '\n') std::cin.get(); //i tuk sushto mahame '\n' ot bufera
                        map.printLocationInfo(x,y);
                    }
                    else
                    {
                        //znachi sled koordinatite ima i ime "info <x> <y> <name>"
                        std::string name;
                        std::cin >> name;

                        if (std::cin.fail())
                        {
                            clearInputStream();
                            throw std::invalid_argument("Invalid name for info command!");
                        }

                        while (std::cin.get() != '\n' && !std::cin.eof()) {} // izchistvame vsichko ostanalo ot reda vkluchitelno i '\n' za da e chist bufera
                        map.printCitizenInfo(x,y,name);
                    }
                }
            }
            else if (command == "stat")
            {
                while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
                    std::cin.get();

                //proverqvame dali potrebitelq e natisnal enter bez da vuvede opciq
                if (std::cin.peek() == '\n' || std::cin.peek() == EOF)
                {
                    if (std::cin.peek() == '\n') std::cin.get(); //izqjdame \n
                    throw std::invalid_argument("Usage: stat <happiness | money | life | profession | buildings>");
                }

                //chetem opciqta
                std::string option;
                std::cin >> option;

                if (std::cin.fail())
                {
                    clearInputStream();
                    throw std::invalid_argument("Invalid option for stat command!");
                }

                //izchistvame ostatuka ot reda, vkluch '\n', za da e chist buferut za straniciraneto
                while (std::cin.get() != '\n' && !std::cin.eof()) {}

                map.printStatistics(option);
            }
            else if (command == "history")
            {
                //izqjdame eventualni prazni mesta
                while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
                    std::cin.get();

                //proverqvame dali sledva nov red
                if (std::cin.peek() == '\n' || std::cin.peek() == EOF)
                {
                    if (std::cin.peek() == '\n') std::cin.get(); //izqjdame '\n' ot potoka
                    map.printCityHistory();
                }
                else
                {
                    std::cout << "Invalid format! Just use 'history'." << std::endl;
                    while (std::cin.get() != '\n' && !std::cin.eof()) {}
                }
            }
            else
            {
                std::cout << "Unknown command: " << command << std::endl;

                //ignorirame ostatuka ot greshnata komanda (primer asdf 10 20 Ivan\n)
                while (std::cin.peek() != '\n' && !std::cin.eof()) { std::cin.get(); }
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        std::cout << "> "; //nova strelka za sledvashta komanda
    }
}

void CommandCenter::clearInputStream()
{
    std::cin.clear();   //izchistva flaga za greshka
    while (std::cin.get() != '\n' && !std::cin.eof()) {}//izhurlq ostatuka ot reda?
}

Profession* CommandCenter::createProfessionFromString(const std::string& str)
{
    if (str == "Teacher") return new Teacher();
    if (str == "Programmer") return new Programmer();
    if (str == "Miner") return new Miner();
    if (str == "Student") return new Student();
    if (str == "Unemployed") return new Unemployed();

    return nullptr;
}