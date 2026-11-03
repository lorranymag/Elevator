#include "Elevator.h"
#include <cstdlib>
#include <stdexcept>

Elevator::Elevator(int id, int lowestFloor, int highestFloor, int capacityKg)
    : id(id), lowestFloor(lowestFloor), highestFloor(highestFloor),
    capacityKg(capacityKg), currentFloor(lowestFloor), targetFloor(lowestFloor),
    currentLoadKg(0), direction(Direction::None), state(State::Idle),
    travelTimeMs(2000), doorTimeMs(1000), doorHoldTimeMs(1000), elapsedTimeMs(0) {
    if (id < 1 || lowestFloor < 1 || highestFloor < lowestFloor || capacityKg < 1) {
        throw std::invalid_argument("Invalid elevator configuration.");
    }
}

Elevator::~Elevator() = default;
int Elevator::getId() const { return id; }
int Elevator::getLowestFloor() const { return lowestFloor; }
int Elevator::getHighestFloor() const { return highestFloor; }
int Elevator::getCurrentFloor() const { return currentFloor; }
int Elevator::getTargetFloor() const { return targetFloor; }
int Elevator::getCapacityKg() const { return capacityKg; }
int Elevator::getCurrentLoadKg() const { return currentLoadKg; }
int Elevator::getTravelTimeMs() const { return travelTimeMs; }
int Elevator::getDoorTimeMs() const { return doorTimeMs; }
int Elevator::getElapsedTimeMs() const { return elapsedTimeMs; }
Elevator::Direction Elevator::getDirection() const { return direction; }
Elevator::State Elevator::getState() const { return state; }
const std::vector<Elevator::Request>& Elevator::getPendingRequests() const {
    return pendingRequests;
}

bool Elevator::canAccessFloor(int floor) const {
    return floor >= lowestFloor && floor <= highestFloor;
}

Elevator::Direction Elevator::tripDirection(const Request& request) const {
    return request.destinationFloor > request.pickupFloor ? Direction::Up : Direction::Down;
}

bool Elevator::hasPassengers() const {
    for (const auto& request : pendingRequests) {
        if (request.pickedUp) { return true; }
    }
    return false;
}

bool Elevator::isOnTheWay(const Request& request) const {
    int floor = request.pickedUp ? request.destinationFloor : request.pickupFloor;
    bool compatible = request.pickedUp || tripDirection(request) == direction;
    bool ahead = (direction == Direction::Up && floor > currentFloor) ||
        (direction == Direction::Down && floor < currentFloor);
    return compatible && ahead;
}

int Elevator::nearestStopAhead() const {
    int nearest = 0; // Floors start at 1; zero means no eligible stop ahead.
    for (const auto& request : pendingRequests) {
        if (isOnTheWay(request)) {
            int floor = request.pickedUp ? request.destinationFloor : request.pickupFloor;
            if (nearest == 0 || std::abs(floor - currentFloor) < std::abs(nearest - currentFloor)) {
                nearest = floor;
            }
        }
    }
    return nearest;
}

void Elevator::refreshMovingTarget() {
    if (state != State::Moving) { return; }

    // At the exact floor boundary we can still stop here. Once travel toward
    // the next floor has begun, currentFloor is behind us and cannot be served.
    if (elapsedTimeMs == 0) {
        for (const auto& request : pendingRequests) {
            bool pickupHere = !request.pickedUp && request.pickupFloor == currentFloor &&
                tripDirection(request) == direction;
            bool dropoffHere = request.pickedUp && request.destinationFloor == currentFloor;
            if (pickupHere || dropoffHere) {
                targetFloor = currentFloor;
                prepareStop();
                return;
            }
        }
    }

    int nearest = nearestStopAhead();
    if (nearest != 0 && std::abs(nearest - currentFloor) < std::abs(targetFloor - currentFloor)) {
        targetFloor = nearest;
    }
}

bool Elevator::enqueueTrip(int pickupFloor, int destinationFloor) {
    if (!canAccessFloor(pickupFloor) || !canAccessFloor(destinationFloor) ||
        pickupFloor == destinationFloor) {
        return false;
    }
    Request request = { pickupFloor, destinationFloor, false };
    pendingRequests.push_back(request);
    if (state == State::Idle) {
        chooseNextStop();
    }
    else if (state == State::Moving) {
        refreshMovingTarget();
    }
    else if (state == State::DoorsOpen && pickupFloor == currentFloor &&
        tripDirection(request) == direction) {
        serveCurrentFloor();
        elapsedTimeMs = 0; // Give a newly boarding passenger a full door hold.
    }
    return true;
}

