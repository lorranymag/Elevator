#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <string>
#include <vector>

class Elevator {
public:

    // enums for direction and state of elevator help keep code readable
    enum class Direction { None, Up, Down };
    enum class State { Idle, Moving, OpeningDoors, DoorsOpen, ClosingDoors };

    // struct for requets to be stored in a vector for each elevator, since each request has multiple variables
    struct Request {
        int pickupFloor;
        int destinationFloor;
        bool pickedUp;
        int weightKg;
    };

    virtual ~Elevator();
    virtual std::string getType() const = 0;
    virtual bool acceptsPin(const std::string& pin) const;
    enum class Capacity { TooHeavy, Wait, Fits };
    Capacity checkCapacity(int amount) const;

    int getId() const;
    int getLowestFloor() const;
    int getHighestFloor() const;
    int getCurrentFloor() const;
    int getTargetFloor() const;
    int getCapacityKg() const;
    int getCurrentLoadKg() const;
    int getTravelTimeMs() const;
    int getDoorTimeMs() const;
    int getElapsedTimeMs() const;
    Direction getDirection() const;
    State getState() const;
    const std::vector<Request>& getPendingRequests() const;
    bool canAccessFloor(int floor) const;

    bool enqueueTrip(int pickupFloor, int destinationFloor, int amount = 1,
        const std::string& pin = "");
    void update(int elapsedMs);
    bool openDoors();
    bool closeDoors();

protected:
    Elevator(int id, int lowestFloor, int highestFloor, int capacityKg);

    // Passenger amounts count people; freight amounts are already kilograms.
    virtual int loadWeightKg(int amount) const = 0;

private:
    bool hasRoom(int weightKg) const;
    Direction tripDirection(const Request& request) const;
    bool hasPassengers() const;
    void chooseNextStop();
    void prepareStop();
    void serveCurrentFloor();
    bool isOnTheWay(const Request& request) const;
    int nearestStopAhead() const;
    void refreshMovingTarget();

    int id;
    int lowestFloor;
    int highestFloor;
    int capacityKg;
    int currentFloor; // Last reached floor while traveling between floors.
    int targetFloor;  // Next scheduled stop, not necessarily the final destination.
    int currentLoadKg;
    Direction direction;
    State state;
    std::vector<Request> pendingRequests;
    int travelTimeMs;
    int doorTimeMs;
    int doorHoldTimeMs;
    int elapsedTimeMs; // Time in the current adjacent-floor step or door phase.
};

#endif
