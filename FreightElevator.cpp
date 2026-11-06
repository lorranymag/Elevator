#include "FreightElevator.h"
#include <stdexcept>


// freight elevators inherit the vast majority of their functionality from parent elevator, so not many changes are needed now.
FreightElevator::FreightElevator(int id, int lowestFloor, int highestFloor,
    int capacityKg, const std::string& pin)
    : Elevator(id, lowestFloor, highestFloor, capacityKg), accessPin(pin) {
    if (!validPin(pin)) { throw std::invalid_argument("Freight PIN must have four digits."); }
}

// freight elevators override the getType() function to return "Freight" instead of "Passenger"
std::string FreightElevator::getType() const { return "Freight"; }


// freight elevators override the acceptsPin() function to check if the provided pin matches the access pin
bool FreightElevator::acceptsPin(const std::string& pin) const { return pin == accessPin; }

// helper function to check if a given pin is valid (four digits)
bool FreightElevator::validPin(const std::string& pin) {
    return pin.size() == 4 && pin.find_first_not_of("0123456789") == std::string::npos;
}

//  helper function to return the weight of the cargo in kilograms, which is already provided as input for freight elevators
int FreightElevator::loadWeightKg(int cargoKg) const { return cargoKg; }
