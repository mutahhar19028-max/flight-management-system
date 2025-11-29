#include "entities/manager.h"
#include "ui/menu.h"
#include "ui/renderer.h"
#include <iostream>
#include <format>
#include <windows.h>

void drawSplashScreen();
bool initializeSystem(SystemManager&);
void printSuccessMessage(const std::string&);
void drawCloseScreen();

int main() {

    drawSplashScreen();
    
    SystemManager system;
    
    if (!initializeSystem(system)) { return 1; }
    
    printSuccessMessage("System initialized successfully!\n");
    
    Menu menu(system);
    menu.run();
    
    drawCloseScreen();
    
    return 0;
}

void drawSplashScreen() {
    SetConsoleOutputCP(CP_UTF8);  // for UTF-8 support on cmd
    
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
}

bool initializeSystem(SystemManager& system) {
    UIRenderer::infoMessage("Initializing Flight Reservation System...\n");
    UIRenderer::showSpinner("Loading database", 1200);
    
    if (!system.initialize("data/system-data.db")) {
        std::cout << "\n";
        UIRenderer::errorMessage("Failed to initialize system.");
        UIRenderer::errorMessage("Please check database connection and try again.");
        UIRenderer::sleep(2000);
        return false;
    }
    return true;
}

void printSuccessMessage(const std::string& message) {
    std::cout << "\n    ";
    UIRenderer::successMessage(message);
    UIRenderer::progressBar(100, 40);
    UIRenderer::sleep(500);
}

void drawCloseScreen() {
    UIRenderer::clearScreen();
    std::cout << "\n\n";
    UIRenderer::printCentered(std::format("{}{}  System shutdown complete.{}", 
                              Theme::DIM, Theme::BOLD, Theme::RESET), 80);
    UIRenderer::printCentered(std::format("{}Have a safe flight! ✈{}", 
                              Theme::PRIMARY, Theme::RESET), 80);
    std::cout << "\n\n";
}
