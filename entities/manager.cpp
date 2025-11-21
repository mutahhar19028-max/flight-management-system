#include "manager.h"
#include "../fileIO/textReader.h"
#include "../fileIO/textWriter.h"
#include "../fileIO/dbHandler.h"
#include "../utilities/idGenerator.h"
#include "../utilities/dateTime.h"
#include "../utilities/validator.h"
#include <iostream>
#include <algorithm>

SystemManager::SystemManager() : _m_initialized(false), _m_dbPath("") {}

SystemManager::~SystemManager() {
    if (_m_initialized) {
        _saveToDatabase();
    }
}

bool SystemManager::initialize(const std::string& dbPath) {
    _m_dbPath = dbPath;
    
    if (DBHandler::databaseExists(dbPath)) {
        std::cout << "Loading system from database..." << std::endl;
        if (_loadFromDatabase()) {
            _m_initialized = true;
            std::cout << "System loaded successfully from database!" << std::endl;
            return true;
        }
    } else {
        std::cout << "Database not found. Loading from text files..." << std::endl;
        if (!DBHandler::initializeDatabase(dbPath)) {
            std::cerr << "Failed to initialize database!" << std::endl;
            return false;
        }
        if (_loadFromTextFiles()) {
            std::cout << "Data loaded from text files. Saving to database..." << std::endl;
            if (_saveToDatabase()) {
                _m_initialized = true;
                std::cout << "System initialized successfully!" << std::endl;
                return true;
            }
        }
    }
    
    return false;
}

bool SystemManager::_loadFromDatabase() {
    DBHandler::SystemData data;
    if (!DBHandler::loadDatabase(_m_dbPath, data)) {
        return false;
    }
    
    IdGenerator::setLastCustomerId(data.lastCustomerId);
    IdGenerator::setLastTicketId(data.lastTicketId);
    IdGenerator::setLastFlightId(data.lastFlightId);
    
    _m_customers.clear();
    for (const auto& customer : data.customers) {
        _m_customers.insert(customer);
    }
    
    _m_flights.clear();
    for (const auto& flight : data.flights) {
        _m_flights.insertAtTail(flight);
    }
    
    _m_airports.clear();
    for (const auto& airport : data.airports) {
        _m_airports.insertAtTail(airport);
    }
    
    _m_planes.clear();
    for (const auto& plane : data.planes) {
        _m_planes.insertAtTail(plane);
    }
    
    _m_tickets = data.tickets;
    _m_offers = data.offers;
    _m_waitingLists.clear();
    
    for (const auto& pair : data.waitingLists) {
        int maxSize = 0;
        Plane* plane = _findPlaneById(0);
        if (plane) {
            maxSize = (plane->getTotalSeats() / 100) * 5;
        }
        _m_waitingLists[pair.first] = ds::Queue<int>(maxSize > 0 ? maxSize : 5);
        for (int custId : pair.second) {
            _m_waitingLists[pair.first].enqueue(custId);
        }
    }
    
    return true;
}

bool SystemManager::_loadFromTextFiles() {
    std::vector<Customer> customers = TextReader::readCustomers("data/customers.txt");
    std::vector<Flight> flights = TextReader::readFlights("data/flights.txt");
    std::vector<Airport> airports = TextReader::readAirports("data/airports.txt");
    std::vector<Plane> planes = TextReader::readPlanes("data/planes.txt");
    std::vector<Ticket> tickets = TextReader::readTickets("data/tickets.txt");
    std::vector<Offer> offers = TextReader::readOffers("data/offers.txt");
    
    if (customers.empty() || airports.empty() || planes.empty()) {
        std::cerr << "Error: Failed to load required data files" << std::endl;
        return false;
    }
    
    int maxCustomerId = 1000;
    for (const auto& customer : customers) {
        _m_customers.insert(customer);
        if (customer.getCustomerId() > maxCustomerId) {
            maxCustomerId = customer.getCustomerId();
        }
    }
    IdGenerator::setLastCustomerId(maxCustomerId);
    
    int maxFlightId = 100;
    for (const auto& flight : flights) {
        _m_flights.insertAtTail(flight);
        if (flight.getFlightNumber() > maxFlightId) {
            maxFlightId = flight.getFlightNumber();
        }
    }
    IdGenerator::setLastFlightId(maxFlightId);
    
    for (const auto& airport : airports) {
        _m_airports.insertAtTail(airport);
    }
    
    for (const auto& plane : planes) {
        _m_planes.insertAtTail(plane);
    }
    
    int maxTicketId = 5000;
    for (const auto& ticket : tickets) {
        _m_tickets.push_back(ticket);
        if (ticket.getTicketNumber() > maxTicketId) {
            maxTicketId = ticket.getTicketNumber();
        }
    }
    IdGenerator::setLastTicketId(maxTicketId);
    
    _m_offers = offers;
    
    return true;
}

