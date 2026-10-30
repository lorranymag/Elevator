#ifndef ELEVATOR_H
#define ELEVATOR_H

class Elevator {


public:
	Elevator(int id, int numFloors, int currentFloor);
	void moveToFloor(int floor);
	void callElevator(int currentFloor);
	int getCurrentFloor() const;
	int getId() const;
	void openDoor();
	void closeDoor();

	int id; int numFloors; int currentFloor; bool doorOpen; bool moving; int direction;


};




#endif ELEVATOR_H
