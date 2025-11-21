#include "validator.h"
#include <cctype>
#include <algorithm>

bool Validator::isValidPhone(const std::string& phone) {
    if (phone.empty()) return false;
    
    for (char c : phone) {
        if (!std::isdigit(c) && c != '-' && c != ' ' && c != '+' && c != '(' && c != ')') {
            return false;
        }
    }
    
    int digitCount = 0;
    for (char c : phone) {
        if (std::isdigit(c)) digitCount++;
    }
    
    return digitCount >= 7 && digitCount <= 15;
}

bool Validator::isValidName(const std::string& name) {
    if (name.empty()) return false;
    
    for (char c : name) {
        if (!std::isalpha(c) && c != ' ' && c != '-' && c != '\'') {
            return false;
        }
    }
    
    return true;
}

bool Validator::isValidDate(const std::string& date) {
    if (date.length() < 8 || date.length() > 10) return false;
    
    size_t firstSlash = date.find('/');
    size_t secondSlash = date.rfind('/');
    
    if (firstSlash == std::string::npos || secondSlash == std::string::npos || firstSlash == secondSlash) {
        return false;
    }
    
    try {
        int day = std::stoi(date.substr(0, firstSlash));
        int month = std::stoi(date.substr(firstSlash + 1, secondSlash - firstSlash - 1));
        int year = std::stoi(date.substr(secondSlash + 1));
        
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        return true;
    } catch (...) {
        return false;
    }
}

bool Validator::isValidTime(const std::string& time) {
    if (time.length() != 5 || time[2] != ':') return false;
    
    try {
        int hour = std::stoi(time.substr(0, 2));
        int minute = std::stoi(time.substr(3, 2));
        
        return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
    } catch (...) {
        return false;
    }
}

bool Validator::isValidPositiveInt(const std::string& str) {
    if (str.empty()) return false;
    
    for (char c : str) {
        if (!std::isdigit(c)) return false;
    }
    
    try {
        int value = std::stoi(str);
        return value > 0;
    } catch (...) {
        return false;
    }
}

bool Validator::isValidChoice(int choice, int min, int max) {
    return choice >= min && choice <= max;
}

bool Validator::isValidTicketType(const std::string& type) {
    std::string lowerType = type;
    std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);
    
    return (lowerType == "economy" || lowerType == "business" || 
            lowerType == "first" || lowerType == "premium");
}

std::string Validator::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

bool Validator::isEmpty(const std::string& str) {
    return trim(str).empty();
}