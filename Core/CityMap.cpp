#include "CityMap.hpp"
#include "../Buildings/Dorm.hpp"
#include "../Professions/Miner.hpp"
#include "../Buildings/ModernBuilding.hpp"
#include "../Professions/Programmer.hpp"
#include "../Professions/Student.hpp"
#include "../Professions/Teacher.hpp"
#include "../Professions/Unemployed.hpp"
#include <iostream>
#include <fstream>
#include "../Utilities/BinaryUtils.hpp"
#include "../Buildings/Panel.hpp"

void CityMap::generate(unsigned rows, unsigned cols)
{
    n = rows;
    m = cols;

    ///Избор на случайно име на град от предварително дефинирано множество.
    std::vector<std::string> cityNames{
        "Sofia", "Plovdiv", "Varna", "Burgas", "Ruse", "Shumen",
        "Pleven", "Stara Zagora", "Targovishte", "Polski Trumbej"
    };

    ///Избор на случайно име на човек от предварително дефинирано множество.
    std::vector<std::string> randomNames{
        "Valentin","Berov","Martin","Viktor","Gabriela",
            "Silvia","Sophie","Ludmil","Ivan","Petar","Maria",
            "Thea","Katerina"};

    cityName = cityNames[std::rand() % cityNames.size()]; ///Избира случайно спрямо размера на масива

    ///Фиксираме начална дата
    day = 1;
    month = 1;
    year = 2026;
    startDate = "01.01.2026";
    currentDate = "01.01.2026";

    grid.clear(); ///Прави grid празен
    grid.resize(n,std::vector<Cell>(m)); ///n-новият размер,x:стойност,чрез която се инициализират допулнителни елементи от vector

    ///Запълване със случайни сгради и граждани
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            if ((std::rand() % 100) < 75) ///Даваме 75% шанс да се построи сграда в тази клетка
            {
                Building* newBuilding = nullptr;
                BuildingType type = static_cast<BuildingType>(std::rand() % 3); ///Генерираме само: 0-modern, 1-panel, 2-dorm
                unsigned maxCapacity  = 5 + (std::rand() % 36); ///Максимум капацитет между 5 и 40
                bool isDorm = false;

                switch (type)
                {
                    case BuildingType::Modern:
                        newBuilding = new ModernBuilding(maxCapacity); break;
                    case BuildingType::Panel:
                        newBuilding = new Panel(maxCapacity); break;
                    case BuildingType::Dorm:
                        newBuilding = new ModernBuilding(maxCapacity);
                        isDorm = true; break;
                    default:
                        break; ///Ако по някаква причина type е None не правим нищо
                }

                grid[i][j].build(newBuilding); ///Построяваме сградата в клетката

                unsigned citizenCount = std::rand() % (maxCapacity + 1); ///Случаен брой жители от 0 до максималния капацитет
                for (size_t k = 0; k < citizenCount; ++k)
                {
                    ///Уникално име, за да можем да различим различни граждани с еднакви имена
                    std::string citName = randomNames[std::rand() % randomNames.size()] + "@" + std::to_string(std::rand() % 1000);
                    unsigned happiness = std::rand() % 101;
                    unsigned money = std::rand() % 3001;
                    unsigned lifePoints = 50 + (std::rand() % 51); ///Живот между 50 и 100, за да не започват симулацията мъртви

                    Profession* citProf = nullptr;
                    if (isDorm)  citProf = new Student(); ///Тук сме в общак и задължително създаваме студент
                    else    ///Тук сме в панелка или модерна сграда - всеки може да живее тук, включително студенти
                    {
                        unsigned profType = std::rand() % 5;
                        if (profType == 0)  citProf = new Teacher();
                        else if (profType == 1) citProf = new Programmer();
                        else if (profType == 2) citProf = new Miner();
                        else if (profType == 3) citProf = new Unemployed();
                        else citProf = new Student();
                    }

                    Citizen* newCitizen = new Citizen(citName,citProf,happiness,money,lifePoints);

                    delete citProf;///!

                    grid[i][j].addPerson(newCitizen);
                }
            }
        }
    }

    std::string logDescr = "City generated: " + cityName + " with dimensions " + std::to_string(n) + "x" + std::to_string(m);
    cityHistory.push_back(LogEntry(currentDate,logDescr));

    std::cout << currentDate << std::endl;
}

