#include "flight.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

Flight::Flight() : flightNumber(0), planeId(0), day(0), month(0), year(0), 
                   departureTime("00:00"), destination("") {}

Flight::Flight(int flightNum, int plnId, int d, int m, int y, string time, string dest)
    : flightNumber(flightNum), planeId(plnId), day(d), month(m), year(y),
      departureTime(time), destination(dest) {}

int Flight::getFlightNumber() const { return flightNumber; }
int Flight::getPlaneId() const { return planeId; }
int Flight::getDay() const { return day; }
int Flight::getMonth() const { return month; }
int Flight::getYear() const { return year; }
string Flight::getDepartureTime() const { return departureTime; }
string Flight::getDestination() const { return destination; }
std::map<string, int> Flight::getTicketPricing() const { return ticketPricing; }
std::map<string, int> Flight::getSeatsReserved() const { return seatsReserved; }

int Flight::getPriceForClass(string className) const {
    auto it = ticketPricing.find(className);
    return (it != ticketPricing.end()) ? it->second : 0;
}

int Flight::getSeatsReservedForClass(string className) const {
    auto it = seatsReserved.find(className);
    return (it != seatsReserved.end()) ? it->second : 0;
}

void Flight::setFlightNumber(int flightNum) { flightNumber = flightNum; }
void Flight::setPlaneId(int plnId) { planeId = plnId; }
void Flight::setDay(int d) { day = d; }
void Flight::setMonth(int m) { month = m; }
void Flight::setYear(int y) { year = y; }
void Flight::setDepartureTime(string time) { departureTime = time; }
void Flight::setDestination(string dest) { destination = dest; }
void Flight::setTicketPricing(std::map<string, int> pricing) { ticketPricing = pricing; }
void Flight::setSeatsReserved(std::map<string, int> reserved) { seatsReserved = reserved; }

void Flight::addTicketPrice(string className, int price) {
    ticketPricing[className] = price;
}

// Seat management
bool Flight::reserveSeat(string className) {
    seatsReserved[className]++;
    return true;
}

bool Flight::cancelSeat(string className) {
    if (seatsReserved[className] > 0) {
        seatsReserved[className]--;
        return true;
    }
    return false;
}

int Flight::getAvailableSeats(string className, int totalSeatsInClass) const {
    int reserved = getSeatsReservedForClass(className);
    return totalSeatsInClass - reserved;
}

string Flight::getDateString() const {
    return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
}

void Flight::display() const {
    cout << "\n===== Flight Info =====" << endl;
    cout << "Flight Number: " << flightNumber << endl;
    cout << "Plane ID: " << planeId << endl;
    cout << "Date: " << day << "/" << month << "/" << year << endl;
    cout << "Departure Time: " << departureTime << endl;
    cout << "Destination: " << destination << endl;
    cout << "Ticket Pricing:" << endl;
    for (const auto& pair : ticketPricing) {
        cout << "  " << pair.first << ": $" << pair.second << endl;
    }
    cout << "Seats Reserved:" << endl;
    for (const auto& pair : seatsReserved) {
        cout << "  " << pair.first << ": " << pair.second << " seats" << endl;
    }
    cout << "=======================" << endl;
}