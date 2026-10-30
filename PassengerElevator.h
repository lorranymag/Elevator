#ifndef PASSENGER_ELEVATOR_H
#define PASSENGER_ELEVATOR_H
#include "Elevator.h"

//child class of Elevator. Contains functions and variables specific to passenger elevators
class PassengerElevator : public Elevator {
public:
	PassengerElevator(int id, int currentFloor, int maxCapacity);
	void loadPassengers(int count);
	void unloadPassengers();
private:
	int maxCapacity;
	int currentLoad;
};

#endif  PASSENGER_ELEVATOR_H
