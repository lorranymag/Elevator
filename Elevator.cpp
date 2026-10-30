#include "Elevator.h"
#include <vector>
#include <IOStream>



class Elevator {

public:
	
	Elevator() {}
	void moveToFloor(int floor) {}
	void callElevator(int currentFloor) {}
	int getCurrentFloor() const {}
	void setId() {
		this->id = std::rand() % 1000 + 1;
	}
	int getId() const {}	
	void openDoor() {}
	void closeDoor() {}
	void getStatus() const {
		std::cout << "Elevator ID: " << id << std::endl;
		std::cout << "Current Floor: " << currentFloor << std::endl;
		std::cout << "Door Open: " << (doorOpen ? "Yes" : "No") << std::endl;
		std::cout << "Moving: " << (moving ? "Yes, " : "No");
		if (moving) {
			std::cout << (movingUp ? "Going up" : "Going down") << std::endl;
		}
		
	}

	int id; int numFloors; int currentFloor; bool doorOpen; bool moving; bool movingUp;


};

