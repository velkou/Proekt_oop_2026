#pragma once
#include "../Core/CityMap.hpp"

///Парсер на команди. Осигурява интерактивния CLI и управлява жизнения цикъл на входа.

class CommandCenter {
public:
    ///Главния цикъл на приложението
    void run();
private:
    ///Помощна функция за изчистване на потора при грешни данни
    void clearInputStream();
    Profession* createProfessionFromString(const std::string& str);
private:
    CityMap map;
};