void CityMap::addCitizen(unsigned x, unsigned y, Citizen* citizen)
{
    if (n <= x || m <= y) throw std::invalid_argument("Invalid coordinates");

    Cell& targetCell = grid[x][y];
    const Building* bldg = targetCell.getBuilding();

    if (bldg == nullptr)
        throw std::invalid_argument("No building at this location to move in!");
    else
    {
        ///Има ли свободно място
        if (targetCell.getCitizens().size() >= bldg->getMaxCap())
            throw std::runtime_error("The building at this location is full!");


        ///Ако сградата е общак, има ли право човекът да е там
        if (bldg->getType() == BuildingType::Dorm)
        {
            if (!citizen->canLiveInDorm())
                throw std::invalid_argument("Only students can live in a dorm!");
        }
    }

    for (const Citizen* c : targetCell.getCitizens())
    {
        if (c->getName() == citizen->getName())
            throw std::invalid_argument("A citizen with this name already exists here!");
    }

    targetCell.addPerson(citizen);
}

void CityMap::removeCitizen(unsigned x, unsigned y, const std::string& name)
{
    if (x >= n || y >= m ) throw std::invalid_argument("Invalid coordinates");
    grid[x][y].removePerson(name);
}

void CityMap::step(int count)
{
    if (count == 0) return;
    if (count > 0)
    {
        for (size_t i = 0; i < count; ++i)
            performSingleStep();
    }
    else
    {
        size_t stepsBack = -count;

        if (stepsBack > historyOfStates.size())
        {
            std::cout << "Cannot go that far in the past! Going back to the very beginning." << std::endl;
            stepsBack = historyOfStates.size();
        }

        if (stepsBack > 0)
        {
            ///Състоянието от миналото
            CityMap past = historyOfStates[historyOfStates.size() - stepsBack];

            std::vector<CityMap> preservedHistory(historyOfStates.begin(), historyOfStates.end() - stepsBack);
            ///Инициализира preservedHistory с данните от historyOfStates;https://www.geeksforgeeks.org/cpp/initialize-a-vector-in-cpp-different-ways/
            past.historyOfStates = preservedHistory;

            ///Заменяме текущия град с града от миналото
            *this = past;

            std::cout << "Traveled back in time to: " << currentDate << std::endl;
        }
    }
}

void CityMap::printCityInfo() const
{
    unsigned lines = 0;
    if (!printLine("City info: " + cityName,lines)) return;

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            ///Взимаме сградата
            const Building* bldg = grid[i][j].getBuilding();
            if (bldg == nullptr) continue;

            if (!printLine("Location [" + std::to_string(i) + "][" + std::to_string(j) + "]",lines)) return;

            ///Разпознаваме и принтираме типа на сградата
            std::string bldgType = "Unknown building";
            switch (bldg->getType())
            {
                case BuildingType::Modern: bldgType = "Modern Building"; break;
                case BuildingType::Panel: bldgType = "Panel"; break;
                case BuildingType::Dorm: bldgType = "Dorm"; break;
                default: break;
            }

            if (!printLine(bldgType + ":",lines)) return;

            for (Citizen* c : grid[i][j].getCitizens())
            {
                std::string citData = " - " + c->getName() + " | " + c->getProfession();
                if (!printLine(citData,lines)) return;
            }
        }
    }
}

void CityMap::printLocationInfo(unsigned x, unsigned y) const
{
    if (x >= n || y >= m)
    {
        std::cout << "Invalid coordinates!" << std::endl;
        return;
    }

    unsigned lines = 0;
    const Cell& target = grid[x][y];

    ///Взимаме сградата
    const Building* bldg = target.getBuilding();

    if (bldg == nullptr)
    {
        std::cout << "Location [" + std::to_string(x) + "][" + std::to_string(y) + "] is empty." << std::endl;
        return;
    }

    if (!printLine("Location [" + std::to_string(x) + "][" + std::to_string(y) + "]",lines)) return;

    std::string bldgType = "Unknown";
    switch (bldg->getType())
    {
        case BuildingType::Modern: bldgType = "Modern Building"; break;
        case BuildingType::Panel: bldgType = "Panel"; break;
        case BuildingType::Dorm: bldgType = "Dorm"; break;
        default: break;
    }

    ///Отпечатваме типа, наема, капацитета и свободните места в сградата
    std::string bldgInfo =  "Type: " + bldgType +
        "/Base rent: " + std::to_string(bldg->getBaseRent()) +
        "/Capacity: " + std::to_string(bldg->getMaxCap()) +
        "/Free spaces: " + std::to_string(bldg->getMaxCap() - target.getCitizens().size());

    if (!printLine(bldgInfo,lines)) return;

    for (Citizen* c : target.getCitizens())
    {
        std::string citData = " - " + c->getName() + " | " + c->getProfession();
        if (!printLine(citData,lines)) return;
    }
}

