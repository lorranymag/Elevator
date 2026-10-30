#include <iostream>
using namespace std;

int main() {
    int numberOfFloors;
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

    return 0;
}
