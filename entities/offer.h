#ifndef OFFER_H
#define OFFER_H

#include <string>

using std::string;

class Offer {
private:
    int offerId;
    int flightNumber; // -1 means applicable to all flights
    string offerType; // "miles" or "flights"
    int discountAmount; // percentage discount (e.g., 10 for 10%)

public:
    // Constructors
    Offer();
    Offer(int id, int flightNum, string type, int discount);

    // Getters
    int getOfferId() const;
    int getFlightNumber() const;
    string getOfferType() const;
    int getDiscountAmount() const;

    // Setters
    void setOfferId(int id);
    void setFlightNumber(int flightNum);
    void setOfferType(string type);
    void setDiscountAmount(int discount);

    // Utility
    bool isApplicableToFlight(int flightNum) const;

    // Display
    void display() const;
};

#endif