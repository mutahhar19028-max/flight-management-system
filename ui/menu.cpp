#include "menu.h"
#include "renderer.h"
#include "interactiveMenu.h"
#include <iostream>
#include <limits>
#include <format>

Menu::Menu(SystemManager& system) : _m_system(system) {}

void Menu::run() {
    if (!_m_system.isInitialized()) {
        UIRenderer::clearScreen();
        UIRenderer::errorMessage("System not initialized!");
        return;
    }
    
    // Welcome animation
    UIRenderer::clearScreen();
    UIRenderer::sleep(100);
    
    std::cout << "\n\n";
    
    UIRenderer::showSpinner("Initializing system", 800);
    UIRenderer::successMessage("System ready!");
    UIRenderer::sleep(500);
    
    int choice = 0;
    while (choice != 6) {
        UIRenderer::clearScreen();
        
        // Create interactive main menu
        InteractiveMenu mainMenu("MAIN MENU", {
            {"  ", " Flight Reservation ", 1},
            {"  ", " Customers ", 2},
            {"  ", " Flights ", 3},
            {"  ", " Save Data ", 4},
            {"  ", " View System Info ", 5},
            {"  ", " Exit ", 6}
        });
        
        choice = mainMenu.show();
        
        if (choice == -1) continue;  // ESC pressed
        
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
                UIRenderer::clearScreen();
                UIRenderer::showSpinner("Saving system data", 1000);
                UIRenderer::successMessage("Data saved successfully!");
                UIRenderer::pause();
                break;
            case 5:
                UIRenderer::clearScreen();
                UIRenderer::infoMessage("Loading system information...");
                UIRenderer::sleep(300);
                _m_system.displayAllAirports();
                std::cout << "\n";
                _m_system.displayAllPlanes();
                UIRenderer::pause();
                break;
            case 6:
                UIRenderer::clearScreen();
                UIRenderer::showSpinner("Saving and closing", 800);
                std::cout << "\n";
                UIRenderer::typeText(std::format("{}Thank you for using Flight Reservation System!{}", 
                                   Theme::PRIMARY, Theme::RESET), 15);
                UIRenderer::sleep(300);
                break;
        }
    }
}

void Menu::_handleFlightReservation() {
    int choice = 0;
    while (choice != 5) {
        UIRenderer::clearScreen();
        
        InteractiveMenu menu("FLIGHT RESERVATION", {
            {"", "Search for Flights", 1},
            {"", "Reserve Flight", 2},
            {"", "Cancel Reservation", 3},
            {"", "Change Reservation", 4},
            {"", "Back to Main Menu", 5}
        });
        
        choice = menu.show();
        
        if (choice == -1) choice = 5;  // ESC = back
        
        switch (choice) {
            case 1: _searchFlights(); break;
            case 2: _reserveFlight(); break;
            case 3: _cancelReservation(); break;
            case 4: _changeReservation(); break;
            case 5: break;
        }
    }
}

void Menu::_handleCustomers() {
    int choice = 0;
    while (choice != 5) {
        UIRenderer::clearScreen();
        
        InteractiveMenu menu("CUSTOMER MANAGEMENT", {
            {"", "Add Customer", 1},
            {"", "Update Customer", 2},
            {"", "Delete Customer", 3},
            {"", "View Customer Profile", 4},
            {"", "Back to Main Menu", 5}
        });
        
        choice = menu.show();
        
        if (choice == -1) choice = 5;  // ESC = back
        
        switch (choice) {
            case 1: _addCustomer(); break;
            case 2: _updateCustomer(); break;
            case 3: _deleteCustomer(); break;
            case 4: _viewCustomerProfile(); break;
            case 5: break;
        }
    }
}

void Menu::_handleFlights() {
    int choice = 0;
    while (choice != 5) {
        UIRenderer::clearScreen();
        
        InteractiveMenu menu("FLIGHT MANAGEMENT", {
            {"", "Add Flight", 1},
            {"", "Update Flight", 2},
            {"", "Delete Flight", 3},
            {"", "View All Flights", 4},
            {"", "Back to Main Menu", 5}
        });
        
        choice = menu.show();
        
        if (choice == -1) choice = 5;  // ESC = back
        
        switch (choice) {
            case 1: _addFlight(); break;
            case 2: _updateFlight(); break;
            case 3: _deleteFlight(); break;
            case 4: _viewFlights(); break;
            case 5: break;
        }
    }
}

void Menu::_searchFlights() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ SEARCH FLIGHTS ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    std::string destination = _getStringInput(std::format("{} Destination:{} ", 
                                              Theme::SECONDARY, Theme::RESET));
    
    UIRenderer::showSpinner("Searching flights", 600);
    std::cout << "\n";
    _m_system.searchFlights(destination);
    UIRenderer::pause();
}