bool SystemManager::_saveToDatabase() {
    DBHandler::SystemData data;
    
    _m_customers.getAllNodes(data.customers);
    
    data.flights.clear();
    for (int i = 0; i < _m_flights.size(); ++i) {
        data.flights.push_back(_m_flights.getAt(i));
    }
    
    data.airports.clear();
    for (int i = 0; i < _m_airports.size(); ++i) {
        data.airports.push_back(_m_airports.getAt(i));
    }
    
    data.planes.clear();
    for (int i = 0; i < _m_planes.size(); ++i) {
        data.planes.push_back(_m_planes.getAt(i));
    }
    
    data.tickets = _m_tickets;
    data.offers = _m_offers;
    
    data.waitingLists.clear();
    for (const auto& pair : _m_waitingLists) {
        data.waitingLists[pair.first] = std::vector<int>();
        ds::Queue<int> tempQueue = pair.second;
        while (!tempQueue.isEmpty()) {
            data.waitingLists[pair.first].push_back(tempQueue.front());
            tempQueue.dequeue();
        }
    }
    
    data.lastCustomerId = IdGenerator::getLastCustomerId();
    data.lastTicketId = IdGenerator::getLastTicketId();
    data.lastFlightId = IdGenerator::getLastFlightId();
    
    return DBHandler::saveDatabase(_m_dbPath, data);
}

Plane* SystemManager::_findPlaneById(int planeId) {
    return _m_planes.find([planeId](const Plane& p) {
        return p.getPlaneId() == planeId;
    });
}

Airport* SystemManager::_findAirportByName(const std::string& name) {
    return _m_airports.find([&name](const Airport& a) {
        return a.getName() == name;
    });
}

int SystemManager::_getNextSeatNumber(int flightNumber, const std::string& ticketType) {
    Flight* flight = findFlight(flightNumber);
    if (!flight) return 0;
    
    Plane* plane = _findPlaneById(flight->getPlaneId());
    if (!plane) return 0;
    
    int seatStart = 0;
    if (ticketType == "Business") {
        seatStart = 0;
    } else if (ticketType == "Economy") {
        seatStart = plane->getSeatsForClass("Business");
    }
    
    int reserved = flight->getSeatsReservedForClass(ticketType);
    return seatStart + reserved;
}

bool SystemManager::_checkOfferEligibility(const Customer& customer, const Offer& offer) {
    if (offer.getOfferType() == "miles") {
        return customer.getMilesTraveled() >= 50000;
    } else if (offer.getOfferType() == "flights") {
        return customer.getNumOfFlights() >= 10;
    }
    return false;
}

int SystemManager::_calculateDiscountedPrice(int basePrice, const Customer& customer, int flightNumber) {
    int finalPrice = basePrice;
    int bestDiscount = 0;
    std::string discountReason = "";
    
    for (const auto& offer : _m_offers) {
        if (offer.isApplicableToFlight(flightNumber) && _checkOfferEligibility(customer, offer)) {
            if (offer.getDiscountAmount() > bestDiscount) {
                bestDiscount = offer.getDiscountAmount();
                discountReason = offer.getOfferType();
            }
        }
    }
    
    if (bestDiscount > 0) {
        finalPrice = basePrice - (basePrice * bestDiscount / 100);
    }
    
    return finalPrice;
}

void SystemManager::_updateCustomerFromTickets(Customer& customer) {
    int totalFlights = 0;
    int totalMiles = 0;
    
    for (const auto& ticket : _m_tickets) {
        if (ticket.getCustomerId() == customer.getCustomerId()) {
            totalFlights++;
            Flight* flight = findFlight(ticket.getFlightNum());
            if (flight) {
                Airport* airport = _findAirportByName(flight->getDestination());
                if (airport) {
                    totalMiles += airport->getMilesFromIslamabad();
                }
            }
        }
    }
    
    customer.setNumOfFlights(totalFlights);
    customer.setMilesTraveled(totalMiles);
}

