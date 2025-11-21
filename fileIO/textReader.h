#ifndef TEXT_READER_H
#define TEXT_READER_H

#include <string>
#include <vector>
#include "../entities/customer.h"
#include "../entities/flight.h"
#include "../entities/airport.h"
#include "../entities/plane.h"
#include "../entities/ticket.h"
#include "../entities/offer.h"

class TextReader {
public:
    static std::vector<Customer> readCustomers(const std::string& filename);
    static std::vector<Flight> readFlights(const std::string& filename);
    static std::vector<Airport> readAirports(const std::string& filename);
    static std::vector<Plane> readPlanes(const std::string& filename);
    static std::vector<Ticket> readTickets(const std::string& filename);
    static std::vector<Offer> readOffers(const std::string& filename);
    
private:
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string trim(const std::string& str);
};

#endif