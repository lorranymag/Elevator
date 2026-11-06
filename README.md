# Elevator

**By Ryan Ferguson and Lorrany Evangelista S M**

## Project Description

This project is an elevator simulation program developed in C++. The purpose of the project is to simulate how elevators operate in a building, including floor movement, elevator requests, and different elevator types.

## Features

- Simulates elevator movement between floors.
- Supports passenger and freight elevator types.
- Allows users to interact with the simulation through a console-based menu.
- Manages elevator requests and floor selection.
- Tracks elevator information and status.
- Uses input validation to handle user entries.
- Organizes functionality into separate classes and files.

## Programming Language

- C++

## Project Files

The project contains the following files:

- **Main.cpp** - Starting point of the program. Creates an Interface object and runs the simulation.
- **Elevator.h / Elevator.cpp** - Base elevator class containing common elevator properties and functionality.
- **PassengerElevator.h / PassengerElevator.cpp** - Defines the passenger elevator class, which inherits from Elevator.
- **FreightElevator.h / FreightElevator.cpp** - Defines the freight elevator class, which inherits from Elevator.
- **Interface.h / Interface.cpp** - Handles the console menu, user interaction, and simulation controls.
- **ConsoleInput.h / ConsoleInput.cpp** - Handles console input and input validation.
- **Elevator.slnx** - Solution file.
- **Elevator.vcxproj** - Project configuration.

## How to Run the Program

1. Download the GitHub repository.
2. Open the `Elevator.slnx` solution file.
3. Make sure all project files are included.
4. Run the program.

## Quick Tips/Hints

1. Instructions and interactions are managed by the console
2. User Interface starts with real time status toggled OFF, if you want to see the real time elevator updates: follow menu prompts to toggle.
3. FOR EASE OF EVALUATION: Please start by creating a building of 20 floors, create one passenger elevator that spans all 20 floors, then make multiple calls for that elevator BEFORE toggling the live status ON, I adjusted the timing of the elevators moving/opening/closing to make it easier to stack calls/view testing.
4. When testing the freight elevators, remember that you will need to create a pin# for each freight elevator. This pin# will be required for every call to the freight elevator(like a staff elevator) and lost pins cost $20 to reset.

## How to Use the Program

1. Start the program.
2. Follow the instructions displayed in the console.
3. Console asks user for #floors in building (select 20 for ease of testing).
4. Select option 1 to add a passeneger elevator OR 5 for a freight(you will create a pin for each freight elevator to call it).
5. Follow console prompts, entering ONE number at a time for lowest floor, THEN highest floor, and finally maximum load in kg(if you select a number below 75 the elevator will refuse to pick up a single passenger).
6. After creating an elevator, call it by selecting option 3. You will be prompted to enter the elevator's ID, each elevator will be displayed on the console. Console will ask user for pickup floor, then destination floor.
7. The console will then ask for how many people coming on board(using 75kg per person as an average to check against capacity for passenger elevators).
8. After calling an elevator you can select option 4, then option 1 to toggle the live status and see the movement of the elevator.(keeping the toggle off will make it easier to read the console, you can also select option 2 for a stamped elevator queue/status.)
9. Add elevator trips and make calls as desired, enter 0 in the console menu to exit.

## Object-Oriented Programming Concepts

### Classes and Objects
The program uses separate classes to represent elevators and manage different parts of the simulation.

## Project Goal

The goal of this project is to practice C++ programming and Object-Oriented Programming concepts by creating a simulation of an elevator system.

This project also helps develop skills in:
- Working with multiple header and source files.
- Creating and using classes.
- Implementing inheritance.
- Handling user input.
- Organizing and maintaining code.
- Designing a console-based application.


## ROLES AND RESPONSIBILITIES
-Lorrany provided core logic for handling elevator behavior, README.
-Ryan assisted with building an interface for the user to interact with.
-Brainstormed together on essential features, hierarchy, general project structure.