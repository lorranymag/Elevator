#ifndef PASSENGER_ELEVATOR_H
#define PASSENGER_ELEVATOR_H
#include "Elevator.h"
#include <vector>

// Child class of Elevator
class PassengerElevator : public Elevator {
public:
    PassengerElevator(int id, int currentFloor, int maxCapacity);
    void loadPassengers(int count);
    void unloadPassengers();

    // Request system
    void addRequest(int floor);
    void displayRequests() const;
    void processRequests();
private:
    int maxCapacity;
    int currentLoad;

    // Stores multiple floor requests
    std::vector<int> requests;
};
#endif PASSENGER_ELEVATOR_H
