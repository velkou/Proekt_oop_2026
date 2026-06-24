#pragma once
#include <string>
///Поддържа хронологията в симулацията, като съдържа единично събитие и датата, на която се е случило.
///Използва се както за историята на целия град, така и за индивидуалната история на всеки гражданин.
class LogEntry {
public:
    LogEntry(const std::string& date, const std::string& descr);

    void print() const;

    //Гетъри
    const std::string& getDate() const { return date; }
    const std::string& getDescription() const { return description; }

    //Сериализацията
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
private:
    std::string date;
    std::string description;
};
