#ifndef MENU_H
#define MENU_H

#include "../entities/manager.h"

class Menu {
private:
    SystemManager& _m_system;
    
    void _displayMainMenu();
    void _handleFlightReservation();
    void _handleCustomers();
    void _handleFlights();
    void _handleSetupAndSave();
    
    void _searchFlights();
    void _reserveFlight();
    void _cancelReservation();
    void _changeReservation();
    
    void _addCustomer();
    void _updateCustomer();
    void _deleteCustomer();
    void _viewCustomerProfile();
    
    void _addFlight();
    void _updateFlight();
    void _deleteFlight();
    void _viewFlights();
    
    int _getIntInput(const std::string& prompt);
    std::string _getStringInput(const std::string& prompt);
    void _pause();

public:
    Menu(SystemManager& system);
    void run();
};

#endif