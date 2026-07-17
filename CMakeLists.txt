cmake_minimum_required(VERSION 4.2)
project(proekt)

set(CMAKE_CXX_STANDARD 20)

add_executable(proekt main.cpp
        Entities/Citizen.cpp
        Entities/LogEntry.cpp
        Professions/Teacher.cpp
        Professions/Unemployed.cpp
        Professions/Student.cpp
        Professions/Programmer.cpp
        Professions/Miner.cpp
        Buildings/Building.cpp
        Buildings/Building.hpp
        Buildings/ModernBuilding.cpp
        Buildings/ModernBuilding.hpp
        Buildings/Panel.cpp
        Buildings/Panel.hpp
        Buildings/Dorm.cpp
        Buildings/Dorm.hpp
        Core/Cell.cpp
        Core/Cell.hpp
        Core/CityMap.cpp
        Core/CityMap.hpp
        Core/CommandCenter.cpp
        Core/CommandCenter.hpp
        Utilities/Enums.hpp
        Utilities/BinaryUtils.hpp
        Buildings/Location.cpp
        Buildings/Location.hpp
        Buildings/CentralLoc.cpp
        Buildings/CentralLoc.hpp
        Buildings/EndLoc.cpp
        Buildings/EndLoc.hpp
        Buildings/StandartLoc.cpp
        Buildings/StandartLoc.hpp
)
