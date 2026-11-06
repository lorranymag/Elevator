#include "Interface.h"
#include "ConsoleInput.h"
#include "PassengerElevator.h"
#include "FreightElevator.h"
#include <chrono>
#include <iostream>
#include <limits>
#include <sstream>
#include <thread>







// constructor for interface class, initializes variables to default values
Interface::Interface()
    : prompt(Prompt::Building), running(true), liveStatusEnabled(false),
    buildingFloors(0), nextElevatorId(1),
    lowestFloor(0), highestFloor(0), selectedId(0), pickupFloor(0), destinationFloor(0),
    setupCapacity(0), creatingFreight(false) {
}

// main loop for user interface, called in main and runs until user exits
void Interface::run() {
    ConsoleInput input;
    std::cout << "ELEVATOR SIMULATOR\n"
        << "Floors are numbered from 1 upward. Enter menu numbers to operate.\n"
        << "Elevators keep moving while you enter setup or call details.\n"
        << "Travel: 4 seconds/floor. Doors: 2s opening, 2s open, 2s closing.\n";
    showPrompt();

    // clock logic to track elapsed time for elevator updates
    // Each loop checks for user input and updates elevators with the same elapsed time
    auto previous = std::chrono::steady_clock::now();
    while (running) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - previous).count();
        if (elapsed > 0) {

            // Preserve sub ms remainder; each elevator receives the same time

            int delta = elapsed > std::numeric_limits<int>::max()
                ? std::numeric_limits<int>::max() : static_cast<int>(elapsed);
            previous += std::chrono::milliseconds(delta);
            for (auto& elevator : elevators) { elevator->update(delta); }
            if (liveStatusEnabled && printStatusChanges()) {
                showPrompt();
                if (input.isInteractive() && !enteringPin()) {
                    std::cout << input.getPendingText() << std::flush;
                }
            }
        }

        std::string line;
        ConsoleInput::Result result = input.poll(line);
        if (result == ConsoleInput::Result::End) {
            break;
        }
        if (result == ConsoleInput::Result::Line) {
            processLine(line);
            input.setHidden(enteringPin());
            if (running) {
                showPrompt();
            }
        }
        // Short pause avoids a busy loop. Input is polled by this same thread.
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    std::cout << "\nExiting elevator simulator. Pending trips are discarded.\n";
}

Elevator* Interface::findElevator(int id) const {
    for (const auto& elevator : elevators) {
        if (elevator->getId() == id) {
            return elevator.get();
        }
    }
    return nullptr;
}

// most of the logic from here below is to handle user input and display, not elevator behavior. The elevator logic is in Elevator.cpp and PassengerElevator.cpp.


void Interface::promptDetails(std::string& label, int& minimum, int& maximum) const {
    minimum = 1;
    maximum = buildingFloors;
    switch (prompt) {
    case Prompt::Building: label = "Number of building floors"; minimum = 5; maximum = 20; break;
    case Prompt::Menu: label = "Choose a menu number"; minimum = 0; maximum = 5; break;
    case Prompt::Settings: label = "Choose a settings option"; minimum = 0; maximum = 1; break;
    case Prompt::Lowest: label = "Lowest accessible floor"; break;
    case Prompt::Highest: label = "Highest accessible floor"; minimum = lowestFloor; break;
    case Prompt::Capacity: label = "Maximum load in kg"; maximum = 10000; break;
    case Prompt::ElevatorId:
        label = "Enter elevator ID (0 to cancel)"; minimum = 0; maximum = nextElevatorId - 1; break;
    case Prompt::SetupPin:
    case Prompt::AccessPin:
        break;
    case Prompt::Load:
        label = dynamic_cast<const FreightElevator*>(findElevator(selectedId))
            ? "Please weigh your cargo and report the weight in kg (whole kg)"
            : "How many people are coming on board? (75 kg per person)";
        minimum = 0;
        maximum = std::numeric_limits<int>::max();
        break;
    case Prompt::Pickup:
    case Prompt::Destination: {
        const Elevator* elevator = findElevator(selectedId);
        minimum = elevator->getLowestFloor();
        maximum = elevator->getHighestFloor();
        label = prompt == Prompt::Pickup ? "Call to floor (pickup)" : "Go to floor (destination)";
        break;
    }
    }
}

