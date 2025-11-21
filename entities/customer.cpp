#include "customer.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

// Constructors
Customer::Customer() : customerId(0), name(""), phone(""), dob(""), numOfFlights(0), milesTraveled(0) {}

Customer::Customer(int id, string n, string ph, string dateOfBirth, int flights, int miles)
    : customerId(id), name(n), phone(ph), dob(dateOfBirth), numOfFlights(flights), milesTraveled(miles) {}

// Getters
int Customer::getCustomerId() const { return customerId; }
string Customer::getName() const { return name; }
string Customer::getPhone() const { return phone; }
string Customer::getDob() const { return dob; }
int Customer::getNumOfFlights() const { return numOfFlights; }
int Customer::getMilesTraveled() const { return milesTraveled; }

// Setters
void Customer::setCustomerId(int id) { customerId = id; }
void Customer::setName(string n) { name = n; }
void Customer::setPhone(string ph) { phone = ph; }
void Customer::setDob(string dateOfBirth) { dob = dateOfBirth; }
void Customer::setNumOfFlights(int flights) { numOfFlights = flights; }
void Customer::setMilesTraveled(int miles) { milesTraveled = miles; }

// Update functions
void Customer::updateNumOfFlights(int increment) {
    numOfFlights += increment;
}

void Customer::updateMilesTraveled(int miles) {
    milesTraveled += miles;
}

// Display
void Customer::display() const {
    cout << "\n===== Customer Profile =====" << endl;
    cout << "Customer ID: " << customerId << endl;
    cout << "Name: " << name << endl;
    cout << "Phone: " << phone << endl;
    cout << "Date of Birth: " << dob << endl;
    cout << "Number of Flights: " << numOfFlights << endl;
    cout << "Miles Traveled: " << milesTraveled << endl;
    cout << "============================" << endl;
}