#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>

using std::string;

class Airport {
private:
    string airportName;
    string country;
    string usualWeatherCondition;
    int timeFromIslamabad;
    int milesFromIslamabad;

public:
    Airport();
    Airport(string name, string ctry, string weather, int time, int miles);

    // Getters
    string getName() const;
    string getCountry() const;
    string getWeather() const;
    int getTimeFromIslamabad() const;
    int getMilesFromIslamabad() const;

    void setName(string name);
    void setCountry(string ctry);
    void setWeather(string weather);
    void setTimeFromIslamabad(int time);
    void setMilesFromIslamabad(int miles);

    void display() const;
};

#endif