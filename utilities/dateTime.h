#ifndef DATETIME_H
#define DATETIME_H

#include <string>

class DateTime {
public:
    static std::string getCurrentDateTime();
    static std::string formatDate(int day, int month, int year);
    static std::string formatDateTime(int day, int month, int year, const std::string& time);
    static void parseDate(const std::string& dateStr, int& day, int& month, int& year);
    static void parseDateTime(const std::string& dateTimeStr, int& day, int& month, int& year, std::string& time);
    static bool isValidDate(int day, int month, int year);
    static bool isValidTime(const std::string& time);
    static int getDayOfWeek(int day, int month, int year);
    static std::string getDayName(int dayOfWeek);
};

#endif