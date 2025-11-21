#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

class Validator {
public:
    static bool isValidPhone(const std::string& phone);
    static bool isValidName(const std::string& name);
    static bool isValidDate(const std::string& date);
    static bool isValidTime(const std::string& time);
    static bool isValidPositiveInt(const std::string& str);
    static bool isValidChoice(int choice, int min, int max);
    static bool isValidTicketType(const std::string& type);
    static std::string trim(const std::string& str);
    static bool isEmpty(const std::string& str);
};

#endif