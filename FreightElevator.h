

#ifndef FREIGHT_ELEVATOR_H
#define FREIGHT_ELEVATOR_H
#include "ELEVATOR.h"
#include <vector>


class FreightElevator : public Elevator {
public:
	FreightElevator(int id, int currentFloor, int maxCapacity);
	void loadCargo(int weight);
	void unloadCargo();
	int getMaxCapacity() const;
	std::vector<int> callOrder;
	int weight; // current weight of cargo in the elevator


	protected:
		int maxCapacity;


};

#endif  FREIGHT_ELEVATOR_H