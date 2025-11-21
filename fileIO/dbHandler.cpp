#include "dbHandler.h"
#include <iostream>
#include <sstream>

bool DBHandler::databaseExists(const std::string& filename) {
    sqlite3* db;
    int result = sqlite3_open(filename.c_str(), &db);
    if (result != SQLITE_OK) {
        return false;
    }
    
    const char* checkTable = "SELECT name FROM sqlite_master WHERE type='table' AND name='customers';";
    sqlite3_stmt* stmt;
    bool exists = false;
    
    if (sqlite3_prepare_v2(db, checkTable, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = true;
        }
        sqlite3_finalize(stmt);
    }
    
    sqlite3_close(db);
    return exists;
}

bool DBHandler::createTables(sqlite3* db) {
    const char* sqlStatements[] = {
        "CREATE TABLE IF NOT EXISTS customers ("
        "id INTEGER PRIMARY KEY, name TEXT, phone TEXT, dob TEXT, "
        "numOfFlights INTEGER, milesTraveled INTEGER);",
        
        "CREATE TABLE IF NOT EXISTS flights ("
        "flightNumber INTEGER PRIMARY KEY, planeId INTEGER, day INTEGER, "
        "month INTEGER, year INTEGER, departureTime TEXT, destination TEXT);",
        
        "CREATE TABLE IF NOT EXISTS flight_pricing ("
        "flightNumber INTEGER, className TEXT, price INTEGER, "
        "FOREIGN KEY(flightNumber) REFERENCES flights(flightNumber));",
        
        "CREATE TABLE IF NOT EXISTS flight_seats_reserved ("
        "flightNumber INTEGER, className TEXT, reserved INTEGER, "
        "FOREIGN KEY(flightNumber) REFERENCES flights(flightNumber));",
        
        "CREATE TABLE IF NOT EXISTS airports ("
        "name TEXT PRIMARY KEY, country TEXT, weather TEXT, "
        "timeFromIslamabad INTEGER, milesFromIslamabad INTEGER);",
        
        "CREATE TABLE IF NOT EXISTS planes ("
        "planeId INTEGER PRIMARY KEY, description TEXT, milesTraveled INTEGER);",
        
        "CREATE TABLE IF NOT EXISTS plane_seats ("
        "planeId INTEGER, className TEXT, numSeats INTEGER, "
        "FOREIGN KEY(planeId) REFERENCES planes(planeId));",
        
        "CREATE TABLE IF NOT EXISTS plane_pricing ("
        "planeId INTEGER, className TEXT, price INTEGER, "
        "FOREIGN KEY(planeId) REFERENCES planes(planeId));",
        
        "CREATE TABLE IF NOT EXISTS tickets ("
        "ticketNumber INTEGER PRIMARY KEY, customerId INTEGER, customerName TEXT, "
        "flightNum INTEGER, ticketType TEXT, cost INTEGER, discount INTEGER, "
        "reasonForDiscount TEXT, dateTime TEXT, seatNumber INTEGER);",
        
        "CREATE TABLE IF NOT EXISTS offers ("
        "offerId INTEGER PRIMARY KEY, flightNumber INTEGER, offerType TEXT, "
        "discountAmount INTEGER);",
        
        "CREATE TABLE IF NOT EXISTS waiting_lists ("
        "flightId INTEGER, customerId INTEGER);",
        
        "CREATE TABLE IF NOT EXISTS metadata ("
        "key TEXT PRIMARY KEY, value INTEGER);"
    };
    
    char* errMsg;
    for (const char* sql : sqlStatements) {
        if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
    }
    
    return true;
}

bool DBHandler::initializeDatabase(const std::string& filename) {
    sqlite3* db;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    bool success = createTables(db);
    sqlite3_close(db);
    return success;
}

bool DBHandler::saveCustomers(sqlite3* db, const std::vector<Customer>& customers) {
    sqlite3_exec(db, "DELETE FROM customers;", nullptr, nullptr, nullptr);
    
    const char* sql = "INSERT INTO customers (id, name, phone, dob, numOfFlights, milesTraveled) "
                      "VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    for (const auto& customer : customers) {
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, customer.getCustomerId());
            sqlite3_bind_text(stmt, 2, customer.getName().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, customer.getPhone().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, customer.getDob().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 5, customer.getNumOfFlights());
            sqlite3_bind_int(stmt, 6, customer.getMilesTraveled());
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    return true;
}

