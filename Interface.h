#ifndef INTERFACE_H
#define INTERFACE_H

#include "Elevator.h"
#include <memory>
#include <string>
#include <vector>

class Interface {
public:
    Interface();
    void run();

private:
    // Each setup/call prompt is a stage, so no prompt blocks elevator updates
    enum class Prompt { Building, Menu, Settings, Lowest, Highest, Capacity, SetupPin, ElevatorId, AccessPin, Pickup, Destination, Load };
    Prompt prompt;
    bool running;
    bool liveStatusEnabled;
    int buildingFloors;
    int nextElevatorId;
    int lowestFloor;
    int highestFloor;
    int selectedId;
    int pickupFloor;
    int destinationFloor;
    int setupCapacity;
    bool creatingFreight;
    std::string tripPin;
    std::vector<std::unique_ptr<Elevator>> elevators;
    std::vector<std::string> lastStatus;

    void processLine(const std::string& line);
    void promptDetails(std::string& label, int& minimum, int& maximum) const;
    void showPrompt() const;
    void showMenu();
    void showSettings();
    bool enteringPin() const;
    void addElevator(int capacity, const std::string& pin = "");
    void submitTrip(int amount);
    void displayElevators() const;
    bool printStatusChanges();
    Elevator* findElevator(int id) const;
    std::string statusLine(const Elevator& elevator) const;
    std::string directionText(Elevator::Direction direction) const;
    std::string stateText(Elevator::State state) const;
};

#endif
