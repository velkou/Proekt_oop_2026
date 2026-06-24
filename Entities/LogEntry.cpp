#include "../Entities/LogEntry.hpp"
#include <iostream>

#include "../Utilities/BinaryUtils.hpp"

LogEntry::LogEntry(const std::string& date, const std::string& descr) :
    date(date), description(descr) {}

void LogEntry::print() const
{
    std::cout << date + " " + description << std::endl;
}

void LogEntry::save(std::ofstream& out) const
{
    BinaryUtils::writeString(out,date);
    BinaryUtils::writeString(out,description);
}

void LogEntry::load(std::ifstream& in)
{
    date = BinaryUtils::readString(in);
    description = BinaryUtils::readString(in);
}