// helper function to determine if the user is currently entering a PIN, which requires special handling for input and display
bool Interface::enteringPin() const {
    return prompt == Prompt::SetupPin || prompt == Prompt::AccessPin;
}

// helper function to display the prompt for user input, which varies based on the current state of the interface
void Interface::showPrompt() const {
    if (enteringPin()) {
        std::cout << (prompt == Prompt::SetupPin ? "Set a 4-digit freight PIN: "
            : "Enter the 4-digit freight PIN (0 to cancel): ")
            << std::flush;
        return;
    }
    std::string label;
    int minimum, maximum;
    promptDetails(label, minimum, maximum);
    if (prompt == Prompt::Load) {
        std::cout << label << " (0 to cancel): " << std::flush;
        return;
    }
    std::cout << label << " (" << minimum << "-" << maximum << "): " << std::flush;
}

// helper function to display the current status of all elevators, which is called when the user requests it or when live status printing is enabled
void Interface::showMenu() {
    prompt = Prompt::Menu;
    std::cout << "\n----------------------------------------\n"
        << "Building: " << buildingFloors << " floors | Elevators: " << elevators.size()
        << "\n----------------------------------------\n"
        << "1. Add a passenger elevator\n2. View elevators and queued trips\n"
        << "3. Call an elevator (pickup + destination)\n"
        << "4. Settings (live status: " << (liveStatusEnabled ? "ON" : "OFF") << ")\n"
        << "5. Add a freight elevator\n0. Quit\n"
        << "----------------------------------------\n";
}

// helper function to display the current settings of the interface, which is called when the user requests it
void Interface::showSettings() {
    prompt = Prompt::Settings;
    std::cout << "\n--------------- SETTINGS ----------------\n"
        << "Live status printing: " << (liveStatusEnabled ? "ON" : "OFF") << "\n"
        << "1. Toggle live status printing\n"
        << "0. Back to main menu\n"
        << "Elevators keep running with live printing off.\n"
        << "----------------------------------------\n";
}

// helper function to display the current status of all elevators, which is called when the user requests it or when live status printing is enabled
void Interface::addElevator(int capacity, const std::string& pin) {
    if (creatingFreight) {
        elevators.push_back(std::unique_ptr<Elevator>(new FreightElevator(
            nextElevatorId, lowestFloor, highestFloor, capacity, pin)));
    }
    else {
        elevators.push_back(std::unique_ptr<Elevator>(new PassengerElevator(
            nextElevatorId, lowestFloor, highestFloor, capacity)));
    }
    lastStatus.push_back(statusLine(*elevators.back()));
    std::cout << "\nCreated " << (creatingFreight ? "freight" : "passenger")
        << " elevator #" << nextElevatorId++ << " at floor " << lowestFloor
        << ", idle with doors closed.\n";
    showMenu();
}


