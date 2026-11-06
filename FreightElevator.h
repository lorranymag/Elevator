#ifndef FREIGHT_ELEVATOR_H
#define FREIGHT_ELEVATOR_H

#include "Elevator.h"

class FreightElevator : public Elevator {
public:
    FreightElevator(int id, int lowestFloor, int highestFloor, int capacityKg,
        const std::string& pin);
    std::string getType() const override;
    bool acceptsPin(const std::string& pin) const override;
    static bool validPin(const std::string& pin);

protected:
    int loadWeightKg(int cargoKg) const override;

private:
    std::string accessPin;
};

#endif
