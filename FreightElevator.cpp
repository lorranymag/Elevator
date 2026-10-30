class FreightElevator : Elevator {
public:
	FreightElevator(int id, int currentFloor, int maxCapacity);
	void loadCargo(int weight);
	void unloadCargo();
private:
	int maxCapacity;
	int currentLoad;
};