bool SystemManager::addCustomer(const std::string& name, const std::string& phone, const std::string& dob) {
    if (!Validator::isValidName(name) || !Validator::isValidPhone(phone) || !Validator::isValidDate(dob)) {
        std::cerr << "Invalid customer data!" << std::endl;
        return false;
    }
    
    int newId = IdGenerator::generateCustomerId();
    Customer customer(newId, name, phone, dob, 0, 0);
    _m_customers.insert(customer);
    
    _saveToDatabase();
    std::cout << "Customer added successfully! Customer ID: " << newId << std::endl;
    return true;
}

bool SystemManager::updateCustomer(int customerId, const std::string& name, const std::string& phone, const std::string& dob) {
    Customer* customer = findCustomer(customerId);
    if (!customer) {
        std::cerr << "Customer not found!" << std::endl;
        return false;
    }
    
    if (!name.empty() && Validator::isValidName(name)) {
        customer->setName(name);
    }
    if (!phone.empty() && Validator::isValidPhone(phone)) {
        customer->setPhone(phone);
    }
    if (!dob.empty() && Validator::isValidDate(dob)) {
        customer->setDob(dob);
    }
    
    _saveToDatabase();
    std::cout << "Customer updated successfully!" << std::endl;
    return true;
}

bool SystemManager::deleteCustomer(int customerId) {
    Customer tempCustomer;
    tempCustomer.setCustomerId(customerId);
    
    bool hasTickets = false;
    for (const auto& ticket : _m_tickets) {
        if (ticket.getCustomerId() == customerId) {
            hasTickets = true;
            break;
        }
    }
    
    if (hasTickets) {
        std::cerr << "Cannot delete customer with existing tickets!" << std::endl;
        return false;
    }
    
    _m_customers.remove(tempCustomer);
    _saveToDatabase();
    std::cout << "Customer deleted successfully!" << std::endl;
    return true;
}

Customer* SystemManager::findCustomer(int customerId) {
    return _m_customers.find([customerId](const Customer& c) {
        return c.getCustomerId() == customerId;
    });
}

void SystemManager::displayCustomer(int customerId) {
    Customer* customer = findCustomer(customerId);
    if (customer) {
        customer->display();
        
        std::cout << "\nTickets:" << std::endl;
        bool hasTickets = false;
        for (const auto& ticket : _m_tickets) {
            if (ticket.getCustomerId() == customerId) {
                ticket.print();
                hasTickets = true;
            }
        }
        if (!hasTickets) {
            std::cout << "No tickets found for this customer." << std::endl;
        }
    } else {
        std::cout << "Customer not found!" << std::endl;
    }
}

void SystemManager::displayAllCustomers() {
    std::cout << "\n========== ALL CUSTOMERS ==========" << std::endl;
    _m_customers.inorderTraverse([](const Customer& c) {
        std::cout << "ID: " << c.getCustomerId() 
                  << " | Name: " << c.getName() 
                  << " | Flights: " << c.getNumOfFlights() 
                  << " | Miles: " << c.getMilesTraveled() << std::endl;
    });
    std::cout << "====================================" << std::endl;
}

bool SystemManager::addFlight(int planeId, int day, int month, int year, const std::string& time, const std::string& destination) {
    Plane* plane = _findPlaneById(planeId);
    if (!plane) {
        std::cerr << "Plane not found!" << std::endl;
        return false;
    }
    
    Airport* airport = _findAirportByName(destination);
    if (!airport) {
        std::cerr << "Airport not found!" << std::endl;
        return false;
    }
    
    if (!DateTime::isValidDate(day, month, year) || !DateTime::isValidTime(time)) {
        std::cerr << "Invalid date or time!" << std::endl;
        return false;
    }
    
    int newFlightNumber = IdGenerator::generateFlightId();
    Flight flight(newFlightNumber, planeId, day, month, year, time, destination);
    
    auto pricing = plane->getClassPricing();
    for (const auto& pair : pricing) {
        flight.addTicketPrice(pair.first, pair.second);
    }
    
    _m_flights.insertAtTail(flight);
    
    int maxWaitingSize = (plane->getTotalSeats() / 100) * 5;
    if (maxWaitingSize == 0) maxWaitingSize = 5;
    _m_waitingLists[newFlightNumber] = ds::Queue<int>(maxWaitingSize);
    
    _saveToDatabase();
    std::cout << "Flight added successfully! Flight Number: " << newFlightNumber << std::endl;
    return true;
}

