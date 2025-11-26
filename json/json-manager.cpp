#include "json-manager.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

JsonManager::JsonManager(SystemManager& sys) : system(sys) {}

std::string JsonManager::timestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "[%Y-%m-%d %H:%M:%S]");
    return oss.str();
}

json JsonManager::processCommand(const json& cmd) {
    json response;
    std::string action = cmd.value("action", "");

    if (action == "reserveSeat") {
        response = handleReserveSeat(cmd);
    } else if (action == "cancelReservation") {
        response = handleCancelReservation(cmd);
    } else if (action == "changeReservation") {
        response = handleChangeReservation(cmd);
    } else if (action == "getCustomerTickets") {
        response = handleGetCustomerTickets(cmd);
    } else if (action == "searchFlights") {
        response = handleSearchFlights(cmd);
    } else if (action == "getFlightDetails") {
        response = handleGetFlightDetails(cmd);
    } else if (action == "getAllFlights") {
        response = handleGetAllFlights();
    } else if (action == "getAllCustomers") {
        response = handleGetAllCustomers();
    } else if (action == "getCustomerProfile") {
        response = handleGetCustomerProfile(cmd);
    } else if (action == "addToWaitingList") {
        response = handleAddToWaitingList(cmd);
    } else if (action == "getWaitingList") {
        response = handleGetWaitingList(cmd);
    } else if (action == "exit") {
        response["success"] = true;
        response["message"] = "Shutting down";
    } else {
        response["success"] = false;
        response["message"] = "Unknown action: " + action;
    }

    if (cmd.contains("actionId")) {
        response["actionId"] = cmd["actionId"];
    }

    return response;
}

