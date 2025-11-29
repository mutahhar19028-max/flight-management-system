#include "plane.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

Plane::Plane() : planeId(0), description(""), numberOfMilesTraveled(0) {}

Plane::Plane(int id, string desc, int miles)
    : planeId(id), description(desc), numberOfMilesTraveled(miles) {}

int Plane::getPlaneId() const { return planeId; }
string Plane::getDescription() const { return description; }
int Plane::getNumberOfMilesTraveled() const { return numberOfMilesTraveled; }
std::map<string, int> Plane::getSeatsPerClass() const { return numberOfSeatsPerClass; }
std::map<string, int> Plane::getClassPricing() const { return classPricing; }

int Plane::getSeatsForClass(string className) const {
    auto it = numberOfSeatsPerClass.find(className);
    return (it != numberOfSeatsPerClass.end()) ? it->second : 0;
}

int Plane::getPriceForClass(string className) const {
    auto it = classPricing.find(className);
    return (it != classPricing.end()) ? it->second : 0;
}

int Plane::getTotalSeats() const {
    int total = 0;
    for (const auto& pair : numberOfSeatsPerClass) {
        total += pair.second;
    }
    return total;
}

void Plane::setPlaneId(int id) { planeId = id; }
void Plane::setDescription(string desc) { description = desc; }
void Plane::setNumberOfMilesTraveled(int miles) { numberOfMilesTraveled = miles; }
void Plane::setSeatsPerClass(std::map<string, int> seats) { numberOfSeatsPerClass = seats; }
void Plane::setClassPricing(std::map<string, int> pricing) { classPricing = pricing; }

void Plane::addSeatClass(string className, int numSeats) {
    numberOfSeatsPerClass[className] = numSeats;
}

void Plane::addClassPrice(string className, int price) {
    classPricing[className] = price;
}

void Plane::updateNumberOfMilesTraveled(int miles) {
    numberOfMilesTraveled += miles;
}

void Plane::display() const {
    cout << "\n===== Plane Info =====" << endl;
    cout << "Plane ID: " << planeId << endl;
    cout << "Description: " << description << endl;
    cout << "Miles Traveled: " << numberOfMilesTraveled << endl;
    cout << "Seat Configuration:" << endl;
    for (const auto& pair : numberOfSeatsPerClass) {
        cout << "  " << pair.first << ": " << pair.second << " seats";
        auto priceIt = classPricing.find(pair.first);
        if (priceIt != classPricing.end()) {
            cout << " ($" << priceIt->second << ")";
        }
        cout << endl;
    }
    cout << "Total Seats: " << getTotalSeats() << endl;
    cout << "======================" << endl;
}