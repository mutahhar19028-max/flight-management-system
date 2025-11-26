#include "entities/manager.h"
#include "json/json-manager.h"
#include "json/json.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    SystemManager system;
    
    if (!system.initialize("data/system-data.db")) {
        json errorResponse;
        errorResponse["success"] = false;
        errorResponse["message"] = "Failed to initialize system";
        std::cout << errorResponse.dump() << std::endl;
        return 1;
    }
    
    JsonManager jsonManager(system);
    
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        try {
            json command = json::parse(line);
            
            if (command.value("action", "") == "exit") {
                json exitResponse;
                exitResponse["success"] = true;
                exitResponse["message"] = "System shutting down";
                if (command.contains("actionId")) {
                    exitResponse["actionId"] = command["actionId"];
                }
                std::cout << exitResponse.dump() << std::endl;
                break;
            }
            
            json response = jsonManager.processCommand(command);
            std::cout << response.dump() << std::endl;
            std::cout.flush();
            
        } catch (const json::parse_error& e) {
            json errorResponse;
            errorResponse["success"] = false;
            errorResponse["message"] = std::string("JSON parse error: ") + e.what();
            std::cout << errorResponse.dump() << std::endl;
        } catch (const std::exception& e) {
            json errorResponse;
            errorResponse["success"] = false;
            errorResponse["message"] = std::string("Exception: ") + e.what();
            std::cout << errorResponse.dump() << std::endl;
        }
    }
    
    return 0;
}