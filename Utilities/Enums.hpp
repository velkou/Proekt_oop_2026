#pragma once
///За гарантиране на Type Safety и оптимизация при сериализацията, в проекта са въведени следните enum class-ове
///чрез които логиката за зареждане от файл и извличане на статистики разчита на бързите switch конструкции.
enum class ProfessionType
{
    Teacher,
    Programmer,
    Miner,
    Student,
    Unemployed,
    None
};

enum class BuildingType
{
    Modern,
    Panel,
    Dorm,
    None
};