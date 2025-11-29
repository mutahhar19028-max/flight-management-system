#ifndef MENU_H
#define MENU_H

#include "../entities/manager.h"
#include <string>

class Menu {
private:
    SystemManager& _m_system;
    
    // Menu handlers
    void _handleFlightReservation();
    void _handleCustomers();
    void _handleFlights();
    
    // Flight reservation operations
    void _searchFlights();
    void _reserveFlight();
    void _cancelReservation();
    void _changeReservation();
    
    // Customer operations
    void _addCustomer();
    void _updateCustomer();
    void _deleteCustomer();
    void _viewCustomerProfile();
    
    // Flight operations
    void _addFlight();
    void _updateFlight();
    void _deleteFlight();
    void _viewFlights();
    
    // Input helpers
    int _getIntInput(const std::string& prompt);
    std::string _getStringInput(const std::string& prompt);
    void _pause();

public:
    Menu(SystemManager& system);
    void run();
};

#endif // MENU_H