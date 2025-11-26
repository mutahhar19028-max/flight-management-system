#ifndef JSON_MANAGER_H
#define JSON_MANAGER_H

#include "../entities/manager.h"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

class JsonManager {
private:
    SystemManager& system;
    std::string timestamp();

public:
    explicit JsonManager(SystemManager& sys);
    json processCommand(const json& cmd);

private:
    json handleReserveSeat(const json& cmd);
    json handleCancelReservation(const json& cmd);
    json handleChangeReservation(const json& cmd);
    json handleGetCustomerTickets(const json& cmd);
    json handleSearchFlights(const json& cmd);
    json handleGetFlightDetails(const json& cmd);
    json handleGetAllFlights();
    json handleGetAllCustomers();
    json handleGetCustomerProfile(const json& cmd);
    json handleAddToWaitingList(const json& cmd);
    json handleGetWaitingList(const json& cmd);

    json ticketToJson(Ticket* ticket);
    json flightToJson(Flight* flight);
    json customerToJson(Customer* customer);
};

#endif