#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <string>
#include <vector>

class Elevator {
public:
    enum class Direction { None, Up, Down };
    enum class State { Idle, Moving, OpeningDoors, DoorsOpen, ClosingDoors };

    // A trip stays in the queue until its passenger reaches the destination
	// Requests need multiple attributes so a struct is used instead of a pair or tuple
    struct Request {
        int pickupFloor;
        int destinationFloor;
        bool pickedUp;
    };

	// The base class is abstract; each elevator type must implement getType().
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

	//prtected methods are available to derived classes for implementing type specific rules
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