json JsonManager::handleReserveSeat(const json& cmd) {
    json response;

    try {
        int customerId = cmd.value("customerId", 0);
        int flightNumber = cmd.value("flightNumber", 0);
        std::string ticketType = cmd.value("ticketType", "Economy");

        if (customerId == 0 || flightNumber == 0) {
            response["success"] = false;
            response["message"] = "Invalid customer ID or flight number";
            return response;
        }

        Customer* customer = system.findCustomer(customerId);
        if (!customer) {
            bool added = system.addCustomer("Customer" + std::to_string(customerId), 
                                           "000-0000", "01/01/2000");
            if (!added) {
                response["success"] = false;
                response["message"] = "Failed to create customer";
                return response;
            }
            customer = system.findCustomer(customerId);
        }

        bool success = system.reserveSeat(customerId, flightNumber, ticketType);

        if (success) {
            response["success"] = true;
            response["message"] = "Seat reserved successfully";
            
            auto tickets = system.getCustomerTickets(customerId);
            if (!tickets.empty()) {
                response["ticketNumber"] = tickets.back().getTicketNumber();
            }
        } else {
            response["success"] = false;
            response["message"] = "Reservation failed - flight may be full or not found";
        }

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleCancelReservation(const json& cmd) {
    json response;

    try {
        int ticketNumber = cmd.value("ticketNumber", 0);

        if (ticketNumber == 0) {
            response["success"] = false;
            response["message"] = "Invalid ticket number";
            return response;
        }

        bool success = system.cancelReservation(ticketNumber);

        if (success) {
            response["success"] = true;
            response["message"] = "Reservation cancelled successfully";
        } else {
            response["success"] = false;
            response["message"] = "Cancellation failed - ticket not found";
        }

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleChangeReservation(const json& cmd) {
    json response;

    try {
        int ticketNumber = cmd.value("ticketNumber", 0);
        int newFlightNumber = cmd.value("newFlightNumber", 0);
        std::string newTicketType = cmd.value("newTicketType", "Economy");

        if (ticketNumber == 0 || newFlightNumber == 0) {
            response["success"] = false;
            response["message"] = "Invalid ticket or flight number";
            return response;
        }

        bool success = system.changeReservation(ticketNumber, newFlightNumber, newTicketType);

        if (success) {
            response["success"] = true;
            response["message"] = "Reservation changed successfully";
        } else {
            response["success"] = false;
            response["message"] = "Change failed - ticket or flight not found";
        }

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleGetCustomerTickets(const json& cmd) {
    json response;

    try {
        int customerId = cmd.value("customerId", 0);

        if (customerId == 0) {
            response["success"] = false;
            response["message"] = "Invalid customer ID";
            return response;
        }

        auto tickets = system.getCustomerTickets(customerId);

        json ticketsArray = json::array();
        for (const auto& ticket : tickets) {
            json ticketObj;
            ticketObj["ticketNumber"] = ticket.getTicketNumber();
            ticketObj["flightNumber"] = ticket.getFlightNum();
            ticketObj["ticketType"] = ticket.getTicketType();
            ticketObj["cost"] = ticket.getCostOfFlight();
            ticketObj["seatNumber"] = ticket.getSeatNumber();
            ticketObj["dateTime"] = ticket.getDateTime();
            ticketObj["discount"] = ticket.getDiscount();
            ticketObj["discountReason"] = ticket.getReasonForDiscount();
            ticketsArray.push_back(ticketObj);
        }

        response["success"] = true;
        response["tickets"] = ticketsArray;
        response["count"] = ticketsArray.size();

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleSearchFlights(const json& cmd) {
    json response;

    try {
        std::string destination = cmd.value("destination", "");

        if (destination.empty()) {
            response["success"] = false;
            response["message"] = "Destination required";
            return response;
        }

        response["success"] = true;
        response["message"] = "Use getAllFlights and filter by destination in frontend";

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleGetFlightDetails(const json& cmd) {
    json response;

    try {
        int flightNumber = cmd.value("flightNumber", 0);

        if (flightNumber == 0) {
            response["success"] = false;
            response["message"] = "Invalid flight number";
            return response;
        }

        Flight* flight = system.findFlight(flightNumber);

        if (flight) {
            response["success"] = true;
            response["flight"] = flightToJson(flight);
        } else {
            response["success"] = false;
            response["message"] = "Flight not found";
        }

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleGetAllFlights() {
    json response;

    try {
        response["success"] = true;
        response["message"] = "Flights loaded from database directly in frontend";

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleGetAllCustomers() {
    json response;

    try {
        response["success"] = true;
        response["message"] = "Customers loaded from database directly in frontend";

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleGetCustomerProfile(const json& cmd) {
    json response;

    try {
        int customerId = cmd.value("customerId", 0);

        if (customerId == 0) {
            response["success"] = false;
            response["message"] = "Invalid customer ID";
            return response;
        }

        Customer* customer = system.findCustomer(customerId);

        if (customer) {
            response["success"] = true;
            response["customer"] = customerToJson(customer);
        } else {
            response["success"] = false;
            response["message"] = "Customer not found";
        }

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleAddToWaitingList(const json& cmd) {
    json response;

    try {
        int customerId = cmd.value("customerId", 0);
        int flightNumber = cmd.value("flightNumber", 0);

        if (customerId == 0 || flightNumber == 0) {
            response["success"] = false;
            response["message"] = "Invalid customer ID or flight number";
            return response;
        }

        bool success = system.addToWaitingList(customerId, flightNumber);

        if (success) {
            response["success"] = true;
            response["message"] = "Added to waiting list successfully";
        } else {
            response["success"] = false;
            response["message"] = "Failed to add to waiting list";
        }

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::handleGetWaitingList(const json& cmd) {
    json response;

    try {
        int flightNumber = cmd.value("flightNumber", 0);

        if (flightNumber == 0) {
            response["success"] = false;
            response["message"] = "Invalid flight number";
            return response;
        }

        response["success"] = true;
        response["message"] = "Waiting list available";

    } catch (const std::exception& e) {
        response["success"] = false;
        response["message"] = std::string("Exception: ") + e.what();
    }

    return response;
}

json JsonManager::ticketToJson(Ticket* ticket) {
    json obj;
    obj["ticketNumber"] = ticket->getTicketNumber();
    obj["customerId"] = ticket->getCustomerId();
    obj["customerName"] = ticket->getCustomerName();
    obj["flightNumber"] = ticket->getFlightNum();
    obj["ticketType"] = ticket->getTicketType();
    obj["cost"] = ticket->getCostOfFlight();
    obj["discount"] = ticket->getDiscount();
    obj["discountReason"] = ticket->getReasonForDiscount();
    obj["dateTime"] = ticket->getDateTime();
    obj["seatNumber"] = ticket->getSeatNumber();
    return obj;
}

json JsonManager::flightToJson(Flight* flight) {
    json obj;
    obj["flightNumber"] = flight->getFlightNumber();
    obj["planeId"] = flight->getPlaneId();
    obj["day"] = flight->getDay();
    obj["month"] = flight->getMonth();
    obj["year"] = flight->getYear();
    obj["departureTime"] = flight->getDepartureTime();
    obj["destination"] = flight->getDestination();
    
    auto pricing = flight->getTicketPricing();
    json pricingJson = json::object();
    for (const auto& pair : pricing) {
        pricingJson[pair.first] = pair.second;
    }
    obj["pricing"] = pricingJson;
    
    return obj;
}

json JsonManager::customerToJson(Customer* customer) {
    json obj;
    obj["customerId"] = customer->getCustomerId();
    obj["name"] = customer->getName();
    obj["phone"] = customer->getPhone();
    obj["dob"] = customer->getDob();
    obj["numOfFlights"] = customer->getNumOfFlights();
    obj["milesTraveled"] = customer->getMilesTraveled();
    return obj;
}