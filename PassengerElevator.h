#ifndef PASSENGER_ELEVATOR_H
#define PASSENGER_ELEVATOR_H

#include "Elevator.h"

// Uses the shared base behavior; passenger specific rules can be added later.
// mostly fine for now, freight ele might need more differences
class PassengerElevator : public Elevator {
public:
    PassengerElevator(int id, int lowestFloor, int highestFloor, int capacityKg);
    std::string getType() const override;
};

#endif