void CityMap::printCitizenInfo(unsigned x, unsigned y, const std::string& name) const
{
    if (x >= n || y >= m)
    {
        std::cout << "Invalid coordinates!" << std::endl;
        return;
    }

    unsigned lines = 0;
    const Cell& targetCell = grid[x][y];

    ///Търсим човека в клетката
    Citizen* targetCitizen = nullptr;
    for (Citizen* c : targetCell.getCitizens())
    {
        if (c->getName() == name)
        {
            targetCitizen = c;
            break;
        }
    }

    if (targetCitizen == nullptr)
    {
        std::cout << "Citizen " << name << " not found at this location." << std::endl;
        return;
    }

    ///Печатаме основната информация
    if (!printLine("Citizen: " + targetCitizen->getName(),lines))  return;
    if (!printLine("Profession: " + targetCitizen->getProfession(),lines))  return;
    if (!printLine("Happiness: " + std::to_string(targetCitizen->getHappiness()),lines))  return;
    if (!printLine("Money: " + std::to_string(targetCitizen->getMoney()),lines))  return;
    if (!printLine("Life: " + std::to_string( targetCitizen->getLifePoints()),lines))  return;
    if (!printLine("History: ",lines))  return;

    ///Печатаме историята също със странициране, защото може да е дълга
    for (const LogEntry& entry : targetCitizen->getHistory())
    {
        std::string logLine = "[" + entry.getDate() + "] " + entry.getDescription();
        if (!printLine(logLine,lines)) return;
    }

}

void CityMap::printStatistics(const std::string& option) const
{
    if (option == "happiness" || option == "money" || option == "life")
    {
        unsigned minVal = -1; //най голямото unsigned число
        unsigned maxVal = 0;
        unsigned long long sum = 0;
        unsigned count = 0;


        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < m; ++j)
            {
                for (Citizen* c : grid[i][j].getCitizens())
                {
                    unsigned val = 0;

                    if (option == "happiness") val = c->getHappiness();
                    else if (option == "money") val = c->getMoney();
                    else if (option == "life") val = c->getLifePoints();
                    else continue;

                    if (val < minVal) minVal = val;
                    if (val > maxVal) maxVal = val;

                    sum += val;
                    count++;
                }
            }
        }

        if (!count)
        {
            std::cout << "No citizens in the city to calculate statistics for " << option << std::endl;
            return;
        }

        double average = static_cast<double>(sum) / count;
        unsigned lines = 0;
        printLine("Statistics for " + option + ": ",lines);
        printLine("Average: " + std::to_string(average),lines);
        printLine("Minimum: " + std::to_string(minVal),lines);
        printLine("Maximum: " + std::to_string(maxVal),lines);
    }
    else if (option == "profession")
    {
        ///Когато вземем професията на даден жител претърсваме вектора с имената
        ///ако я намерим увеличаваме бройката на съшата позиция в profCounts
        ///ако не я намерим я добавяме като нова
        std::vector<std::string> profNames;
        std::vector<unsigned> profCounts;
        unsigned totalCitizens = 0;

        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < m; ++j)
            {
                for (Citizen* c : grid[i][j].getCitizens())
                {
                    std::string currProf = c->getProfession();
                    totalCitizens++;

                    ///Проверяваме дали вече сме записали тази професия
                    bool found = false;
                    for (size_t k = 0; k < profNames.size(); ++k)
                    {
                        if (profNames[k] == currProf)
                        {
                            profCounts[k]++;///увеличаваме бройката на съответния индекс
                            found = true;
                            break;
                        }
                    }

                    ///Ако я виждаме за пръв път, я добавяме в края на двата вектора
                    if (!found)
                    {
                        profNames.push_back(currProf);
                        profCounts.push_back(1);
                    }
                }
            }
        }
        if (!totalCitizens)
        {
            std::cout << "No citizens in the city to calculate statistics for profession!" << std::endl;
            return;
        }

        unsigned lines = 0;
        printLine("Profession statistics: ",lines);

        for (size_t i = 0; i < profNames.size(); ++i)
        {
            std::string line = " - " + profNames[i] + ": " + std::to_string(profCounts[i]);
            if (!printLine(line,lines)) return;
        }
    }
    else if (option == "buildings")
    {
        std::vector<std::string> bldgTypes;
        std::vector<unsigned> bldgTypeCounts;

        std::vector<std::string> bldgLocs;
        std::vector<unsigned> bldgLocCounts;

        unsigned totalBldgs = 0;

        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < m; ++j)
            {
                const Building* bldg = grid[i][j].getBuilding();

                if (bldg != nullptr)
                {
                    totalBldgs++;
                    ///Проверка по тип
                    std::string currType = "Unknown";
                    switch (bldg->getType())
                    {
                        case BuildingType::Dorm: currType = "Dorm"; break;
                        case BuildingType::Panel: currType = "Panel"; break;
                        case BuildingType::Modern: currType = "Modern Building"; break;
                        default: break;
                    }

                    bool typeFound = false;
                    for (size_t k = 0; k < bldgTypes.size(); ++k)
                    {
                        if (bldgTypes[k] == currType)
                        {
                            bldgTypeCounts[k]++;
                            typeFound = true;
                            break;
                        }
                    }
                    if (!typeFound)
                    {
                        bldgTypes.push_back(currType);
                        bldgTypeCounts.push_back(1);
                    }

                    ///Проверка по локация
                    std::string currLoc = "Standard";
                    int cx = n / 2;
                    int cy = m / 2;

                    ///Смятаме Манхатънското разстояние както е в step
                    int dist = std::abs((int)i - cx) + std::abs((int)j - cy);

                    ///Намираме min(n,m)
                    int minDim = (n < m) ? n : m;

                    if (dist <= minDim / 8)
                        currLoc = "Central";
                    else if (dist > 6 * (minDim / 8))
                        currLoc = "Peripheral";

                    bool locFound = false;
                    for (size_t k = 0; k < bldgLocs.size(); ++k)
                    {
                        if (bldgLocs[k] == currLoc)
                        {
                            bldgLocCounts[k]++;
                            locFound = true;
                            break;
                        }
                    }
                    if (!locFound)
                    {
                        bldgLocs.push_back(currLoc);
                        bldgLocCounts.push_back(1);
                    }
                }
            }
        }

        if (!totalBldgs)
        {
            std::cout << "No buildings exist in the city yet!" << std::endl;
            return;
        }

        unsigned lines = 0;
        printLine("Building statistics: ",lines);
        printLine("Total buildings: " + std::to_string(totalBldgs),lines);

        printLine("By type: ",lines);
        for (size_t i = 0; i < bldgTypes.size(); ++i)
        {
            std::string line = " - " + bldgTypes[i] + ": " + std::to_string(bldgTypeCounts[i]);
            if (!printLine(line,lines)) return;
        }

        printLine("By location: ",lines);
        for (size_t i = 0; i < bldgLocs.size(); ++i)
        {
            std::string line = " - " + bldgLocs[i] + ": " + std::to_string(bldgLocCounts[i]);
            if (!printLine(line,lines)) return;
        }
    }
    else
    {
        std::cerr << "No statistic for chosen option is available!" << std::endl;
    }
}

