#include "FreightElevator.h"
#include <stdexcept>

FreightElevator::FreightElevator(int id, int lowestFloor, int highestFloor,
    int capacityKg, const std::string& pin)
    : Elevator(id, lowestFloor, highestFloor, capacityKg), accessPin(pin) {
    if (!validPin(pin)) { throw std::invalid_argument("Freight PIN must have four digits."); }
}

std::string FreightElevator::getType() const { return "Freight"; }

bool FreightElevator::acceptsPin(const std::string& pin) const { return pin == accessPin; }

bool FreightElevator::validPin(const std::string& pin) {
    return pin.size() == 4 && pin.find_first_not_of("0123456789") == std::string::npos;
}

int FreightElevator::loadWeightKg(int cargoKg) const { return cargoKg; }
