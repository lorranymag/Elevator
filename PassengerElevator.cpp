class PassengerElevator : public Elevator {
public:
	PassengerElevator(int id, int currentFloor, int maxCapacity);
	void loadPassengers(int count);
	void unloadPassengers();
private:
	int maxCapacity;
	int currentLoad;
};