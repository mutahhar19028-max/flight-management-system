#include "offer.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

// Constructors
Offer::Offer() : offerId(0), flightNumber(-1), offerType(""), discountAmount(0) {}

Offer::Offer(int id, int flightNum, string type, int discount)
    : offerId(id), flightNumber(flightNum), offerType(type), discountAmount(discount) {}

// Getters
int Offer::getOfferId() const { return offerId; }
int Offer::getFlightNumber() const { return flightNumber; }
string Offer::getOfferType() const { return offerType; }
int Offer::getDiscountAmount() const { return discountAmount; }

// Setters
void Offer::setOfferId(int id) { offerId = id; }
void Offer::setFlightNumber(int flightNum) { flightNumber = flightNum; }
void Offer::setOfferType(string type) { offerType = type; }
void Offer::setDiscountAmount(int discount) { discountAmount = discount; }

// Utility
bool Offer::isApplicableToFlight(int flightNum) const {
    return (flightNumber == -1 || flightNumber == flightNum);
}

// Display
void Offer::display() const {
    cout << "\n===== Offer Details =====" << endl;
    cout << "Offer ID: " << offerId << endl;
    cout << "Flight Number: " << (flightNumber == -1 ? "All Flights" : to_string(flightNumber)) << endl;
    cout << "Offer Type: " << offerType << endl;
    cout << "Discount: " << discountAmount << "%" << endl;
    cout << "=========================" << endl;
}