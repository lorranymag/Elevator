#include "PassengerElevator.h"


// passenger elevators inherit the vast majority of their functionality from parent elevator, so not many changes are needed now.
PassengerElevator::PassengerElevator(int id, int lowestFloor, int highestFloor,
    int capacityKg)
    : Elevator(id, lowestFloor, highestFloor, capacityKg) {
}

// passenger elevators override the getType() function to return "Passenger" instead of "Freight"
std::string PassengerElevator::getType() const {
    return "Passenger";
}

// helper function to return the weight of the passengers in kilograms, which is estimated based on the number of people and an average adult weight
int PassengerElevator::loadWeightKg(int people) const {
    const int estimatedAdultKg = 75;
    if (people <= 0 || people > getCapacityKg() / estimatedAdultKg) { return 0; }
    return people * estimatedAdultKg;
}
