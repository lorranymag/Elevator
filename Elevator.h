#ifndef ELEVATOR_H
#define ELEVATOR_H

class Elevator {


public:
	Elevator(int id, int currentFloor);
	void moveToFloor(int floor);
	int getCurrentFloor() const;
	int getId() const;
};




#endif ELEVATOR_H