bool DBHandler::saveFlights(sqlite3* db, const std::vector<Flight>& flights) {
    sqlite3_exec(db, "DELETE FROM flights;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "DELETE FROM flight_pricing;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "DELETE FROM flight_seats_reserved;", nullptr, nullptr, nullptr);
    
    const char* flightSql = "INSERT INTO flights (flightNumber, planeId, day, month, year, departureTime, destination) "
                            "VALUES (?, ?, ?, ?, ?, ?, ?);";
    const char* pricingSql = "INSERT INTO flight_pricing (flightNumber, className, price) VALUES (?, ?, ?);";
    const char* seatsSql = "INSERT INTO flight_seats_reserved (flightNumber, className, reserved) VALUES (?, ?, ?);";
    
    for (const auto& flight : flights) {
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, flightSql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, flight.getFlightNumber());
            sqlite3_bind_int(stmt, 2, flight.getPlaneId());
            sqlite3_bind_int(stmt, 3, flight.getDay());
            sqlite3_bind_int(stmt, 4, flight.getMonth());
            sqlite3_bind_int(stmt, 5, flight.getYear());
            sqlite3_bind_text(stmt, 6, flight.getDepartureTime().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 7, flight.getDestination().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        auto pricing = flight.getTicketPricing();
        for (const auto& pair : pricing) {
            if (sqlite3_prepare_v2(db, pricingSql, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, flight.getFlightNumber());
                sqlite3_bind_text(stmt, 2, pair.first.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 3, pair.second);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
        
        auto reserved = flight.getSeatsReserved();
        for (const auto& pair : reserved) {
            if (sqlite3_prepare_v2(db, seatsSql, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, flight.getFlightNumber());
                sqlite3_bind_text(stmt, 2, pair.first.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 3, pair.second);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
    }
    
    return true;
}

bool DBHandler::saveAirports(sqlite3* db, const std::vector<Airport>& airports) {
    sqlite3_exec(db, "DELETE FROM airports;", nullptr, nullptr, nullptr);
    
    const char* sql = "INSERT INTO airports (name, country, weather, timeFromIslamabad, milesFromIslamabad) "
                      "VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    for (const auto& airport : airports) {
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, airport.getName().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, airport.getCountry().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, airport.getWeather().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 4, airport.getTimeFromIslamabad());
            sqlite3_bind_int(stmt, 5, airport.getMilesFromIslamabad());
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    return true;
}

bool DBHandler::savePlanes(sqlite3* db, const std::vector<Plane>& planes) {
    sqlite3_exec(db, "DELETE FROM planes;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "DELETE FROM plane_seats;", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "DELETE FROM plane_pricing;", nullptr, nullptr, nullptr);
    
    const char* planeSql = "INSERT INTO planes (planeId, description, milesTraveled) VALUES (?, ?, ?);";
    const char* seatsSql = "INSERT INTO plane_seats (planeId, className, numSeats) VALUES (?, ?, ?);";
    const char* pricingSql = "INSERT INTO plane_pricing (planeId, className, price) VALUES (?, ?, ?);";
    
    for (const auto& plane : planes) {
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, planeSql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, plane.getPlaneId());
            sqlite3_bind_text(stmt, 2, plane.getDescription().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 3, plane.getNumberOfMilesTraveled());
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        auto seats = plane.getSeatsPerClass();
        for (const auto& pair : seats) {
            if (sqlite3_prepare_v2(db, seatsSql, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, plane.getPlaneId());
                sqlite3_bind_text(stmt, 2, pair.first.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 3, pair.second);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
        
        auto prices = plane.getClassPricing();
        for (const auto& pair : prices) {
            if (sqlite3_prepare_v2(db, pricingSql, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, plane.getPlaneId());
                sqlite3_bind_text(stmt, 2, pair.first.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmt, 3, pair.second);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
    }
    
    return true;
}

bool DBHandler::saveTickets(sqlite3* db, const std::vector<Ticket>& tickets) {
    sqlite3_exec(db, "DELETE FROM tickets;", nullptr, nullptr, nullptr);
    
    const char* sql = "INSERT INTO tickets (ticketNumber, customerId, customerName, flightNum, ticketType, "
                      "cost, discount, reasonForDiscount, dateTime, seatNumber) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    for (const auto& ticket : tickets) {
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, ticket.getTicketNumber());
            sqlite3_bind_int(stmt, 2, ticket.getCustomerId());
            sqlite3_bind_text(stmt, 3, ticket.getCustomerName().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 4, ticket.getFlightNum());
            sqlite3_bind_text(stmt, 5, ticket.getTicketType().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 6, ticket.getCostOfFlight());
            sqlite3_bind_int(stmt, 7, ticket.getDiscount() ? 1 : 0);
            sqlite3_bind_text(stmt, 8, ticket.getReasonForDiscount().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 9, ticket.getDateTime().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 10, ticket.getSeatNumber());
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    return true;
}

bool DBHandler::saveOffers(sqlite3* db, const std::vector<Offer>& offers) {
    sqlite3_exec(db, "DELETE FROM offers;", nullptr, nullptr, nullptr);
    
    const char* sql = "INSERT INTO offers (offerId, flightNumber, offerType, discountAmount) "
                      "VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    for (const auto& offer : offers) {
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, offer.getOfferId());
            sqlite3_bind_int(stmt, 2, offer.getFlightNumber());
            sqlite3_bind_text(stmt, 3, offer.getOfferType().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 4, offer.getDiscountAmount());
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    return true;
}

bool DBHandler::saveWaitingLists(sqlite3* db, const std::map<int, std::vector<int>>& waitingLists) {
    sqlite3_exec(db, "DELETE FROM waiting_lists;", nullptr, nullptr, nullptr);
    
    const char* sql = "INSERT INTO waiting_lists (flightId, customerId) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    for (const auto& pair : waitingLists) {
        for (int custId : pair.second) {
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_int(stmt, 1, pair.first);
                sqlite3_bind_int(stmt, 2, custId);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
    }
    
    return true;
}

bool DBHandler::saveMetadata(sqlite3* db, int lastCustomerId, int lastTicketId, int lastFlightId) {
    sqlite3_exec(db, "DELETE FROM metadata;", nullptr, nullptr, nullptr);
    
    const char* sql = "INSERT INTO metadata (key, value) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    std::string keys[] = {"lastCustomerId", "lastTicketId", "lastFlightId"};
    int values[] = {lastCustomerId, lastTicketId, lastFlightId};
    
    for (int i = 0; i < 3; ++i) {
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, keys[i].c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmt, 2, values[i]);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    return true;
}

bool DBHandler::saveDatabase(const std::string& filename, const SystemData& data) {
    sqlite3* db;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    
    bool success = true;
    success = success && saveCustomers(db, data.customers);
    success = success && saveFlights(db, data.flights);
    success = success && saveAirports(db, data.airports);
    success = success && savePlanes(db, data.planes);
    success = success && saveTickets(db, data.tickets);
    success = success && saveOffers(db, data.offers);
    success = success && saveWaitingLists(db, data.waitingLists);
    success = success && saveMetadata(db, data.lastCustomerId, data.lastTicketId, data.lastFlightId);
    
    if (success) {
        sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    } else {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    }
    
    sqlite3_close(db);
    return success;
}

bool DBHandler::loadCustomers(sqlite3* db, std::vector<Customer>& customers) {
    const char* sql = "SELECT id, name, phone, dob, numOfFlights, milesTraveled FROM customers;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Customer customer;
        customer.setCustomerId(sqlite3_column_int(stmt, 0));
        customer.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        customer.setPhone(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        customer.setDob(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        customer.setNumOfFlights(sqlite3_column_int(stmt, 4));
        customer.setMilesTraveled(sqlite3_column_int(stmt, 5));
        customers.push_back(customer);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBHandler::loadFlights(sqlite3* db, std::vector<Flight>& flights) {
    const char* sql = "SELECT flightNumber, planeId, day, month, year, departureTime, destination FROM flights;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Flight flight;
        int flightNum = sqlite3_column_int(stmt, 0);
        flight.setFlightNumber(flightNum);
        flight.setPlaneId(sqlite3_column_int(stmt, 1));
        flight.setDay(sqlite3_column_int(stmt, 2));
        flight.setMonth(sqlite3_column_int(stmt, 3));
        flight.setYear(sqlite3_column_int(stmt, 4));
        flight.setDepartureTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        flight.setDestination(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        
        sqlite3_stmt* pricingStmt;
        std::string pricingSql = "SELECT className, price FROM flight_pricing WHERE flightNumber = " + std::to_string(flightNum) + ";";
        if (sqlite3_prepare_v2(db, pricingSql.c_str(), -1, &pricingStmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(pricingStmt) == SQLITE_ROW) {
                std::string className = reinterpret_cast<const char*>(sqlite3_column_text(pricingStmt, 0));
                int price = sqlite3_column_int(pricingStmt, 1);
                flight.addTicketPrice(className, price);
            }
            sqlite3_finalize(pricingStmt);
        }
        
        sqlite3_stmt* seatsStmt;
        std::string seatsSql = "SELECT className, reserved FROM flight_seats_reserved WHERE flightNumber = " + std::to_string(flightNum) + ";";
        if (sqlite3_prepare_v2(db, seatsSql.c_str(), -1, &seatsStmt, nullptr) == SQLITE_OK) {
            std::map<std::string, int> reserved;
            while (sqlite3_step(seatsStmt) == SQLITE_ROW) {
                std::string className = reinterpret_cast<const char*>(sqlite3_column_text(seatsStmt, 0));
                int count = sqlite3_column_int(seatsStmt, 1);
                reserved[className] = count;
            }
            flight.setSeatsReserved(reserved);
            sqlite3_finalize(seatsStmt);
        }
        
        flights.push_back(flight);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBHandler::loadAirports(sqlite3* db, std::vector<Airport>& airports) {
    const char* sql = "SELECT name, country, weather, timeFromIslamabad, milesFromIslamabad FROM airports;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Airport airport;
        airport.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        airport.setCountry(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        airport.setWeather(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        airport.setTimeFromIslamabad(sqlite3_column_int(stmt, 3));
        airport.setMilesFromIslamabad(sqlite3_column_int(stmt, 4));
        airports.push_back(airport);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBHandler::loadPlanes(sqlite3* db, std::vector<Plane>& planes) {
    const char* sql = "SELECT planeId, description, milesTraveled FROM planes;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Plane plane;
        int planeId = sqlite3_column_int(stmt, 0);
        plane.setPlaneId(planeId);
        plane.setDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        plane.setNumberOfMilesTraveled(sqlite3_column_int(stmt, 2));
        
        sqlite3_stmt* seatsStmt;
        std::string seatsSql = "SELECT className, numSeats FROM plane_seats WHERE planeId = " + std::to_string(planeId) + ";";
        if (sqlite3_prepare_v2(db, seatsSql.c_str(), -1, &seatsStmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(seatsStmt) == SQLITE_ROW) {
                std::string className = reinterpret_cast<const char*>(sqlite3_column_text(seatsStmt, 0));
                int numSeats = sqlite3_column_int(seatsStmt, 1);
                plane.addSeatClass(className, numSeats);
            }
            sqlite3_finalize(seatsStmt);
        }
        
        sqlite3_stmt* pricingStmt;
        std::string pricingSql = "SELECT className, price FROM plane_pricing WHERE planeId = " + std::to_string(planeId) + ";";
        if (sqlite3_prepare_v2(db, pricingSql.c_str(), -1, &pricingStmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(pricingStmt) == SQLITE_ROW) {
                std::string className = reinterpret_cast<const char*>(sqlite3_column_text(pricingStmt, 0));
                int price = sqlite3_column_int(pricingStmt, 1);
                plane.addClassPrice(className, price);
            }
            sqlite3_finalize(pricingStmt);
        }
        
        planes.push_back(plane);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBHandler::loadTickets(sqlite3* db, std::vector<Ticket>& tickets) {
    const char* sql = "SELECT ticketNumber, customerId, customerName, flightNum, ticketType, "
                      "cost, discount, reasonForDiscount, dateTime, seatNumber FROM tickets;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Ticket ticket;
        ticket.setTicketNumber(sqlite3_column_int(stmt, 0));
        ticket.setCustomerId(sqlite3_column_int(stmt, 1));
        ticket.setCustomerName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        ticket.setFlightNum(sqlite3_column_int(stmt, 3));
        ticket.setTicketType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        ticket.setCostOfFlight(sqlite3_column_int(stmt, 5));
        ticket.setDiscount(sqlite3_column_int(stmt, 6) == 1);
        ticket.setReasonForDiscount(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
        ticket.setDateTime(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)));
        ticket.setSeatNumber(sqlite3_column_int(stmt, 9));
        tickets.push_back(ticket);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBHandler::loadOffers(sqlite3* db, std::vector<Offer>& offers) {
    const char* sql = "SELECT offerId, flightNumber, offerType, discountAmount FROM offers;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Offer offer;
        offer.setOfferId(sqlite3_column_int(stmt, 0));
        offer.setFlightNumber(sqlite3_column_int(stmt, 1));
        offer.setOfferType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        offer.setDiscountAmount(sqlite3_column_int(stmt, 3));
        offers.push_back(offer);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBHandler::loadWaitingLists(sqlite3* db, std::map<int, std::vector<int>>& waitingLists) {
    const char* sql = "SELECT flightId, customerId FROM waiting_lists;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int flightId = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        waitingLists[flightId].push_back(customerId);
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBHandler::loadMetadata(sqlite3* db, int& lastCustomerId, int& lastTicketId, int& lastFlightId) {
    const char* sql = "SELECT key, value FROM metadata;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string key = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int value = sqlite3_column_int(stmt, 1);
        
        if (key == "lastCustomerId") lastCustomerId = value;
        else if (key == "lastTicketId") lastTicketId = value;
        else if (key == "lastFlightId") lastFlightId = value;
    }
    
    sqlite3_finalize(stmt);
    return true;
}

bool DBHandler::loadDatabase(const std::string& filename, SystemData& data) {
    sqlite3* db;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    bool success = true;
    success = success && loadCustomers(