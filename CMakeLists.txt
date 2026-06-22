cmake_minimum_required(VERSION 4.2)
project(proekt)

set(CMAKE_CXX_STANDARD 20)

add_executable(proekt main.cpp
        Citizen.cpp
        LogEntry.cpp
        Teacher.cpp
        Unemployed.cpp
        Student.cpp
        Programmer.cpp
        Miner.cpp
        Building.cpp
        Building.hpp
        ModernBuilding.cpp
        ModernBuilding.hpp
        Panel.cpp
        Panel.hpp
        Dorm.cpp
        Dorm.hpp
        Cell.cpp
        Cell.hpp
        CityMap.cpp
        CityMap.hpp
        CommandCenter.cpp
        CommandCenter.hpp
)
