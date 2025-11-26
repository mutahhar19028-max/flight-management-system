#include "entities/manager.h"
#include "ui/menu.h"
#include <iostream>

int main() {
    SystemManager system;
    
    std::cout << "Initializing Flight Reservation System..." << std::endl;
    
    if (!system.initialize("data/system-data.db")) {
        std::cerr << "Failed to initialize system. Exiting..." << std::endl;
        return 1;
    }
    
    Menu menu(system);
    menu.run();
    
    return 0;
}