void CityMap::printCityHistory() const
{
    if (cityHistory.empty())
    {
        std::cout << "The city has no history yet!" << std::endl;
        return;
    }

    unsigned lines = 0;
    if (!printLine("City history of " + cityName + ": ",lines)) return;

    for (const LogEntry& entry : cityHistory)
    {
        std::string logLine = "[" + entry.getDate() + "]" + entry.getDescription();
        if (!printLine(logLine,lines)) return;
    }
}

void CityMap::saveToFile(const std::string& filename) const
{
    std::ofstream out(filename,std::ios::binary);
    if (!out.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for saving!" << std::endl;
        return;
    }

    ///Записваме примитивните типове на града
    BinaryUtils::writePrimitive(out,n);
    BinaryUtils::writePrimitive(out,m);
    BinaryUtils::writePrimitive(out,day);
    BinaryUtils::writePrimitive(out,month);
    BinaryUtils::writePrimitive(out,year);

    ///Записваме стринговете на града
    BinaryUtils::writeString(out,cityName);
    BinaryUtils::writeString(out,startDate);
    BinaryUtils::writeString(out,currentDate);

    ///Записваме общата история на града (cityHistory)
    size_t historySize = cityHistory.size();
    BinaryUtils::writePrimitive(out,historySize);
    for (const LogEntry& entry : cityHistory)
        entry.save(out);

    ///Записваме grid-a
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
            grid[i][j].save(out); //vsqka kletka zapisva sgradata, horata si i istoriqta na samata kletka
    }

    if (out.fail())
        throw std::runtime_error("Critical error with file saving");

    out.close();
    std::cout << "Simulation state successfully saved to " << filename << std::endl;
}

