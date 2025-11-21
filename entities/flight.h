#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <map>
#include <vector>

using std::string;

class Flight {
private:
    int flightNumber;
    int planeId;
    int day;
    int month;
    int year;
    string departureTime; // format: HH:MM
    string destination; // airport name
    std::map<string, int> ticketPricing; // e.g., {"Economy": 400, "Business": 1000}
    std::map<string, int> seatsReserved; // e.g., {"Economy": 45, "Business": 10}

public:
    // Constructors
    Flight();
    Flight(int flightNum, int plnId, int d, int m, int y, string time, string dest);

    // Getters
    int getFlightNumber() const;
    int getPlaneId() const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    string getDepartureTime() const;
    string getDestination() const;
    std::map<string, int> getTicketPricing() const;
    int getPriceForClass(string className) const;
    std::map<string, int> getSeatsReserved() const;
    int getSeatsReservedForClass(string className) const;

    // Setters
    void setFlightNumber(int flightNum);
    void setPlaneId(int plnId);
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);
    void setDepartureTime(string time);
    void setDestination(string dest);
    void setTicketPricing(std::map<string, int> pricing);
    void addTicketPrice(string className, int price);
    void setSeatsReserved(std::map<string, int> reserved);

    // Seat management
    bool reserveSeat(string className);
    bool cancelSeat(string className);
    int getAvailableSeats(string className, int totalSeatsInClass) const;

    // Display
    void display() const;
    string getDateString() const;
};

#endif