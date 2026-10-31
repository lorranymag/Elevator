#include "PassengerElevator.h"
#include <iostream>
#include <algorithm>

PassengerElevator::PassengerElevator(int id, int currentFloor, int maxCapacity) {
    this->id = id;
    this->currentFloor = currentFloor;
    this->maxCapacity = maxCapacity;
    this->currentLoad = 0;
}

void PassengerElevator::loadPassengers(int count) {
    if (this->currentLoad + count <= this->maxCapacity) {
        this->currentLoad += count;
    }
    else {
        std::cout << "Cannot load passengers. Exceeds maximum capacity." << std::endl;
    }
}

void PassengerElevator::unloadPassengers() {
    this->currentLoad = 0;
}

void PassengerElevator::addRequest(int floor) {
    bool floorAlreadyRequested = false;

    for (int i = 0; i < this->requests.size(); i++) {
        if (this->requests.at(i) == floor) {
            floorAlreadyRequested = true;
            return;
        }
    }

    if (floorAlreadyRequested == false) {
        this->requests.push_back(floor);
    }
}

void PassengerElevator::displayRequests() const {
    if (this->requests.empty()) {
        std::cout << "No pending requests." << std::endl;
    }
    else {
        std::cout << "Pending floor requests: ";

        for (int i = 0; i < this->requests.size(); i++) {
            std::cout << this->requests.at(i) << " ";
        }

        std::cout << std::endl;
    }
}

void PassengerElevator::processRequests() {
    if (this->requests.empty()) {
        std::cout << "No requests to process." << std::endl;
        return;
    }

    std::sort(this->requests.begin(), this->requests.end());

    for (int i = 0; i < this->requests.size(); i++) {
        this->moveToFloor(this->requests.at(i));
    }

    this->requests.clear();
}
