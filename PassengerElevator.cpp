#include "PassengerElevator.h"

PassengerElevator::PassengerElevator(int id, int lowestFloor, int highestFloor,
    int capacityKg)
    : Elevator(id, lowestFloor, highestFloor, capacityKg) {
}

std::string PassengerElevator::getType() const {
    return "Passenger";
}
