#ifndef TEXT_WRITER_H
#define TEXT_WRITER_H

#include <string>
#include <vector>
#include "../entities/customer.h"
#include "../entities/flight.h"
#include "../entities/airport.h"
#include "../entities/plane.h"
#include "../entities/ticket.h"
#include "../entities/offer.h"

class TextWriter {
public:
    static bool writeCustomers(const std::string& filename, const std::vector<Customer>& customers);
    static bool writeFlights(const std::string& filename, const std::vector<Flight>& flights);
    static bool writeAirports(const std::string& filename, const std::vector<Airport>& airports);
    static bool writePlanes(const std::string& filename, const std::vector<Plane>& planes);
    static bool writeTickets(const std::string& filename, const std::vector<Ticket>& tickets);
    static bool writeOffers(const std::string& filename, const std::vector<Offer>& offers);
};

#endif