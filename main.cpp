#include "entities/manager.h"
#include "ui/menu.h"
#include "ui/renderer.h"
#include <iostream>
#include <format>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);  // Tell Windows CMD to interpret output as UTF-8
    // ═══════════════════════════════════════════════════════════
    //  SPLASH SCREEN
    // ═══════════════════════════════════════════════════════════
    UIRenderer::clearScreen();
    UIRenderer::sleep(200);
    
    std::cout << "\n\n\n";
    UIRenderer::printCentered(std::format("{}{}╔══════════════════════════════════════════════════════════════╗{}", 
                              Theme::PRIMARY, Theme::BOLD, Theme::RESET), 80);
    UIRenderer::sleep(50);
    UIRenderer::printCentered(std::format("{}{}║                                                              ║{}", 
                              Theme::PRIMARY, Theme::BOLD, Theme::RESET), 80);
    UIRenderer::sleep(50);
    UIRenderer::printCentered(std::format("{}{}║              ✈  FLIGHT RESERVATION SYSTEM  ✈                 ║{}", 
                              Theme::PRIMARY, Theme::BOLD, Theme::RESET), 81);
    UIRenderer::sleep(50);
    UIRenderer::printCentered(std::format("{}{}║               Islamabad Airport - Pakistan                   ║{}", 
                              Theme::SECONDARY, Theme::BOLD, Theme::RESET), 80);
    UIRenderer::sleep(50);
    UIRenderer::printCentered(std::format("{}{}║                                                              ║{}", 
                              Theme::PRIMARY, Theme::BOLD, Theme::RESET), 80);
    UIRenderer::sleep(50);
    UIRenderer::printCentered(std::format("{}{}╚══════════════════════════════════════════════════════════════╝{}", 
                              Theme::PRIMARY, Theme::BOLD, Theme::RESET), 80);
    
    std::cout << "\n\n";
    UIRenderer::sleep(300);
    
    // ═══════════════════════════════════════════════════════════
    //  SYSTEM INITIALIZATION
    // ═══════════════════════════════════════════════════════════
    SystemManager system;
    
    UIRenderer::infoMessage("Initializing Flight Reservation System...\n");
    UIRenderer::showSpinner("Loading database", 1200);
    
    if (!system.initialize("data/system-data.db")) {
        std::cout << "\n";
        UIRenderer::errorMessage("Failed to initialize system.");
        UIRenderer::errorMessage("Please check database connection and try again.");
        UIRenderer::sleep(2000);
        return 1;
    }
    
    std::cout << "\n    ";
    UIRenderer::successMessage("System initialized successfully!");
    UIRenderer::progressBar(100, 40);
    UIRenderer::sleep(500);
    
    // ═══════════════════════════════════════════════════════════
    //  RUN MAIN MENU
    // ═══════════════════════════════════════════════════════════
    Menu menu(system);
    menu.run();
    
    // ═══════════════════════════════════════════════════════════
    //  CLEANUP & EXIT
    // ═══════════════════════════════════════════════════════════
    UIRenderer::clearScreen();
    std::cout << "\n\n";
    UIRenderer::printCentered(std::format("{}{}  System shutdown complete.{}", 
                              Theme::DIM, Theme::BOLD, Theme::RESET), 80);
    UIRenderer::printCentered(std::format("{}Have a safe flight! ✈{}", 
                              Theme::PRIMARY, Theme::RESET), 80);
    std::cout << "\n\n";
    
    return 0;
}