void Menu::_reserveFlight() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ RESERVE FLIGHT ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    int customerId = _getIntInput(std::format("{} Customer ID:{} ", Theme::SECONDARY, Theme::RESET));
    int flightNumber = _getIntInput(std::format("{} Flight Number:{} ", Theme::SECONDARY, Theme::RESET));
    
    std::cout << "\n";
    InteractiveMenu ticketMenu("SELECT TICKET TYPE", {
        {"", "Economy Class", 1},
        {"", "Business Class", 2}
    });
    
    int ticketChoice = ticketMenu.show();
    if (ticketChoice == -1) {
        UIRenderer::infoMessage("Reservation cancelled.");
        _pause();
        return;
    }
    
    std::string ticketType = (ticketChoice == 1) ? "Economy" : "Business";
                                             
    
    UIRenderer::showSpinner("Processing reservation", 800);
    std::cout << "\n";
    _m_system.reserveSeat(customerId, flightNumber, ticketType);
    UIRenderer::pause();
}

void Menu::_cancelReservation() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ CANCEL RESERVATION ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    int ticketNumber = _getIntInput(std::format("{} Ticket Number:{} ", Theme::SECONDARY, Theme::RESET));
    
    UIRenderer::warningMessage("Processing cancellation...");
    UIRenderer::sleep(400);
    _m_system.cancelReservation(ticketNumber);
    UIRenderer::pause();
}

void Menu::_changeReservation() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ CHANGE RESERVATION ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    int ticketNumber = _getIntInput(std::format("{} Current Ticket Number:{} ", 
                                    Theme::SECONDARY, Theme::RESET));
    int newFlightNumber = _getIntInput(std::format("{} New Flight Number:{} ", 
                                       Theme::SECONDARY, Theme::RESET));
    std::string newTicketType = _getStringInput(std::format("{} New Ticket Type:{} ", 
                                                Theme::SECONDARY, Theme::RESET));
    
    UIRenderer::showSpinner("Processing change", 800);
    std::cout << "\n";
    _m_system.changeReservation(ticketNumber, newFlightNumber, newTicketType);
    UIRenderer::pause();
}

void Menu::_addCustomer() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ ADD CUSTOMER ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    std::string name = _getStringInput(std::format("{} Name:{} ", Theme::SECONDARY, Theme::RESET));
    std::string phone = _getStringInput(std::format("{} Phone:{} ", Theme::SECONDARY, Theme::RESET));
    std::string dob = _getStringInput(std::format("{} Date of Birth (DD/MM/YYYY):{} ", 
                                      Theme::SECONDARY, Theme::RESET));
    
    UIRenderer::showSpinner("Adding customer", 600);
    std::cout << "\n";
    _m_system.addCustomer(name, phone, dob);
    UIRenderer::pause();
}

void Menu::_updateCustomer() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ UPDATE CUSTOMER ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    int customerId = _getIntInput(std::format("{} Customer ID:{} ", Theme::SECONDARY, Theme::RESET));
    
    Customer* customer = _m_system.findCustomer(customerId);
    if (!customer) {
        UIRenderer::errorMessage("Customer not found!");
        UIRenderer::pause();
        return;
    }
    
    std::cout << "\n" << std::format("{}Current Information:{}\n", Theme::INFO, Theme::RESET);
    UIRenderer::printSeparator(50);
    customer->display();
    
    std::cout << "\n" << std::format("{}{}Enter new values (press Enter to keep current):{}\n\n", 
              Theme::WARNING, Theme::BOLD, Theme::RESET);
    
    std::string name = _getStringInput(std::format("{} Name:{} ", Theme::SECONDARY, Theme::RESET));
    std::string phone = _getStringInput(std::format("{} Phone:{} ", Theme::SECONDARY, Theme::RESET));
    std::string dob = _getStringInput(std::format("{} Date of Birth:{} ", Theme::SECONDARY, Theme::RESET));
    
    UIRenderer::showSpinner("Updating customer", 600);
    std::cout << "\n";
    _m_system.updateCustomer(customerId, name, phone, dob);
    UIRenderer::pause();
}

void Menu::_deleteCustomer() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ DELETE CUSTOMER ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    int customerId = _getIntInput(std::format("{} Customer ID:{} ", Theme::SECONDARY, Theme::RESET));
    
    char confirm;
    std::cout << std::format("{} Are you sure? (y/n):{} ", Theme::WARNING, Theme::RESET);
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        UIRenderer::showSpinner("Deleting customer", 600);
        std::cout << "\n";
        _m_system.deleteCustomer(customerId);
    } else {
        UIRenderer::infoMessage("Deletion cancelled.");
    }
    
    UIRenderer::pause();
}

