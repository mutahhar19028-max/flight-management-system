#include "idGenerator.h"

int IdGenerator::_m_lastCustomerId = 1000;
int IdGenerator::_m_lastTicketId = 5000;
int IdGenerator::_m_lastFlightId = 100;

int IdGenerator::generateCustomerId() {
    return ++_m_lastCustomerId;
}

int IdGenerator::generateTicketId() {
    return ++_m_lastTicketId;
}

int IdGenerator::generateFlightId() {
    return ++_m_lastFlightId;
}

void IdGenerator::setLastCustomerId(int id) {
    _m_lastCustomerId = id;
}

void IdGenerator::setLastTicketId(int id) {
    _m_lastTicketId = id;
}

void IdGenerator::setLastFlightId(int id) {
    _m_lastFlightId = id;
}

int IdGenerator::getLastCustomerId() {
    return _m_lastCustomerId;
}

int IdGenerator::getLastTicketId() {
    return _m_lastTicketId;
}

int IdGenerator::getLastFlightId() {
    return _m_lastFlightId;
}

void IdGenerator::reset() {
    _m_lastCustomerId = 1000;
    _m_lastTicketId = 5000;
    _m_lastFlightId = 100;
}