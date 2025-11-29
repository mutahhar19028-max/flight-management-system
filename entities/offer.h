#ifndef OFFER_H
#define OFFER_H

#include <string>

using std::string;

class Offer {
private:
    int offerId;
    int flightNumber; // -1 means applicable to all flights
    string offerType; // "miles" or "flights"
    int discountAmount; // 10 means 10%

public:
    Offer();
    Offer(int id, int flightNum, string type, int discount);

    int getOfferId() const;
    int getFlightNumber() const;
    string getOfferType() const;
    int getDiscountAmount() const;

    void setOfferId(int id);
    void setFlightNumber(int flightNum);
    void setOfferType(string type);
    void setDiscountAmount(int discount);

    bool isApplicableToFlight(int flightNum) const;

    void display() const;
};

#endif