bool SystemManager::updateFlight(int flightNumber, int day, int month, int year, const std::string& time) {
    Flight* flight = findFlight(flightNumber);
    if (!flight) {
        std::cerr << "Flight not found!" << std::endl;
        return false;
    }
    
    if (!DateTime::isValidDate(day, month, year) || !DateTime::isValidTime(time)) {
        std::cerr << "Invalid date or time!" << std::endl;
        return false;
    }
    
    flight->setDay(day);
    flight->setMonth(month);
    flight->setYear(year);
    flight->setDepartureTime(time);
    
    _saveToDatabase();
    std::cout << "Flight updated successfully!" << std::endl;
    return true;
}

bool SystemManager::deleteFlight(int flightNumber) {
    bool hasTickets = false;
    for (const auto& ticket : _m_tickets) {
        if (ticket.getFlightNum() == flightNumber) {
            hasTickets = true;
            break;
        }
    }
    
    if (hasTickets) {
        std::cerr << "Cannot delete flight with existing tickets!" << std::endl;
        return false;
    }
    
    for (int i = 0; i < _m_flights.size(); ++i) {
        if (_m_flights.getAt(i).getFlightNumber() == flightNumber) {
            _m_flights.removeAt(i);
            _m_waitingLists.erase(flightNumber);
            _saveToDatabase();
            std::cout << "Flight deleted successfully!" << std::endl;
            return true;
        }
    }
    
    std::cerr << "Flight not found!" << std::endl;
    return false;
}

Flight* SystemManager::findFlight(int flightNumber) {
    return _m_flights.find([flightNumber](const Flight& f) {
        return f.getFlightNumber() == flightNumber;
    });
}

