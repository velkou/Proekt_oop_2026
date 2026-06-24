#include <iostream>
#include "Core/CommandCenter.hpp"
int main()
{
    try
    {
        ///std::time(nullptr) е функциям която взема текущото време от часовника на
        ///компютъра в секунди. Подаваме го на srand(seed random)
        ///-> използва текущата секунда като начална точка за всяка формула
        //inicializirame generatora na sluchaini chisla
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        CommandCenter center;
        center.run();
    }
    ///za vsichki standartni greshki
    catch (const std::exception& e)
    {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
    //za nestandartni greshki(hvurlen string ili int)
    catch (...)
    {
        std::cerr << "Unknown critical error!" << std::endl;
        return 1;
    }
    return 0;


    ///Tests:
    ///generate -2 2; generate 0 0;
    ///add 0 0 Ivan Teacher 100 -50 100
    ///add 0 0 Gosho Miner 150 200 150
    ///Нека generate 2 2 -> add 5 5 Tosho Programmer 100 100 100
    ///remove 0 0 Batman -> Изтриване на несъществуващ човек
    ///add 0 0 Pesho .. .. .. и след това пак add 0 0 Pesho .. .. ..
    ///Намираме клетка чрез info, която е Dorm. Опитваме се да вкараме Miner или Programmer там.
    ///Месечни ефекти на различни професии чрез step 30 примерно
    ///Смърт от глад: add 0 0 Peshi 100 100 100 -> step 5
    ///undo: генерираме град -> добавяме жител -> step 1 -> step -1
    ///опит за пътуване назад във времето по време на първия ден
    ///сериализация

}