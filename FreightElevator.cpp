#include "FreightElevator.h"
#include <vector>
#include <IOStream>


//child class of Elevator. Contains functions and variables specific to freight elevators

FreightElevator::FreightElevator(int id, int currentFloor,int numFloors, int maxCapacity) {
		this->currentFloor = currentFloor;
		this->maxCapacity = maxCapacity;
		this->callOrder = std::vector<int>();
		this->numFloors = numFloors;
		this->weight = 0;
		this->doorOpen = false;
		this->moving = false;
		this->movingUp = false;
	}

	void FreightElevator::loadCargo(int cargoWeight) {

		if (this->weight + cargoWeight <= this->maxCapacity) {
			this->weight += cargoWeight;
		}
		else {
			std::cout << "Cannot load cargo. Exceeds maximum capacity." << std::endl;
		}
	}

	void FreightElevator::unloadCargo(int cargoWeight) {
	
		if (this->weight > 0 && this->weight-cargoWeight >= 0) {
			this->weight -= cargoWeight;
		}
		else {
			std::cout << "No cargo to unload." << std::endl;
		}
	
	
	}
	int FreightElevator::getMaxCapacity() const {
		return this->maxCapacity;
	}
	std::vector<int> callOrder;
	int weight; // current weight of cargo in the elevator

	/**void callFreight(int callFloor) {
		
		bool floorAlreadyCalled = false;

		for (int i = 0; (i < this->callOrder.size() && !floorAlreadyCalled); i++) {

			if (callOrder.at(i) == callFloor) {
				floorAlreadyCalled = true;
				return; //do nothing, floor already in call order
			}
		}
		if (floorAlreadyCalled == false) {
			this->callOrder.push_back(callFloor);
		}
	}**/

	/**void FreightElevator::getStatus() const {
		std::cout << "Elevator ID: " << id << std::endl;
		std::cout << "Current Floor: " << currentFloor << std::endl;
		std::cout << "Door Open: " << (doorOpen ? "Yes" : "No") << std::endl;
		std::cout << "Moving: " << (moving ? "Yes, " : "No");
		if (moving) {
			std::cout << (movingUp ? "Going up" : "Going down") << std::endl;
		}
		std::cout << "Max Capacity: " << maxCapacity << std::endl;
		std::cout << "Current Load: " << weight << std::endl;
	}
	**/
