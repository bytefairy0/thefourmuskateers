#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

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
    
    // Getter for vehicle type
    string getType() const { return vehicle; }
    
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
private:
    string ownerName;

public:
    Car(double d, double fA, double fC, string tof, int eS, string owner = "")
        : Transport(d, fA, fC, tof, eS, "Car"), ownerName(owner) {}

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
        if (!ownerName.empty()) {
            cout << "Owner: " << ownerName << "\n";
        }
    }
};

class Bus : public Transport {
private:
    string governmentDepartment;

public:
    Bus(double d, double fA, double fC, string tof, int eS, string govDept = "City Transit Authority")
        : Transport(d, fA, fC, tof, eS, "Bus"), governmentDepartment(govDept) {}

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
        cout << "Operated by: " << governmentDepartment << "\n";
    }
};

class Train : public Transport {
private:
    string railwayCompany;

public:
    Train(double d, double fA, double fC, string tof, int eS, string company = "National Railways")
        : Transport(d, fA, fC, tof, eS, "Train"), railwayCompany(company) {}

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
        cout << "Railway Company: " << railwayCompany << "\n";
    }
};

class Plane : public Transport {
private:
    string airline;

public:
    Plane(double d, double fA, double fC, string tof, int eS, string airlineName = "")
        : Transport(d, fA, fC, tof, eS, "Plane"), airline(airlineName) {}

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
        if (!airline.empty()) {
            cout << "Airline: " << airline << "\n";
        }
    }
};

class Bike : public Transport {
private:
    string ownerName;

public:
    Bike(double d, double fA, double fC, string tof, int eS, string owner = "")
        : Transport(d, fA, fC, tof, eS, "Bike"), ownerName(owner) {}

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
        if (!ownerName.empty()) {
            cout << "Owner: " << ownerName << "\n";
        }
    }
};

// Function to handle vehicle creation and menu interaction
void manageCityVehicles(vector<Transport*>& cityVehicles) {
    int choice;
    double distance, fuelAmount, fuelCost;
    int engineSize;
    string typeOfFuel, ownerInfo;

    do {
        cout << "\nChoose a vehicle to add to your city:\n";
        cout << "1. Car\n2. Bike\n3. Plane\n4. Train\n5. Bus\n6. Bicycle\n0. Finish\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 0) break;

        cout << "Enter distance: ";
        cin >> distance;

        if (choice != 6) {
            cout << "Enter fuel amount:\n";
            cin >> fuelAmount;
            cout << "Enter fuel cost: ";
            cin >> fuelCost;
            cout << "Enter type of fuel (diesel/petrol/cng/electric): ";
            cin >> typeOfFuel;
            cout << "Enter engine size: ";
            cin >> engineSize;
            
            // Get owner or government info based on vehicle type
            cin.ignore(); // Clear buffer
            if (choice == 1 || choice == 2) { // Car or Bike
                cout << "Enter owner name: ";
                getline(cin, ownerInfo);
            } else if (choice == 5) { // Bus
                cout << "Enter government department: ";
                getline(cin, ownerInfo);
            } else if (choice == 3) { // Plane
                cout << "Enter airline name: ";
                getline(cin, ownerInfo);
            } else if (choice == 4) { // Train
                cout << "Enter railway company: ";
                getline(cin, ownerInfo);
            }
        }

        switch (choice) {
            case 1:
                cityVehicles.push_back(new Car(distance, fuelAmount, fuelCost, typeOfFuel, engineSize, ownerInfo));
                break;
            case 2: 
                cityVehicles.push_back(new Bike(distance, fuelAmount, fuelCost, typeOfFuel, engineSize, ownerInfo));
                break;
            case 3: 
                cityVehicles.push_back(new Plane(distance, fuelAmount, fuelCost, typeOfFuel, engineSize, ownerInfo));
                break;
            case 4: 
                cityVehicles.push_back(new Train(distance, fuelAmount, fuelCost, typeOfFuel, engineSize, ownerInfo));
                break;
            case 5:
                cityVehicles.push_back(new Bus(distance, fuelAmount, fuelCost, typeOfFuel, engineSize, ownerInfo));
                break;
            case 6:
                cityVehicles.push_back(new Bicycle(distance));
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (true);
}

#endif // TRANSPORT_H