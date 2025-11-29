#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

using std::string;

class Customer {
private:
    int customerId;
    string name;
    string phone;
    string dob; 
    int numOfFlights;
    int milesTraveled;

public:
    Customer();
    Customer(int id, string n, string ph, string dateOfBirth, int flights, int miles);

    int getCustomerId() const;
    string getName() const;
    string getPhone() const;
    string getDob() const;
    int getNumOfFlights() const;
    int getMilesTraveled() const;

    void setCustomerId(int id);
    void setName(string n);
    void setPhone(string ph);
    void setDob(string dateOfBirth);
    void setNumOfFlights(int flights);
    void setMilesTraveled(int miles);

    void updateNumOfFlights(int increment = 1);
    void updateMilesTraveled(int miles);

    void display() const;
};

#endif