void Elevator::chooseNextStop() {
    elapsedTimeMs = 0;
    if (pendingRequests.empty()) {
        targetFloor = currentFloor;
        direction = Direction::None;
        state = State::Idle;
        return;
    }

    // First, serve compatible calls at this floor without an unnecessary move.
    bool occupied = hasPassengers();
    for (const auto& request : pendingRequests) {
        if ((!request.pickedUp && request.pickupFloor == currentFloor &&
            (tripDirection(request) == direction || !occupied)) ||
            (request.pickedUp && request.destinationFloor == currentFloor)) {
            targetFloor = currentFloor;
            prepareStop();
            return;
        }
    }

    int nearest = nearestStopAhead();
    if (nearest != 0) {
        targetFloor = nearest;
    }
    else {
        // No stop remains in this sweep. Reposition toward the oldest waiting
        // trip. Its destination cannot become eligible before its pickup.
        const Request& request = pendingRequests.front();
        targetFloor = request.pickedUp ? request.destinationFloor : request.pickupFloor;
        direction = targetFloor > currentFloor ? Direction::Up : Direction::Down;
    }
    state = State::Moving;
}

void Elevator::prepareStop() {
    // An empty elevator may turn around to collect a waiting passenger here.
    // An occupied elevator only boards people traveling in its current direction.
    if (!hasPassengers()) {
        bool matching = false;
        for (const auto& request : pendingRequests) {
            if (!request.pickedUp && request.pickupFloor == currentFloor &&
                tripDirection(request) == direction) {
                matching = true;
            }
        }
        if (!matching) {
            for (const auto& request : pendingRequests) {
                if (!request.pickedUp && request.pickupFloor == currentFloor) {
                    direction = tripDirection(request);
                    break;
                }
            }
        }
    }
    state = State::Idle;
    openDoors();
}

void Elevator::serveCurrentFloor() {
    // Build the remaining queue in one pass instead of repeatedly shifting it.
    std::vector<Request> remaining;
    remaining.reserve(pendingRequests.size());
    for (Request request : pendingRequests) {
        if (request.pickedUp && request.destinationFloor == currentFloor) { continue; }
        if (!request.pickedUp && request.pickupFloor == currentFloor &&
            tripDirection(request) == direction) {
            request.pickedUp = true;
        }
        remaining.push_back(request);
    }
    pendingRequests.swap(remaining);
}

bool Elevator::openDoors() {
    if (state != State::Idle) { return false; }
    state = State::OpeningDoors;
    elapsedTimeMs = 0;
    return true;
}

bool Elevator::closeDoors() {
    if (state != State::DoorsOpen) { return false; }
    state = State::ClosingDoors;
    elapsedTimeMs = 0;
    return true;
}

void Elevator::update(int elapsedMs) {
    if (elapsedMs <= 0) { return; }
    while (elapsedMs > 0) {
        // Reevaluate the whole queue on each tick and between floor steps.
        // Retargeting never resets an in-progress travel interval or reverses it.
        refreshMovingTarget();
        if (state == State::Idle) {
            chooseNextStop();
            if (state == State::Idle) { return; }
        }
        int duration = doorTimeMs;
        if (state == State::Moving) { duration = travelTimeMs; }
        else if (state == State::DoorsOpen) { duration = doorHoldTimeMs; }

        int remaining = duration - elapsedTimeMs;
        int step = elapsedMs < remaining ? elapsedMs : remaining;
        elapsedTimeMs += step;
        elapsedMs -= step;
        if (elapsedTimeMs < duration) { return; }
        elapsedTimeMs = 0;

        switch (state) {
        case State::Moving:
            currentFloor += direction == Direction::Up ? 1 : -1;
            if (currentFloor == targetFloor) { prepareStop(); }
            break;
        case State::OpeningDoors:
            state = State::DoorsOpen;
            serveCurrentFloor();
            break;
        case State::DoorsOpen:
            closeDoors();
            break;
        case State::ClosingDoors:
            state = State::Idle;
            chooseNextStop();
            break;
        case State::Idle:
            break;
        }
    }
}
