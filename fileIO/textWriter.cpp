#include "textWriter.h"
#include <fstream>
#include <iostream>

bool TextWriter::writeCustomers(const std::string& filename, const std::vector<Customer>& customers) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return false;
    }
    
    for (const auto& customer : customers) {
        file << customer.getCustomerId() << ","
             << customer.getName() << ","
             << customer.getPhone() << ","
             << customer.getDob() << ","
             << customer.getNumOfFlights() << ","
             << customer.getMilesTraveled() << "\n";
    }
    
    file.close();
    return true;
}

bool TextWriter::writeFlights(const std::string& filename, const std::vector<Flight>& flights) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return false;
    }
    
    for (const auto& flight : flights) {
        file << flight.getFlightNumber() << ","
             << flight.getPlaneId() << ","
             << flight.getDay() << ","
             << flight.getMonth() << ","
             << flight.getYear() << ","
             << flight.getDepartureTime() << ","
             << flight.getDestination();
        
        auto pricing = flight.getTicketPricing();
        for (const auto& pair : pricing) {
            file << "," << pair.first << ":" << pair.second;
        }
        
        file << "\n";
    }
    
    file.close();
    return true;
}

bool TextWriter::writeAirports(const std::string& filename, const std::vector<Airport>& airports) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return false;
    }
    
    for (const auto& airport : airports) {
        file << airport.getName() << ","
             << airport.getCountry() << ","
             << airport.getWeather() << ","
             << airport.getTimeFromIslamabad() << ","
             << airport.getMilesFromIslamabad() << "\n";
    }
    
    file.close();
    return true;
}

bool TextWriter::writePlanes(const std::string& filename, const std::vector<Plane>& planes) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return false;
    }
    
    for (const auto& plane : planes) {
        file << plane.getPlaneId() << ","
             << plane.getDescription() << ","
             << plane.getSeatsForClass("Business") << ","
             << plane.getSeatsForClass("Economy") << "\n";
    }
    
    file.close();
    return true;
}

bool TextWriter::writeTickets(const std::string& filename, const std::vector<Ticket>& tickets) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return false;
    }
    
    for (const auto& ticket : tickets) {
        file << ticket.getCustomerId() << ","
             << ticket.getCustomerName() << ","
             << ticket.getFlightNum() << ","
             << ticket.getTicketNumber() << ","
             << ticket.getTicketType() << ","
             << ticket.getCostOfFlight() << ","
             << (ticket.getDiscount() ? 1 : 0) << ","
             << ticket.getReasonForDiscount() << ","
             << ticket.getDateTime() << ","
             << ticket.getSeatNumber() << "\n";
    }
    
    file.close();
    return true;
}

bool TextWriter::writeOffers(const std::string& filename, const std::vector<Offer>& offers) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << " for writing" << std::endl;
        return false;
    }
    
    for (const auto& offer : offers) {
        file << offer.getOfferId() << ","
             << offer.getFlightNumber() << ","
             << offer.getOfferType() << ","
             << offer.getDiscountAmount() << "\n";
    }
    
    file.close();
    return true;
}