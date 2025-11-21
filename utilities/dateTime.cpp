#include "dateTime.h"
#include <ctime>
#include <sstream>
#include <iomanip>

std::string DateTime::getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << ltm->tm_mday << "/"
        << std::setw(2) << (ltm->tm_mon + 1) << "/"
        << (ltm->tm_year + 1900) << " "
        << std::setw(2) << ltm->tm_hour << ":"
        << std::setw(2) << ltm->tm_min;
    return oss.str();
}

std::string DateTime::formatDate(int day, int month, int year) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << day << "/"
        << std::setw(2) << month << "/"
        << year;
    return oss.str();
}

std::string DateTime::formatDateTime(int day, int month, int year, const std::string& time) {
    return formatDate(day, month, year) + " " + time;
}

void DateTime::parseDate(const std::string& dateStr, int& day, int& month, int& year) {
    std::istringstream iss(dateStr);
    char delim;
    iss >> day >> delim >> month >> delim >> year;
}

void DateTime::parseDateTime(const std::string& dateTimeStr, int& day, int& month, int& year, std::string& time) {
    size_t spacePos = dateTimeStr.find(' ');
    if (spacePos != std::string::npos) {
        std::string dateStr = dateTimeStr.substr(0, spacePos);
        time = dateTimeStr.substr(spacePos + 1);
        parseDate(dateStr, day, month, year);
    } else {
        parseDate(dateTimeStr, day, month, year);
        time = "00:00";
    }
}

bool DateTime::isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29;
    }
    
    if (day < 1 || day > daysInMonth[month]) return false;
    
    return true;
}

bool DateTime::isValidTime(const std::string& time) {
    if (time.length() != 5 || time[2] != ':') return false;
    
    int hour = std::stoi(time.substr(0, 2));
    int minute = std::stoi(time.substr(3, 2));
    
    return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
}

int DateTime::getDayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }
    int k = year % 100;
    int j = year / 100;
    int h = (day + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
    return (h + 6) % 7;
}

std::string DateTime::getDayName(int dayOfWeek) {
    std::string days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    if (dayOfWeek >= 0 && dayOfWeek <= 6) {
        return days[dayOfWeek];
    }
    return "Unknown";
}