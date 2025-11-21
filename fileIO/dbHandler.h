#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include <string>
#include <vector>
#include <map>
#include "../entities/customer.h"
#include "../entities/flight.h"
#include "../entities/airport.h"
#include "../entities/plane.h"
#include "../entities/ticket.h"
#include "../entities/offer.h"
#include "../sqlite/sqlite3.h"

class DBHandler {
public:
    struct SystemData {
        std::vector<Customer> customers;
        std::vector<Flight> flights;
        std::vector<Airport> airports;
        std::vector<Plane> planes;
        std::vector<Ticket> tickets;
        std::vector<Offer> offers;
        std::map<int, std::vector<int>> waitingLists;
        int lastCustomerId;
        int lastTicketId;
        int lastFlightId;
    };
    
    static bool databaseExists(const std::string& filename);
    static bool initializeDatabase(const std::string& filename);
    static bool saveDatabase(const std::string& filename, const SystemData& data);
    static bool loadDatabase(const std::string& filename, SystemData& data);
    
private:
    static bool createTables(sqlite3* db);
    static bool saveCustomers(sqlite3* db, const std::vector<Customer>& customers);
    static bool saveFlights(sqlite3* db, const std::vector<Flight>& flights);
    static bool saveAirports(sqlite3* db, const std::vector<Airport>& airports);
    static bool savePlanes(sqlite3* db, const std::vector<Plane>& planes);
    static bool saveTickets(sqlite3* db, const std::vector<Ticket>& tickets);
    static bool saveOffers(sqlite3* db, const std::vector<Offer>& offers);
    static bool saveWaitingLists(sqlite3* db, const std::map<int, std::vector<int>>& waitingLists);
    static bool saveMetadata(sqlite3* db, int lastCustomerId, int lastTicketId, int lastFlightId);
    
    static bool loadCustomers(sqlite3* db, std::vector<Customer>& customers);
    static bool loadFlights(sqlite3* db, std::vector<Flight>& flights);
    static bool loadAirports(sqlite3* db, std::vector<Airport>& airports);
    static bool loadPlanes(sqlite3* db, std::vector<Plane>& planes);
    static bool loadTickets(sqlite3* db, std::vector<Ticket>& tickets);
    static bool loadOffers(sqlite3* db, std::vector<Offer>& offers);
    static bool loadWaitingLists(sqlite3* db, std::map<int, std::vector<int>>& waitingLists);
    static bool loadMetadata(sqlite3* db, int& lastCustomerId, int& lastTicketId, int& lastFlightId);
};

#endif