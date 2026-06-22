#pragma once
#include <string>

class LogEntry {
public:
    LogEntry(const std::string& date, const std::string& descr);
    void print() const;
    const std::string& getDate() const { return date; }
    const std::string& getDescription() const { return description; }
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
private:
    std::string date;
    std::string description;
};
