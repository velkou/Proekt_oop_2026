#include <iostream>
#include "CommandCenter.hpp"
int main()
{
    //inicializirame generatora na sluchaini chisla
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    CommandCenter center;
    center.run();
}