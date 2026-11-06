#include "PassengerElevator.h"


// passenger elevators inherit the vast majority of their functionality from parent elevator, so not many changes are needed now.
PassengerElevator::PassengerElevator(int id, int lowestFloor, int highestFloor,
    int capacityKg)
    : Elevator(id, lowestFloor, highestFloor, capacityKg) {
}

std::string PassengerElevator::getType() const {
    return "Passenger";
}
