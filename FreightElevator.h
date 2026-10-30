#ifndef FREIGHT_ELEVATOR_H
#define FREIGHT_ELEVATOR_H

class FreightElevator : Elevator {
public:
	FreightElevator(int id, int currentFloor, int maxCapacity);
	void loadCargo(int weight);
	void unloadCargo();
private:
	int maxCapacity;
	int currentLoad;
};

#endif  FREIGHT_ELEVATOR_H