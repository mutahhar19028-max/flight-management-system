#include "ticket.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

Ticket::Ticket() : customerId(0), customerName(""), flightNum(0), ticketNumber(0),
                   ticketType(""), costOfFlight(0), discount(false), 
                   reasonForDiscount(""), dateTime(""), seatNumber(0) {}

Ticket::Ticket(int custId, string custName, int fNum, int tNum, string tType,
               int cost, bool disc, string reason, string dt, int seat)
    : customerId(custId), customerName(custName), flightNum(fNum), ticketNumber(tNum),
      ticketType(tType), costOfFlight(cost), discount(disc), 
      reasonForDiscount(reason), dateTime(dt), seatNumber(seat) {}

int Ticket::getCustomerId() const { return customerId; }
string Ticket::getCustomerName() const { return customerName; }
int Ticket::getFlightNum() const { return flightNum; }
int Ticket::getTicketNumber() const { return ticketNumber; }
string Ticket::getTicketType() const { return ticketType; }
int Ticket::getCostOfFlight() const { return costOfFlight; }
bool Ticket::getDiscount() const { return discount; }
string Ticket::getReasonForDiscount() const { return reasonForDiscount; }
string Ticket::getDateTime() const { return dateTime; }
int Ticket::getSeatNumber() const { return seatNumber; }

void Ticket::setCustomerId(int custId) { customerId = custId; }
void Ticket::setCustomerName(string custName) { customerName = custName; }
void Ticket::setFlightNum(int fNum) { flightNum = fNum; }
void Ticket::setTicketNumber(int tNum) { ticketNumber = tNum; }
void Ticket::setTicketType(string tType) { ticketType = tType; }
void Ticket::setCostOfFlight(int cost) { costOfFlight = cost; }
void Ticket::setDiscount(bool disc) { discount = disc; }
void Ticket::setReasonForDiscount(string reason) { reasonForDiscount = reason; }
void Ticket::setDateTime(string dt) { dateTime = dt; }
void Ticket::setSeatNumber(int seat) { seatNumber = seat; }

void Ticket::print() const {
    cout << "\n======== TICKET ========" << endl;
    cout << "Ticket Number: " << ticketNumber << endl;
    cout << "Customer ID: " << customerId << endl;
    cout << "Customer Name: " << customerName << endl;
    cout << "Flight Number: " << flightNum << endl;
    cout << "Date/Time: " << dateTime << endl;
    cout << "Ticket Type: " << ticketType << endl;
    cout << "Seat Number: " << seatNumber << endl;
    cout << "Cost: $" << costOfFlight;
    if (discount) {
        cout << " (Discount Applied: " << reasonForDiscount << ")";
    }
    cout << endl;
    cout << "========================" << endl;
}

string Ticket::toFileFormat() const {
    std::ostringstream oss;
    oss << customerId << "," << customerName << "," << flightNum << "," 
        << ticketNumber << "," << ticketType << "," << costOfFlight << "," 
        << (discount ? 1 : 0) << "," << reasonForDiscount << "," 
        << dateTime << "," << seatNumber;
    return oss.str();
}