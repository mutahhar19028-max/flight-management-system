#include "airport.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

// Constructors
Airport::Airport() : airportName(""), country(""), usualWeatherCondition(""), 
                     timeFromIslamabad(0), milesFromIslamabad(0) {}

Airport::Airport(string name, string ctry, string weather, int time, int miles)
    : airportName(name), country(ctry), usualWeatherCondition(weather),
      timeFromIslamabad(time), milesFromIslamabad(miles) {}

// Getters
string Airport::getName() const { return airportName; }
string Airport::getCountry() const { return country; }
string Airport::getWeather() const { return usualWeatherCondition; }
int Airport::getTimeFromIslamabad() const { return timeFromIslamabad; }
int Airport::getMilesFromIslamabad() const { return milesFromIslamabad; }

// Setters
void Airport::setName(string name) { airportName = name; }
void Airport::setCountry(string ctry) { country = ctry; }
void Airport::setWeather(string weather) { usualWeatherCondition = weather; }
void Airport::setTimeFromIslamabad(int time) { timeFromIslamabad = time; }
void Airport::setMilesFromIslamabad(int miles) { milesFromIslamabad = miles; }

// Display
void Airport::display() const {
    cout << "\n===== Airport Info =====" << endl;
    cout << "Airport: " << airportName << endl;
    cout << "Country: " << country << endl;
    cout << "Weather: " << usualWeatherCondition << endl;
    cout << "Time from Islamabad: " << timeFromIslamabad << " hours" << endl;
    cout << "Distance from Islamabad: " << milesFromIslamabad << " miles" << endl;
    cout << "========================" << endl;
}