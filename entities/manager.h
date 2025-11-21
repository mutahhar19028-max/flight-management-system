#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include "../dsLib/bst.hpp"
#include "../dsLib/doublyLinkedList.hpp"
#include "../dsLib/linkedList.hpp"
#include "../dsLib/queue.hpp"
#include "../entities/customer.h"
#include "../entities/flight.h"
#include "../entities/airport.h"
#include "../entities/plane.h"
#include "../entities/ticket.h"
#include "../entities/offer.h"

struct CustomerCompare {
    bool operator()(const Customer& a, const Customer& b) const {
        return a.getCustomerId() < b.getCustomerId();
    }
};

class SystemManager {
private:
    ds::BST<Customer, CustomerCompare> _m_customers;
    ds::DoublyLinkedList<Flight> _m_flights;
    ds::LinkedList<Airport> _m_airports;
    ds::LinkedList<Plane> _m_planes;
    std::vector<Ticket> _m_tickets;
    std::vector<Offer> _m_offers;
    std::map<int, ds::Queue<int>> _m_waitingLists;
    std::string _m_dbPath;
    bool _m_initialized;

private:
    bool _loadFromDatabase();
    bool _loadFromTextFiles();
    bool _saveToDatabase();
    Plane* _findPlaneById(int planeId);
    Airport* _findAirportByName(const std::string& name);
    int _getNextSeatNumber(int flightNumber, const std::string& ticketType);
    int _calculateDiscountedPrice(int basePrice, const Customer& customer, int flightNumber);
    bool _checkOfferEligibility(const Customer& customer, const Offer& offer);
    void _updateCustomerFromTickets(Customer& customer);

public:
    SystemManager();
    ~SystemManager();
    
    bool initialize(const std::string& dbPath);
    
    bool addCustomer(const std::string& name, const std::string& phone, const std::string& dob);
    bool updateCustomer(int customerId, const std::string& name, const std::string& phone, const std::string& dob);
    bool deleteCustomer(int customerId);
    Customer* findCustomer(int customerId);
    void displayCustomer(int customerId);
    void displayAllCustomers();
    
    bool addFlight(int planeId, int day, int month, int year, const std::string& time, const std::string& destination);
    bool updateFlight(int flightNumber, int day, int month, int year, const std::string& time);
    bool deleteFlight(int flightNumber);
    Flight* findFlight(int flightNumber);
    void searchFlights(const std::string& destination);
    void displayFlight(int flightNumber);
    void displayAllFlights();
    
    bool reserveSeat(int customerId, int flightNumber, const std::string& ticketType);
    bool cancelReservation(int ticketNumber);
    bool changeReservation(int ticketNumber, int newFlightNumber, const std::string& newTicketType);
    
    bool addToWaitingList(int customerId, int flightNumber);
    void processWaitingList(int flightNumber);
    void displayWaitingList(int flightNumber);
    
    Ticket* findTicket(int ticketNumber);
    void displayTicket(int ticketNumber);
    std::vector<Ticket> getCustomerTickets(int customerId);
    
    Airport* findAirport(const std::string& name);
    Plane* findPlane(int planeId);
    void displayAllAirports();
    void displayAllPlanes();
    
    bool isInitialized() const;
};

#endif