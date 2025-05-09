
#ifdef transport
#ifndef transport
#endif
#include <iostream>
#include <vector>

using namespace std;

class Transport;
class Bicycle;
class Car;
class Bike;
class Plane;
class Train;
class Bus;

class Transport {
protected:
    double distance;
    double fuelAmount;
    double fuelCost;
    string typeOfFuel;
    int engineSize;
    string vehicle;
    double carbonEmissions = 0.0;

protected:
    double getEmissionFactor() const {
        if (typeOfFuel == "petrol") return 4.18;
        if (typeOfFuel == "diesel") return 3.56;
        if (typeOfFuel == "cng") return 1.98;
        if (typeOfFuel == "electric") return 0.0;
        return 1.0;
    }

public:
    Transport(double d, double fA, double fC, string tof, int eS, string veh)
        : distance(d), fuelAmount(fA), fuelCost(fC), typeOfFuel(tof), engineSize(eS), vehicle(veh) {}

    virtual void calculateCarbonEmissions() = 0;
    virtual void calculateTotalFuelCost() = 0;

    virtual void displayInfo() {
        calculateCarbonEmissions();
        cout << "Type of vehicle: " << vehicle;
        cout << "\nDistance: " << distance << " km.";
        cout << "\nAmount of Fuel: " << fuelAmount << " litres.";
        cout << "\nCost of fuel: $" << fuelCost << endl;
        calculateTotalFuelCost();
        cout << "Engine size: " << engineSize;
        cout << "\nThe amount of carbon emissions produced: " << carbonEmissions << " kg CO2." << endl;
        cout << "\n---------------------------------\n";
    }

    double getCarbonEmissions() const {
        return carbonEmissions;
    }
    
    virtual ~Transport() {}
};

class Bicycle : public Transport {
public:
    Bicycle(double d) : Transport(d, 0.0, 0.0, "None", 0, "Bicycle") {}

    void calculateCarbonEmissions() override {
        carbonEmissions = 0.0;
        cout << "Carbon emissions for bicycle: " << carbonEmissions << " kg CO2 (zero emissions)\n";
    }

    void calculateTotalFuelCost() override {
        cout << "Total fuel used: 0 liters.\n";
    }

    void displayInfo() override {
        calculateCarbonEmissions();
        cout << "\n------Bicycle Info------\n";
        Transport::displayInfo();
        cout << "Eco-friendly transport as a bicycle uses no fuel !!\n";
    }
};

class Car : public Transport {
public:
    Car(double d, double fA, double fC, string tof, int eS)
        : Transport(d, fA, fC, tof, eS, "Car") {}

    void calculateCarbonEmissions() override {
        carbonEmissions = fuelAmount * getEmissionFactor() * distance;
        cout << "Carbon emissions for car: " << carbonEmissions << " kg CO2\n";
    }

    void calculateTotalFuelCost() override {
        double totalCost = fuelAmount * 1.5 * getEmissionFactor();
        cout << "Total fuel cost for car: $" << totalCost << "\n";
    }

    void displayInfo() override {
        cout << "\n------Car Info------\n";
        Transport::displayInfo();
    }
};

class Bus : public Transport {
public:
    Bus(double d, double fA, double fC, string tof, int eS)
        : Transport(d, fA, fC, tof, eS, "Bus") {}

    void calculateCarbonEmissions() override {
        carbonEmissions = fuelAmount * getEmissionFactor() * distance;
    }

    void calculateTotalFuelCost() override {
        double totalCost = fuelAmount * 2.0 * getEmissionFactor();
        cout << "Total fuel cost for bus: $" << totalCost << "\n";
    }

    void displayInfo() override {
        cout << "\n------Bus Info------\n";
        Transport::displayInfo();
    }
};

class Train : public Transport {
public:
    Train(double d, double fA, double fC, string tof, int eS)
        : Transport(d, fA, fC, tof, eS, "Train") {}

    void calculateCarbonEmissions() override {
        carbonEmissions = fuelAmount * getEmissionFactor() * distance;
    }

    void calculateTotalFuelCost() override {
        double totalCost = fuelAmount * 1.8 * getEmissionFactor();
        cout << "Total fuel cost for train: $" << totalCost << "\n";
    }

    void displayInfo() override {
        cout << "\n------Train Info------\n";
        Transport::displayInfo();
    }
};

class Plane : public Transport {
public:
    Plane(double d, double fA, double fC, string tof, int eS)
        : Transport(d, fA, fC, tof, eS, "Plane") {}

    void calculateCarbonEmissions() override {
        carbonEmissions = fuelAmount * getEmissionFactor() * distance;
    }

    void calculateTotalFuelCost() override {
        double totalCost = fuelAmount * 3.0 * getEmissionFactor();
        cout << "Total fuel cost for plane: $" << totalCost << "\n";
    }

    void displayInfo() override {
        cout << "\n------Plane Info------\n";
        Transport::displayInfo();
    }
};

class Bike : public Transport {
public:
    Bike(double d, double fA, double fC, string tof, int eS)
        : Transport(d, fA, fC, tof, eS, "Bike") {}

    void calculateCarbonEmissions() override {
        carbonEmissions = fuelAmount * getEmissionFactor() * distance;
    }

    void calculateTotalFuelCost() override {
        double totalCost = fuelAmount * 1.3 * getEmissionFactor();
        cout << "Total fuel cost for bike: $" << totalCost << "\n";
    }

    void displayInfo() override {
        cout << "\n------Bike Info------\n";
        Transport::displayInfo();
    }
};

// Function to handle vehicle creation and menu interaction
void manageCityVehicles(vector<Transport*>& cityVehicles) {
    int choice;
    double distance, fuelAmount, fuelCost;
    int engineSize;
    string typeOfFuel;

    do {
        cout << "\nChoose a vehicle to add to your city:\n";
        cout << "1. Car\n2. Bike\n3. Plane\n4. Train\n5. Bus\n6. Bicycle\n0. Finish & Display Info\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice != 6) {
            cout << "Enter distance, fuel amount, fuel cost, fuel type, engine size:\n";
            cin >> distance >> fuelAmount >> fuelCost >> typeOfFuel >> engineSize;
        } else {
            cout << "Enter distance:\n"; 
            cin >> distance;
        }

        switch (choice) {
            case 1:
                cityVehicles.push_back(new Car(distance, fuelAmount, fuelCost, typeOfFuel, engineSize));
                break;
            case 2: 
                cityVehicles.push_back(new Bike(distance, fuelAmount, fuelCost, typeOfFuel, engineSize));
                break;
            case 3: 
                cityVehicles.push_back(new Plane(distance, fuelAmount, fuelCost, typeOfFuel, engineSize));
                break;
            case 4: 
                cityVehicles.push_back(new Train(distance, fuelAmount, fuelCost, typeOfFuel, engineSize));
                break;
            case 5:
                cityVehicles.push_back(new Bus(distance, fuelAmount, fuelCost, typeOfFuel, engineSize));
                break;
            case 6:
                cityVehicles.push_back(new Bicycle(distance));
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (true);
}

int main() {
    vector<Transport*> cityVehicles;
    manageCityVehicles(cityVehicles);

    cout << "\n-------City Vehicle Summary-------\n";
    double totalCarbonEmissions = 0.0;
    for (auto vehicle : cityVehicles) {
        vehicle->displayInfo();
        totalCarbonEmissions += vehicle->getCarbonEmissions();
    }

    cout << "\nTotal carbon emissions from all vehicles: " 
         << totalCarbonEmissions << " kg CO2.\n";

    for (auto vehicle : cityVehicles)
        delete vehicle;
    
    return 0;
}
