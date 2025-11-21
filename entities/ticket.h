#ifndef TICKET_H
#define TICKET_H

#include <string>

using std::string;

class Ticket {
private:
    int customerId;
    string customerName;
    int flightNum;
    int ticketNumber;
    string ticketType; // Economy, Business, etc.
    int costOfFlight;
    bool discount; // 0 = no discount, 1 = discount applied
    string reasonForDiscount; // "miles", "flights", "seasonal", etc.
    string dateTime; // format: DD/MM/YYYY HH:MM
    int seatNumber;

public:
    // Constructors
    Ticket();
    Ticket(int custId, string custName, int fNum, int tNum, string tType, 
           int cost, bool disc, string reason, string dt, int seat);

    // Getters
    int getCustomerId() const;
    string getCustomerName() const;
    int getFlightNum() const;
    int getTicketNumber() const;
    string getTicketType() const;
    int getCostOfFlight() const;
    bool getDiscount() const;
    string getReasonForDiscount() const;
    string getDateTime() const;
    int getSeatNumber() const;

    // Setters
    void setCustomerId(int custId);
    void setCustomerName(string custName);
    void setFlightNum(int fNum);
    void setTicketNumber(int tNum);
    void setTicketType(string tType);
    void setCostOfFlight(int cost);
    void setDiscount(bool disc);
    void setReasonForDiscount(string reason);
    void setDateTime(string dt);
    void setSeatNumber(int seat);

    // Display
    void print() const;
    string toFileFormat() const; // For saving to tickets.txt
};

#endif