// helper function to process a line of user input, which is called when the user presses enter and a complete line is available
void Interface::processLine(const std::string& line) {
    if (enteringPin()) {
        if (prompt == Prompt::AccessPin && line == "0") {
            showMenu();
        }
        else if (!FreightElevator::validPin(line)) {
            std::cout << "\nEnter exactly four digits.\n";
        }
        else if (prompt == Prompt::SetupPin) {
            addElevator(setupCapacity, line);
        }
        else if (findElevator(selectedId)->acceptsPin(line)) {
            tripPin = line;
            prompt = Prompt::Pickup;
        }
        else {
            std::cout << "\nIncorrect freight PIN.\n";
        }
        return;
    }
    std::string label;
    int minimum, maximum, value;
    promptDetails(label, minimum, maximum);
    std::istringstream input(line);
    char extra;
    if (!(input >> value) || (input >> extra) || value < minimum || value > maximum) {
        if (prompt == Prompt::Load) {
            std::cout << "\nEnter a positive whole number, or 0 to cancel.\n";
            return;
        }
        std::cout << "\nEnter one whole number from " << minimum << " to " << maximum << ".\n";
        return;
    }

    switch (prompt) {
    case Prompt::Building:
        buildingFloors = value;
        showMenu();
        break;
    case Prompt::Menu:
        if (value == 0) { running = false; }
        else if (value == 1 || value == 5) {
            if (nextElevatorId == std::numeric_limits<int>::max()) {
                std::cout << "No more elevator IDs are available.\n";
            }
            else {
                creatingFreight = value == 5;
                std::cout << "\nCreate " << (creatingFreight ? "freight" : "passenger")
                    << " elevator #" << nextElevatorId
                    << ". It serves every floor between the two limits.\n";
                prompt = Prompt::Lowest;
            }
        }
        else if (value == 2) {
            displayElevators();
            showMenu();
        }
        else if (value == 4) {
            showSettings();
        }
        else if (elevators.empty()) {
            std::cout << "\nNo elevators yet. Choose 1 to create one.\n";
        }
        else {
            displayElevators();
            prompt = Prompt::ElevatorId;
        }
        break;
    case Prompt::Settings:
        if (value == 0) {
            showMenu();
        }
        else {
            liveStatusEnabled = !liveStatusEnabled;
            // Show one current snapshot when enabled, never replay old updates.
            if (liveStatusEnabled) {
                for (auto& status : lastStatus) { status.clear(); }
            }
            showSettings();
        }
        break;
    case Prompt::Lowest:
        lowestFloor = value;
        prompt = Prompt::Highest;
        break;
    case Prompt::Highest:
        highestFloor = value;
        prompt = Prompt::Capacity;
        break;
    case Prompt::Capacity:
        if (creatingFreight) {
            setupCapacity = value;
            prompt = Prompt::SetupPin;
        }
        else {
            addElevator(value);
        }
        break;
    case Prompt::ElevatorId:
        if (value == 0) { showMenu(); }
        else {
            Elevator* elevator = findElevator(value);
            if (elevator == nullptr) { std::cout << "That elevator ID does not exist.\n"; }
            else if (elevator->getLowestFloor() == elevator->getHighestFloor()) {
                std::cout << "This elevator serves only one floor and cannot provide a trip.\n";
                showMenu();
            }
            else {
                selectedId = value;
                tripPin.clear();
                prompt = dynamic_cast<const FreightElevator*>(elevator)
                    ? Prompt::AccessPin : Prompt::Pickup;
            }
        }
        break;
    case Prompt::Pickup:
        pickupFloor = value;
        std::cout << "\nThe elevator keeps moving while you enter trip details.\n";
        prompt = Prompt::Destination;
        break;
    case Prompt::Destination:
        if (value == pickupFloor) {
            std::cout << "\nDestination must differ from pickup. Select a different floor.\n";
        }
        else {
            destinationFloor = value;
            prompt = Prompt::Load;
        }
        break;
    case Prompt::Load:
        if (value == 0) {
            tripPin.clear();
            showMenu();
        }
        else {
            submitTrip(value);
        }
        break;
    case Prompt::SetupPin:
    case Prompt::AccessPin:
        break; // PINs are handled as strings above to preserve leading zeros.
    }
}

