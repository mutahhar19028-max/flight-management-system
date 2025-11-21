#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

class IdGenerator {
private:
    static int _m_lastCustomerId;
    static int _m_lastTicketId;
    static int _m_lastFlightId;

public:
    static int generateCustomerId();
    static int generateTicketId();
    static int generateFlightId();
    
    static void setLastCustomerId(int id);
    static void setLastTicketId(int id);
    static void setLastFlightId(int id);
    
    static int getLastCustomerId();
    static int getLastTicketId();
    static int getLastFlightId();
    
    static void reset();
};

#endif