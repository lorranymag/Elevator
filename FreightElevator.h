

#ifndef FREIGHT_ELEVATOR_H
#define FREIGHT_ELEVATOR_H
#include "ELEVATOR.h"
#include <vector>



//child class of Elevator. Contains functions and variables specific to freight elevators
class FreightElevator : public Elevator {
public:
	FreightElevator(int id, int currentFloor, int numFloors, int maxCapacity);
	void loadCargo(int cargoWeight);
	void unloadCargo(int cargoWeight);
	int getMaxCapacity() const;
	std::vector<int> callOrder;
	int weight; // current weight of cargo in the elevator


	protected:
		int maxCapacity;


};

#endif  FREIGHT_ELEVATOR_H