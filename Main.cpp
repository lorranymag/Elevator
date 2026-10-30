#include <iostream>
#include "Elevator.h"
#include "FreightElevator.h"
#include "PassengerElevator.h"
using namespace std;


/** Basic flow :
		1)Main calls interface to give user options to configure the building and elevators
		2)Interface will have a menu system to allow user to call elevators, load/unload cargo or passengers, and view elevator status
        3)Elevator class will have core functionality shared between elevators
		4)PassengerElevator and FreightElevator will inherit from Elevator and have their own specific functionality
**/

int main() {


	FreightElevator f1(0, 0, 10,1000); // int id, int currentFloor,int numFloors, int maxCapacity
	
    f1.getStatus();





    /**int numberOfFloors;
    int numberOfElevators;

    cout << "===== Elevator System Configuration =====" << endl;

    cout << "Enter the number of floors (5-20): ";
    cin >> numberOfFloors;

    while (numberOfFloors < 5 || numberOfFloors > 20) {
        cout << "Invalid number of floors." << endl;
        cout << "Please enter a number between 5 and 20: ";
        cin >> numberOfFloors;
    }

    cout << "Enter the number of elevators: ";
    cin >> numberOfElevators;

    while (numberOfElevators < 1) {
        cout << "There must be at least 1 elevator." << endl;
        cout << "Enter the number of elevators: ";
        cin >> numberOfElevators;
    }

    cout << endl;
    cout << "Building configuration complete!" << endl;
    cout << "Number of floors: " << numberOfFloors << endl;
    cout << "Number of elevators: " << numberOfElevators << endl;


    **/
    return 0;
}
