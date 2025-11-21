#include "menu.h"
#include <iostream>
#include <limits>

Menu::Menu(SystemManager& system) : _m_system(system) {}

void Menu::run() {
    if (!_m_system.isInitialized()) {
        std::cerr << "System not initialized!" << std::endl;
        return;
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "   FLIGHT RESERVATION SYSTEM" << std::endl;
    std::cout << "   Welcome to Islamabad Airport" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    int choice = 0;
    while (choice != 6) {
        _displayMainMenu();
        choice = _getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                _handleFlightReservation();
                break;
            case 2:
                _handleCustomers();
                break;
            case 3:
                _handleFlights();
                break;
            case 4:
                std::cout << "Saving system data..." << std::endl;
                std::cout << "Data saved successfully!" << std::endl;
                _pause();
                break;
            case 5:
                _m_system.displayAllAirports();
                _m_system.displayAllPlanes();
                _pause();
                break;
            case 6:
                std::cout << "Saving and exiting..." << std::endl;
                std::cout << "Thank you for using Flight Reservation System!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
                _pause();
        }
    }
}

void Menu::_displayMainMenu() {
    std::cout << "\n========== MAIN MENU ==========" << std::endl;
    std::cout << "1. Flight Reservation" << std::endl;
    std::cout << "2. Customers" << std::endl;
    std::cout << "3. Flights" << std::endl;
    std::cout << "4. Save Data" << std::endl;
    std::cout << "5. View System Info" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "===============================" << std::endl;
}

void Menu::_handleFlightReservation() {
    int choice = 0;
    while (choice != 5) {
        std::cout << "\n===== FLIGHT RESERVATION =====" << std::endl;
        std::cout << "1. Search for Flights" << std::endl;
        std::cout << "2. Reserve Flight" << std::endl;
        std::cout << "3. Cancel Reservation" << std::endl;
        std::cout << "4. Change Reservation" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "==============================" << std::endl;
        
        choice = _getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                _searchFlights();
                break;
            case 2:
                _reserveFlight();
                break;
            case 3:
                _cancelReservation();
                break;
            case 4:
                _changeReservation();
                break;
            case 5:
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                _pause();
        }
    }
}

void Menu::_handleCustomers() {
    int choice = 0;
    while (choice != 5) {
        std::cout << "\n========== CUSTOMERS ==========" << std::endl;
        std::cout << "1. Add Customer" << std::endl;
        std::cout << "2. Update Customer" << std::endl;
        std::cout << "3. Delete Customer" << std::endl;
        std::cout << "4. View Customer Profile" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "===============================" << std::endl;
        
        choice = _getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                _addCustomer();
                break;
            case 2:
                _updateCustomer();
                break;
            case 3:
                _deleteCustomer();
                break;
            case 4:
                _viewCustomerProfile();
                break;
            case 5:
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                _pause();
        }
    }
}

void Menu::_handleFlights() {
    int choice = 0;
    while (choice != 5) {
        std::cout << "\n========== FLIGHTS ==========" << std::endl;
        std::cout << "1. Add Flight" << std::endl;
        std::cout << "2. Update Flight" << std::endl;
        std::cout << "3. Delete Flight" << std::endl;
        std::cout << "4. View All Flights" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "=============================" << std::endl;
        
        choice = _getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                _addFlight();
                break;
            case 2:
                _updateFlight();
                break;
            case 3:
                _deleteFlight();
                break;
            case 4:
                _viewFlights();
                break;
            case 5:
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                _pause();
        }
    }
}

void Menu::_searchFlights() {
    std::string destination = _getStringInput("Enter destination: ");
    _m_system.searchFlights(destination);
    _pause();
}

void Menu::_reserveFlight() {
    int customerId = _getIntInput("Enter Customer ID: ");
    int flightNumber = _getIntInput("Enter Flight Number: ");
    std::string ticketType = _getStringInput("Enter Ticket Type (Economy/Business): ");
    
    _m_system.reserveSeat(customerId, flightNumber, ticketType);
    _pause();
}

void Menu::_cancelReservation() {
    int ticketNumber = _getIntInput("Enter Ticket Number: ");
    _m_system.cancelReservation(ticketNumber);
    _pause();
}

void Menu::_changeReservation() {
    int ticketNumber = _getIntInput("Enter Ticket Number: ");
    int newFlightNumber = _getIntInput("Enter New Flight Number: ");
    std::string newTicketType = _getStringInput("Enter New Ticket Type (Economy/Business): ");
    
    _m_system.changeReservation(ticketNumber, newFlightNumber, newTicketType);
    _pause();
}

void Menu::_addCustomer() {
    std::string name = _getStringInput("Enter Name: ");
    std::string phone = _getStringInput("Enter Phone: ");
    std::string dob = _getStringInput("Enter Date of Birth (DD/MM/YYYY): ");
    
    _m_system.addCustomer(name, phone, dob);
    _pause();
}

void Menu::_updateCustomer() {
    int customerId = _getIntInput("Enter Customer ID: ");
    
    Customer* customer = _m_system.findCustomer(customerId);
    if (!customer) {
        std::cout << "Customer not found!" << std::endl;
        _pause();
        return;
    }
    
    customer->display();
    
    std::cout << "\nEnter new values (press Enter to keep current):" << std::endl;
    std::string name = _getStringInput("Name: ");
    std::string phone = _getStringInput("Phone: ");
    std::string dob = _getStringInput("Date of Birth: ");
    
    _m_system.updateCustomer(customerId, name, phone, dob);
    _pause();
}

void Menu::_deleteCustomer() {
    int customerId = _getIntInput("Enter Customer ID: ");
    
    char confirm;
    std::cout << "Are you sure you want to delete this customer? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        _m_system.deleteCustomer(customerId);
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
    
    _pause();
}

void Menu::_viewCustomerProfile() {
    int customerId = _getIntInput("Enter Customer ID: ");
    _m_system.displayCustomer(customerId);
    _pause();
}

void Menu::_addFlight() {
    _m_system.displayAllPlanes();
    
    int planeId = _getIntInput("Enter Plane ID: ");
    int day = _getIntInput("Enter Day: ");
    int month = _getIntInput("Enter Month: ");
    int year = _getIntInput("Enter Year: ");
    std::string time = _getStringInput("Enter Departure Time (HH:MM): ");
    std::string destination = _getStringInput("Enter Destination: ");
    
    _m_system.addFlight(planeId, day, month, year, time, destination);
    _pause();
}

void Menu::_updateFlight() {
    int flightNumber = _getIntInput("Enter Flight Number: ");
    
    Flight* flight = _m_system.findFlight(flightNumber);
    if (!flight) {
        std::cout << "Flight not found!" << std::endl;
        _pause();
        return;
    }
    
    flight->display();
    
    std::cout << "\nEnter new values:" << std::endl;
    int day = _getIntInput("Day: ");
    int month = _getIntInput("Month: ");
    int year = _getIntInput("Year: ");
    std::string time = _getStringInput("Departure Time (HH:MM): ");
    
    _m_system.updateFlight(flightNumber, day, month, year, time);
    _pause();
}

void Menu::_deleteFlight() {
    int flightNumber = _getIntInput("Enter Flight Number: ");
    
    char confirm;
    std::cout << "Are you sure you want to delete this flight? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        _m_system.deleteFlight(flightNumber);
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
    
    _pause();
}

void Menu::_viewFlights() {
    _m_system.displayAllFlights();
    _pause();
}

int Menu::_getIntInput(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. " << prompt;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

std::string Menu::_getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

void Menu::_pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}