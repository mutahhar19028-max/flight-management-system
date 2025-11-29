#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <map>
#include <vector>

using std::string;

class Plane {
private:
    int planeId;
    string description;
    int numberOfMilesTraveled;
    std::map<string, int> numberOfSeatsPerClass; // {"Economy": 80, "Business": 20}
    std::map<string, int> classPricing; // {"Economy": 400, "Business": 1000}

public:
    Plane();
    Plane(int id, string desc, int miles);

    int getPlaneId() const;
    string getDescription() const;
    int getNumberOfMilesTraveled() const;
    std::map<string, int> getSeatsPerClass() const;
    std::map<string, int> getClassPricing() const;
    int getSeatsForClass(string className) const;
    int getPriceForClass(string className) const;
    int getTotalSeats() const;

    void setPlaneId(int id);
    void setDescription(string desc);
    void setNumberOfMilesTraveled(int miles);
    void setSeatsPerClass(std::map<string, int> seats);
    void setClassPricing(std::map<string, int> pricing);
    void addSeatClass(string className, int numSeats);
    void addClassPrice(string className, int price);

    void updateNumberOfMilesTraveled(int miles);

    void display() const;
};

#endif