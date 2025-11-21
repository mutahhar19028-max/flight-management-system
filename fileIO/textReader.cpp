#include "textReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::string> TextReader::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

std::string TextReader::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

std::vector<Customer> TextReader::readCustomers(const std::string& filename) {
    std::vector<Customer> customers;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return customers;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() >= 6) {
            Customer customer;
            customer.setCustomerId(std::stoi(tokens[0]));
            customer.setName(tokens[1]);
            customer.setPhone(tokens[2]);
            customer.setDob(tokens[3]);
            customer.setNumOfFlights(std::stoi(tokens[4]));
            customer.setMilesTraveled(std::stoi(tokens[5]));
            customers.push_back(customer);
        }
    }
    
    file.close();
    return customers;
}

std::vector<Flight> TextReader::readFlights(const std::string& filename) {
    std::vector<Flight> flights;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return flights;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() >= 7) {
            Flight flight;
            flight.setFlightNumber(std::stoi(tokens[0]));
            flight.setPlaneId(std::stoi(tokens[1]));
            flight.setDay(std::stoi(tokens[2]));
            flight.setMonth(std::stoi(tokens[3]));
            flight.setYear(std::stoi(tokens[4]));
            flight.setDepartureTime(tokens[5]);
            flight.setDestination(tokens[6]);
            
            for (size_t i = 7; i < tokens.size(); ++i) {
                size_t colonPos = tokens[i].find(':');
                if (colonPos != std::string::npos) {
                    std::string className = trim(tokens[i].substr(0, colonPos));
                    int price = std::stoi(tokens[i].substr(colonPos + 1));
                    flight.addTicketPrice(className, price);
                }
            }
            
            flights.push_back(flight);
        }
    }
    
    file.close();
    return flights;
}

std::vector<Airport> TextReader::readAirports(const std::string& filename) {
    std::vector<Airport> airports;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return airports;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() >= 5) {
            Airport airport;
            airport.setName(tokens[0]);
            airport.setCountry(tokens[1]);
            airport.setWeather(tokens[2]);
            airport.setTimeFromIslamabad(std::stoi(tokens[3]));
            airport.setMilesFromIslamabad(std::stoi(tokens[4]));
            airports.push_back(airport);
        }
    }
    
    file.close();
    return airports;
}

std::vector<Plane> TextReader::readPlanes(const std::string& filename) {
    std::vector<Plane> planes;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return planes;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() >= 4) {
            Plane plane;
            plane.setPlaneId(std::stoi(tokens[0]));
            plane.setDescription(tokens[1]);
            
            int businessSeats = std::stoi(tokens[2]);
            int economySeats = std::stoi(tokens[3]);
            
            plane.addSeatClass("Business", businessSeats);
            plane.addSeatClass("Economy", economySeats);
            
            plane.addClassPrice("Business", 1000);
            plane.addClassPrice("Economy", 400);
            
            plane.setNumberOfMilesTraveled(0);
            planes.push_back(plane);
        }
    }
    
    file.close();
    return planes;
}

std::vector<Ticket> TextReader::readTickets(const std::string& filename) {
    std::vector<Ticket> tickets;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return tickets;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() >= 10) {
            Ticket ticket;
            ticket.setCustomerId(std::stoi(tokens[0]));
            ticket.setCustomerName(tokens[1]);
            ticket.setFlightNum(std::stoi(tokens[2]));
            ticket.setTicketNumber(std::stoi(tokens[3]));
            ticket.setTicketType(tokens[4]);
            ticket.setCostOfFlight(std::stoi(tokens[5]));
            ticket.setDiscount(std::stoi(tokens[6]) == 1);
            ticket.setReasonForDiscount(tokens[7]);
            ticket.setDateTime(tokens[8]);
            ticket.setSeatNumber(std::stoi(tokens[9]));
            tickets.push_back(ticket);
        }
    }
    
    file.close();
    return tickets;
}

std::vector<Offer> TextReader::readOffers(const std::string& filename) {
    std::vector<Offer> offers;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return offers;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<std::string> tokens = split(line, ',');
        if (tokens.size() >= 4) {
            Offer offer;
            offer.setOfferId(std::stoi(tokens[0]));
            offer.setFlightNumber(std::stoi(tokens[1]));
            offer.setOfferType(tokens[2]);
            offer.setDiscountAmount(std::stoi(tokens[3]));
            offers.push_back(offer);
        }
    }
    
    file.close();
    return offers;
}