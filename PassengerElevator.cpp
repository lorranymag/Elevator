#include "PassengerElevator.h"


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