// helper function to handle the user's input based on the current state of the interface, which is called after a valid line of input is received
void Interface::submitTrip(int amount) {
    Elevator* elevator = findElevator(selectedId);
    Elevator::Capacity capacity = elevator->checkCapacity(amount);
    if (capacity == Elevator::Capacity::TooHeavy) {
        std::cout << "\nThis load exceeds the " << elevator->getCapacityKg()
            << " kg maximum. Reduce the load and try again.\n";
        return;
    }

    bool pickupAlreadyPassed = elevator->getState() == Elevator::State::Moving &&
        ((elevator->getDirection() == Elevator::Direction::Up &&
            pickupFloor < elevator->getCurrentFloor()) ||
            (elevator->getDirection() == Elevator::Direction::Down &&
                pickupFloor > elevator->getCurrentFloor()) ||
            (pickupFloor == elevator->getCurrentFloor() && elevator->getElapsedTimeMs() > 0));
    if (elevator->enqueueTrip(pickupFloor, destinationFloor, amount, tripPin)) {
        std::cout << "\nQueued for elevator #" << selectedId << ": pickup "
            << pickupFloor << " -> destination " << destinationFloor << ".\n";
        if (capacity == Elevator::Capacity::Wait) {
            std::cout << "There is not enough room right now. The trip will wait until capacity frees up.\n";
        }
        if (pickupAlreadyPassed) {
            std::cout << "That pickup floor has already been passed; it will wait for a later pass.\n";
        }
    }
    else {
        std::cout << "The elevator cannot accept that trip.\n";
    }
    tripPin.clear();
    showMenu();
}

std::string Interface::statusLine(const Elevator& elevator) const {
    int onboard = 0;
    for (const auto& request : elevator.getPendingRequests()) {
        if (request.pickedUp) { ++onboard; }
    }
    std::ostringstream out;
    out << "#" << elevator.getId() << " " << elevator.getType()
        << " | Floor: " << elevator.getCurrentFloor()
        << " | Direction: " << directionText(elevator.getDirection())
        << " | " << stateText(elevator.getState());
    if (elevator.getState() == Elevator::State::Moving) {
        out << " | Next stop: " << elevator.getTargetFloor();
    }
    out << " | Waiting: " << elevator.getPendingRequests().size() - onboard
        << " | Onboard: " << onboard
        << " | Load: " << elevator.getCurrentLoadKg() << "/" << elevator.getCapacityKg() << " kg";
    return out.str();
}

// helper function to display the current status of all elevators, which is called when the user requests it or when live status printing is enabled
void Interface::displayElevators() const {
    std::cout << "\n------------- ELEVATORS ----------------\n";
    if (elevators.empty()) { std::cout << "No elevators yet. Choose 1 to create one.\n"; }
    for (const auto& elevator : elevators) {
        std::cout << statusLine(*elevator) << "\n  Accessible floors: "
            << elevator->getLowestFloor() << "-" << elevator->getHighestFloor()
            << " | Maximum load: " << elevator->getCapacityKg() << " kg\n";
        for (const auto& request : elevator->getPendingRequests()) {
            std::cout << "  " << (request.pickedUp ? "Onboard" : "Waiting")
                << ": pickup " << request.pickupFloor << " -> destination "
                << request.destinationFloor << " (" << request.weightKg << " kg)\n";
        }
    }
}

// helper function to determine if the user is currently entering a PIN, which requires special handling for input and display
bool Interface::printStatusChanges() {
    bool changed = false;
    for (std::size_t i = 0; i < elevators.size(); ++i) {
        std::string status = statusLine(*elevators[i]);
        if (status != lastStatus[i]) {
            if (!changed) { std::cout << "\n\n[Live elevator status]\n"; changed = true; }
            std::cout << status << '\n';
            lastStatus[i] = status;
        }
    }
    return changed;
}

// helper function to convert the elevator's direction enum to a string for display, which is called when showing the status of elevators
std::string Interface::directionText(Elevator::Direction direction) const {
    switch (direction) {
    case Elevator::Direction::Up: return "Up";
    case Elevator::Direction::Down: return "Down";
    case Elevator::Direction::None: return "None";
    }
    return "Unknown";
}

// helper function to convert the elevator's state enum to a string for display, which is called when showing the status of elevators
std::string Interface::stateText(Elevator::State state) const {
    switch (state) {
    case Elevator::State::Idle: return "Idle (doors closed)";
    case Elevator::State::Moving: return "Moving (between floors)";
    case Elevator::State::OpeningDoors: return "Opening doors";
    case Elevator::State::DoorsOpen: return "Doors open";
    case Elevator::State::ClosingDoors: return "Closing doors";
    }
    return "Unknown";
}
