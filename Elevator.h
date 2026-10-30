#ifndef ELEVATOR_H
#define ELEVATOR_H

class Elevator {

public:
	Elevator();

	void moveToFloor(int floor);
	void callElevator(int currentFloor);
	int getCurrentFloor() const;
	void setId();
	int getId() const;
	void openDoor();
	void closeDoor();
	void getStatus() const;

	int id; int numFloors; int currentFloor; bool doorOpen; bool moving; bool movingUp;


};




#endif ELEVATOR_H