void CityMap::loadFromFile(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for loading!\n";
        return;
    }

    ///Изчистваме стария град напълно -> автоматично вика ~Cell за всяка клетка
    grid.clear();

    ///Четем примитивните типове
    BinaryUtils::readPrimitive(in,n);
    BinaryUtils::readPrimitive(in,m);
    BinaryUtils::readPrimitive(in,day);
    BinaryUtils::readPrimitive(in,month);
    BinaryUtils::readPrimitive(in,year);

    ///Четем стринговете
    cityName = BinaryUtils::readString(in);
    startDate = BinaryUtils::readString(in);
    currentDate = BinaryUtils::readString(in);

    ///Възстановяваме общата история на града
    size_t historySize = 0;
    BinaryUtils::readPrimitive(in,historySize);
    cityHistory.clear();
    for (size_t i = 0; i < historySize; ++i)
    {
        LogEntry entry("","");
        entry.load(in);
        cityHistory.push_back(entry);
    }

    ///Възстановяваме grid-a
    grid.clear();
    grid.resize(n,std::vector<Cell>(m)); //preorazmerqvame spored prochetenite n i m
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
            grid[i][j].load(in);
    }

    ///Изчистваме historyOfStates
    ///Започваме времевото пътуване от текущия момент на зареждане
    historyOfStates.clear();

    if (in.fail())
        throw std::runtime_error("Critical error from file reading");

    in.close();
    std::cout << "Simulation state successfully loaded from '" << filename << std::endl;
    std::cout << "Current city: " << cityName << " | Date: " << currentDate << std::endl;
}

bool CityMap::printLine(const std::string& text, unsigned& lineCounter, unsigned maxLines) const
{
    ///Проверяваме дали сме стигнали лимита (това означава, че идва ред за нова страница)
    if (lineCounter >= maxLines)
    {
        std::cout << "Press [Enter] to continue or 'q' to quit!";
        std::string input;
        std::getline(std::cin, input);

        if (input == "q" || input == "Q")
            return false;

        lineCounter = 0; //Нулираме брояча за следващата страница
    }

    ///Принтираме реда
    std::cout << text << std::endl;
    lineCounter++;

    return true;
}

void CityMap::performSingleStep()
{
    ///Запазване на пътуване във времето
    CityMap stateCopy = *this;
    stateCopy.historyOfStates.clear(); ///Трие историята, за да не препълни паметта!
    historyOfStates.push_back(stateCopy);

    ///Броячи за изхода
    unsigned dead = 0, depressed = 0, broke = 0;

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            ///Манхатън разстояние до центъра (n/2,m/2)
            unsigned dist = std::abs((int)i - (int)(n/2)) + std::abs((int)j - (int)(m/2));

            ///Ежедневие на граждани
            grid[i][j].triggerDailyRoutine(day,currentDate,dist,n,m);

            if (day == 1)
            {
                for (Citizen* c : grid[i][j].getCitizens())
                    c->triggerMonthlyEffects(currentDate);
            }

            ///Проверка за умрели и тъжни
            std::vector<std::string> deadNames;

            for (Citizen* c : grid[i][j].getCitizens())
            {
                if (c->getHappiness() == 0) depressed++;
                if (c->getMoney() == 0) broke++;
                if (c->getLifePoints() == 0)
                {
                    dead++;
                    deadNames.push_back(c->getName());///Маркиран за триене
                }
            }

            for (const std::string& name : deadNames)
                grid[i][j].removePerson(name);
        }
    }

    ///Извеждане на резултата
    std::cout << "Dead: " << dead << ", depressed: " << depressed << ", broke: " << broke << "." << std::endl;

    ///Структура съдържаща календарни данни
    std::tm dateStruct = {};
    dateStruct.tm_mday = day; //tekusht den(1-31)
    dateStruct.tm_mon = month - 1; //v <ctime> mesecite sa ot 0-11
    dateStruct.tm_year = year - 1900; // ne se pazi cqlata godina, a se pazi kolko godini sa minali ot 1990-ta nasamm, basi qkoto

    dateStruct.tm_mday += 1; //uvelichavame s 1 den

    std::mktime(&dateStruct); //<ctime> avtomatichno shte si korigira datata

    day = dateStruct.tm_mday;
    month = dateStruct.tm_mon + 1;
    year = dateStruct.tm_year + 1900;

    updateCurrDateStr();
}

///Форматиране на датата в стринг
void CityMap::updateCurrDateStr()
{
    std::string d = (day < 10) ? "0" + std::to_string(day) : std::to_string(day);
    std::string monthStr = (month < 10) ? "0" + std::to_string(month) : std::to_string(month);
    currentDate = d + "." + monthStr + "." + std::to_string(year);
}
