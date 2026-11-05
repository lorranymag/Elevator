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

1. Download or clone the GitHub repository.
2. Open the `Elevator.slnx` solution file.
3. Make sure all project files are included.
4. Run the program.

## How to Use the Program

1. Start the program.
2. Follow the instructions displayed in the console.
3. Select an option from the menu.
4. Enter the requested elevator information.
5. Use the available options to simulate elevator operations.
6. Follow the menu instructions to continue or exit the program.

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
