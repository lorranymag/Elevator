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
    };

    virtual ~Elevator();
    virtual std::string getType() const = 0;

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

    bool enqueueTrip(int pickupFloor, int destinationFloor);
    void update(int elapsedMs);
    bool openDoors();
    bool closeDoors();

protected:
    Elevator(int id, int lowestFloor, int highestFloor, int capacityKg);

private:
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