void SystemManager::searchFlights(const std::string& destination) {
    std::cout << "\n========== FLIGHTS TO " << destination << " ==========" << std::endl;
    bool found = false;
    
    _m_flights.traverse([&](const Flight& f) {
        if (f.getDestination() == destination) {
            std::cout << "Flight #" << f.getFlightNumber() 
                      << " | Date: " << f.getDateString()
                      << " | Time: " << f.getDepartureTime() << std::endl;
            
            auto pricing = f.getTicketPricing();
            std::cout << "  Prices: ";
            for (const auto& pair : pricing) {
                std::cout << pair.first << ": $" << pair.second << "  ";
            }
            std::cout << std::endl;
            
            Plane* plane = _findPlaneById(f.getPlaneId());
            if (plane) {
                auto seats = plane->getSeatsPerClass();
                std::cout << "  Available Seats: ";
                for (const auto& pair : seats) {
                    int available = f.getAvailableSeats(pair.first, pair.second);
                    std::cout << pair.first << ": " << available << "  ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
            found = true;
        }
    });
    
    if (!found) {
        std::cout << "No flights found to " << destination << std::endl;
    }
    std::cout << "================================================" << std::endl;
}

void SystemManager::displayFlight(int flightNumber) {
    Flight* flight = findFlight(flightNumber);
    if (flight) {
        flight->display();
        
        Plane* plane = _findPlaneById(flight->getPlaneId());
        if (plane) {
            std::cout << "\nPlane Details:" << std::endl;
            plane->display();
        }
        
        Airport* airport = _findAirportByName(flight->getDestination());
        if (airport) {
            std::cout << "\nDestination Airport:" << std::endl;
            airport->display();
        }
    } else {
        std::cout << "Flight not found!" << std::endl;
    }
}

void SystemManager::displayAllFlights() {
    std::cout << "\n========== ALL FLIGHTS ==========" << std::endl;
    _m_flights.traverse([](const Flight& f) {
        std::cout << "Flight #" << f.getFlightNumber() 
                  << " | " << f.getDestination()
                  << " | " << f.getDateString()
                  << " @ " << f.getDepartureTime() << std::endl;
    });
    std::cout << "=================================" << std::endl;
}

bool SystemManager::reserveSeat(int customerId, int flightNumber, const std::string& ticketType) {
    Customer* customer = findCustomer(customerId);
    if (!customer) {
        std::cerr << "Customer not found!" << std::endl;
        return false;
    }
    
    Flight* flight = findFlight(flightNumber);
    if (!flight) {
        std::cerr << "Flight not found!" << std::endl;
        return false;
    }
    
    Plane* plane = _findPlaneById(flight->getPlaneId());
    if (!plane) {
        std::cerr << "Plane not found!" << std::endl;
        return false;
    }
    
    int totalSeats = plane->getSeatsForClass(ticketType);
    int availableSeats = flight->getAvailableSeats(ticketType, totalSeats);
    
    if (availableSeats <= 0) {
        std::cout << "Flight is full for " << ticketType << " class!" << std::endl;
        char choice;
        std::cout << "Would you like to be added to the waiting list? (y/n): ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            return addToWaitingList(customerId, flightNumber);
        }
        return false;
    }
    
    int basePrice = flight->getPriceForClass(ticketType);
    int finalPrice = _calculateDiscountedPrice(basePrice, *customer, flightNumber);
    bool hasDiscount = (finalPrice < basePrice);
    std::string discountReason = hasDiscount ? "loyalty" : "";
    
    int ticketNumber = IdGenerator::generateTicketId();
    int seatNumber = _getNextSeatNumber(flightNumber, ticketType);
    std::string dateTime = DateTime::formatDateTime(flight->getDay(), flight->getMonth(), 
                                                     flight->getYear(), flight->getDepartureTime());
    
    Ticket ticket(customerId, customer->getName(), flightNumber, ticketNumber, 
                  ticketType, finalPrice, hasDiscount, discountReason, dateTime, seatNumber);
    
    _m_tickets.push_back(ticket);
    flight->reserveSeat(ticketType);
    
    Airport* airport = _findAirportByName(flight->getDestination());
    if (airport) {
        customer->updateMilesTraveled(airport->getMilesFromIslamabad());
    }
    customer->updateNumOfFlights(1);
    
    _saveToDatabase();
    
    std::cout << "\n========== RESERVATION SUCCESSFUL ==========" << std::endl;
    ticket.print();
    std::cout << "============================================" << std::endl;
    
    return true;
}

bool SystemManager::cancelReservation(int ticketNumber) {
    auto it = std::find_if(_m_tickets.begin(), _m_tickets.end(), 
                           [ticketNumber](const Ticket& t) {
                               return t.getTicketNumber() == ticketNumber;
                           });
    
    if (it == _m_tickets.end()) {
        std::cerr << "Ticket not found!" << std::endl;
        return false;
    }
    
    Ticket ticket = *it;
    Flight* flight = findFlight(ticket.getFlightNum());
    Customer* customer = findCustomer(ticket.getCustomerId());
    
    if (flight) {
        flight->cancelSeat(ticket.getTicketType());
        processWaitingList(ticket.getFlightNum());
    }
    
    if (customer) {
        Airport* airport = flight ? _findAirportByName(flight->getDestination()) : nullptr;
        if (airport) {
            int miles = customer->getMilesTraveled() - airport->getMilesFromIslamabad();
            customer->setMilesTraveled(miles > 0 ? miles : 0);
        }
        int flights = customer->getNumOfFlights() - 1;
        customer->setNumOfFlights(flights > 0 ? flights : 0);
    }
    
    _m_tickets.erase(it);
    _saveToDatabase();
    
    std::cout << "Reservation cancelled successfully!" << std::endl;
    return true;
}

bool SystemManager::changeReservation(int ticketNumber, int newFlightNumber, const std::string& newTicketType) {
    Ticket* ticket = findTicket(ticketNumber);
    if (!ticket) {
        std::cerr << "Ticket not found!" << std::endl;
        return false;
    }
    
    int customerId = ticket->getCustomerId();
    int oldFlightNumber = ticket->getFlightNum();
    
    if (!cancelReservation(ticketNumber)) {
        return false;
    }
    
    if (!reserveSeat(customerId, newFlightNumber, newTicketType)) {
        std::cerr << "Failed to reserve new flight. Previous reservation was cancelled." << std::endl;
        return false;
    }
    
    std::cout << "Reservation changed successfully!" << std::endl;
    return true;
}

bool SystemManager::addToWaitingList(int customerId, int flightNumber) {
    Customer* customer = findCustomer(customerId);
    if (!customer) {
        std::cerr << "Customer not found!" << std::endl;
        return false;
    }
    
    Flight* flight = findFlight(flightNumber);
    if (!flight) {
        std::cerr << "Flight not found!" << std::endl;
        return false;
    }
    
    if (_m_waitingLists.find(flightNumber) == _m_waitingLists.end()) {
        Plane* plane = _findPlaneById(flight->getPlaneId());
        int maxSize = plane ? (plane->getTotalSeats() / 100) * 5 : 5;
        _m_waitingLists[flightNumber] = ds::Queue<int>(maxSize);
    }
    
    if (_m_waitingLists[flightNumber].isFull()) {
        std::cerr << "Waiting list is full!" << std::endl;
        return false;
    }
    
    _m_waitingLists[flightNumber].enqueue(customerId);
    _saveToDatabase();
    
    std::cout << "Customer added to waiting list successfully!" << std::endl;
    return true;
}

void SystemManager::processWaitingList(int flightNumber) {
    if (_m_waitingLists.find(flightNumber) == _m_waitingLists.end()) {
        return;
    }
    
    if (_m_waitingLists[flightNumber].isEmpty()) {
        return;
    }
    
    Flight* flight = findFlight(flightNumber);
    if (!flight) return;
    
    Plane* plane = _findPlaneById(flight->getPlaneId());
    if (!plane) return;
    
    auto seatsPerClass = plane->getSeatsPerClass();
    for (const auto& pair : seatsPerClass) {
        int available = flight->getAvailableSeats(pair.first, pair.second);
        
        while (available > 0 && !_m_waitingLists[flightNumber].isEmpty()) {
            int customerId = _m_waitingLists[flightNumber].front();
            _m_waitingLists[flightNumber].dequeue();
            
            std::cout << "Processing waiting list: Customer " << customerId 
                      << " is being reserved on flight " << flightNumber << std::endl;
            
            if (reserveSeat(customerId, flightNumber, pair.first)) {
                available--;
            }
        }
    }
    
    _saveToDatabase();
}

void SystemManager::displayWaitingList(int flightNumber) {
    if (_m_waitingLists.find(flightNumber) == _m_waitingLists.end() || 
        _m_waitingLists[flightNumber].isEmpty()) {
        std::cout << "No customers in waiting list for flight " << flightNumber << std::endl;
        return;
    }
    
    std::cout << "\n========== WAITING LIST FOR FLIGHT " << flightNumber << " ==========" << std::endl;
    ds::Queue<int> tempQueue = _m_waitingLists[flightNumber];
    int position = 1;
    while (!tempQueue.isEmpty()) {
        int custId = tempQueue.front();
        Customer* customer = findCustomer(custId);
        if (customer) {
            std::cout << position << ". Customer ID: " << custId 
                      << " | Name: " << customer->getName() << std::endl;
        }
        tempQueue.dequeue();
        position++;
    }
    std::cout << "========================================================" << std::endl;
}

Ticket* SystemManager::findTicket(int ticketNumber) {
    auto it = std::find_if(_m_tickets.begin(), _m_tickets.end(),
                           [ticketNumber](const Ticket& t) {
                               return t.getTicketNumber() == ticketNumber;
                           });
    return (it != _m_tickets.end()) ? &(*it) : nullptr;
}

void SystemManager::displayTicket(int ticketNumber) {
    Ticket* ticket = findTicket(ticketNumber);
    if (ticket) {
        ticket->print();
    } else {
        std::cout << "Ticket not found!" << std::endl;
    }
}

std::vector<Ticket> SystemManager::getCustomerTickets(int customerId) {
    std::vector<Ticket> customerTickets;
    for (const auto& ticket : _m_tickets) {
        if (ticket.getCustomerId() == customerId) {
            customerTickets.push_back(ticket);
        }
    }
    return customerTickets;
}

Airport* SystemManager::findAirport(const std::string& name) {
    return _findAirportByName(name);
}

Plane* SystemManager::findPlane(int planeId) {
    return _findPlaneById(planeId);
}

void SystemManager::displayAllAirports() {
    std::cout << "\n========== ALL AIRPORTS ==========" << std::endl;
    _m_airports.traverse([](const Airport& a) {
        std::cout << a.getName() << " (" << a.getCountry() << ") - " 
                  << a.getMilesFromIslamabad() << " miles from Islamabad" << std::endl;
    });
    std::cout << "==================================" << std::endl;
}

void SystemManager::displayAllPlanes() {
    std::cout << "\n========== ALL PLANES ==========" << std::endl;
    _m_planes.traverse([](const Plane& p) {
        std::cout << "Plane ID: " << p.getPlaneId() << " | " << p.getDescription() 
                  << " | Total Seats: " << p.getTotalSeats() << std::endl;
    });
    std::cout << "================================" << std::endl;
}

bool SystemManager::isInitialized() const {
    return _m_initialized;
}