void Menu::_viewCustomerProfile() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ CUSTOMER PROFILE ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    int customerId = _getIntInput(std::format("{} Customer ID:{} ", Theme::SECONDARY, Theme::RESET));
    
    UIRenderer::showSpinner("Loading profile", 400);
    std::cout << "\n";
    UIRenderer::printSeparator(70);
    _m_system.displayCustomer(customerId);
    UIRenderer::printSeparator(70);
    UIRenderer::pause();
}

void Menu::_addFlight() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ ADD FLIGHT ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    UIRenderer::infoMessage("Available planes:");
    UIRenderer::printSeparator(70);
    _m_system.displayAllPlanes();
    UIRenderer::printSeparator(70);
    
    std::cout << "\n";
    int planeId = _getIntInput(std::format("{} Plane ID:{} ", Theme::SECONDARY, Theme::RESET));
    int day = _getIntInput(std::format("{} Day:{} ", Theme::SECONDARY, Theme::RESET));
    int month = _getIntInput(std::format("{} Month:{} ", Theme::SECONDARY, Theme::RESET));
    int year = _getIntInput(std::format("{} Year:{} ", Theme::SECONDARY, Theme::RESET));
    std::string time = _getStringInput(std::format("{} Departure Time (HH:MM):{} ", 
                                       Theme::SECONDARY, Theme::RESET));
    std::string destination = _getStringInput(std::format("{} Destination:{} ", 
                                              Theme::SECONDARY, Theme::RESET));
    
    UIRenderer::showSpinner("Adding flight", 800);
    std::cout << "\n";
    _m_system.addFlight(planeId, day, month, year, time, destination);
    UIRenderer::pause();
}

void Menu::_updateFlight() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ UPDATE FLIGHT ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    int flightNumber = _getIntInput(std::format("{} Flight Number:{} ", Theme::SECONDARY, Theme::RESET));
    
    Flight* flight = _m_system.findFlight(flightNumber);
    if (!flight) {
        UIRenderer::errorMessage("Flight not found!");
        UIRenderer::pause();
        return;
    }
    
    std::cout << "\n" << std::format("{}Current Information:{}\n", Theme::INFO, Theme::RESET);
    UIRenderer::printSeparator(70);
    flight->display();
    UIRenderer::printSeparator(70);
    
    std::cout << "\n" << std::format("{}{}Enter new values:{}\n\n", 
              Theme::WARNING, Theme::BOLD, Theme::RESET);
    
    int day = _getIntInput(std::format("{} Day:{} ", Theme::SECONDARY, Theme::RESET));
    int month = _getIntInput(std::format("{} Month:{} ", Theme::SECONDARY, Theme::RESET));
    int year = _getIntInput(std::format("{} Year:{} ", Theme::SECONDARY, Theme::RESET));
    std::string time = _getStringInput(std::format("{} Departure Time (HH:MM):{} ", 
                                       Theme::SECONDARY, Theme::RESET));
    
    UIRenderer::showSpinner("Updating flight", 600);
    std::cout << "\n";
    _m_system.updateFlight(flightNumber, day, month, year, time);
    UIRenderer::pause();
}

void Menu::_deleteFlight() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ DELETE FLIGHT ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    int flightNumber = _getIntInput(std::format("{} Flight Number:{} ", Theme::SECONDARY, Theme::RESET));
    
    char confirm;
    std::cout << std::format("{} Are you sure? (y/n):{} ", Theme::WARNING, Theme::RESET);
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        UIRenderer::showSpinner("Deleting flight", 600);
        std::cout << "\n";
        _m_system.deleteFlight(flightNumber);
    } else {
        UIRenderer::infoMessage("Deletion cancelled.");
    }
    
    UIRenderer::pause();
}

void Menu::_viewFlights() {
    UIRenderer::clearScreen();
    std::cout << "\n" << std::format("{}{}═══ ALL FLIGHTS ═══{}\n\n", 
              Theme::PRIMARY, Theme::BOLD, Theme::RESET);
    
    UIRenderer::showSpinner("Loading flights", 500);
    std::cout << "\n";
    UIRenderer::printSeparator(70);
    _m_system.displayAllFlights();
    UIRenderer::printSeparator(70);
    UIRenderer::pause();
}

int Menu::_getIntInput(const std::string& prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        UIRenderer::errorMessage("Invalid input.");
        std::cout << prompt;
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
    UIRenderer::pause();
}