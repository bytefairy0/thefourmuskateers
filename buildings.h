#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

/**
 * Building base class for all city structures
 */
class Building {
private:
    string name;
    double ecoScoreImpact;
    int capacity;
    
public:
    // Constructor
    Building(const string& buildingName, double impact = 0.0, int cap = 0)
        : name(buildingName), ecoScoreImpact(impact), capacity(cap) {}
    
    // Virtual destructor
    virtual ~Building() = default;
    
    // Getters
    string getName() const { return name; }
    double getEcoScoreImpact() const { return ecoScoreImpact; }
    int getCapacity() const { return capacity; }
    
    // Setters
    void setEcoScoreImpact(double impact) { ecoScoreImpact = impact; }
    void setCapacity(int cap) { capacity = cap; }
    
    // Virtual function to display building info
    virtual void displayInfo() const {
        cout << "Building: " << name << endl;
        cout << "Eco Score Impact: " << ecoScoreImpact << endl;
        cout << "Capacity: " << capacity << endl;
    }
    
    // Virtual function to update eco score
    virtual void updateEcoScore() {
        // Base implementation does nothing
    }
};

/**
 * Residential building class
 */
class ResidentialBuilding : public Building {
private:
    int residents;
    
public:
    // Constructor
    ResidentialBuilding(const string& name, int numResidents = 0)
        : Building(name, 5.0, numResidents), residents(numResidents) {}
    
    // Getter and setter
    int getResidents() const { return residents; }
    void setResidents(int num) { residents = num; setCapacity(num); }
    
    // Override display info
    void displayInfo() const override {
        cout << "\n----- Residential Building -----" << endl;
        Building::displayInfo();
        cout << "Current Residents: " << residents << endl;
    }
    
    // Override update eco score
    void updateEcoScore() override {
        // Each resident has some impact on eco score
        setEcoScoreImpact(5.0 + (residents * 0.5));
    }
};

/**
 * Commercial building class
 */
class CommercialBuilding : public Building {
private:
    int businessCount;
    double energyUsage;
    
public:
    // Constructor
    CommercialBuilding(const string& name, int businesses = 0, double energy = 100.0)
        : Building(name, 10.0, 50), businessCount(businesses), energyUsage(energy) {}
    
    // Getters and setters
    int getBusinessCount() const { return businessCount; }
    double getEnergyUsage() const { return energyUsage; }
    
    void setBusinessCount(int count) { businessCount = count; }
    void setEnergyUsage(double usage) { energyUsage = usage; }
    
    // Override display info
    void displayInfo() const override {
        cout << "\n----- Commercial Building -----" << endl;
        Building::displayInfo();
        cout << "Businesses: " << businessCount << endl;
        cout << "Energy Usage: " << energyUsage << " kWh" << endl;
    }
    
    // Override update eco score
    void updateEcoScore() override {
        // Business count and energy usage impacts eco score
        setEcoScoreImpact(10.0 + (businessCount * 2.0) + (energyUsage * 0.05));
    }
};

/**
 * Green building class
 */
class GreenBuilding : public Building {
private:
    double solarOutput;
    bool rainwaterHarvesting;
    double greenSpaceArea;
    
public:
    // Constructor
    GreenBuilding(const string& name, double solar = 50.0, bool rainwater = true, double greenSpace = 100.0)
        : Building(name, -50.0, 30), solarOutput(solar), rainwaterHarvesting(rainwater), greenSpaceArea(greenSpace) {}
    
    // Getters and setters
    double getSolarOutput() const { return solarOutput; }
    bool hasRainwaterHarvesting() const { return rainwaterHarvesting; }
    double getGreenSpaceArea() const { return greenSpaceArea; }
    
    void setSolarOutput(double output) { solarOutput = output; }
    void setRainwaterHarvesting(bool has) { rainwaterHarvesting = has; }
    void setGreenSpaceArea(double area) { greenSpaceArea = area; }
    
    // Override display info
    void displayInfo() const override {
        cout << "\n----- Green Building -----" << endl;
        Building::displayInfo();
        cout << "Solar Output: " << solarOutput << " kW" << endl;
        cout << "Rainwater Harvesting: " << (rainwaterHarvesting ? "Yes" : "No") << endl;
        cout << "Green Space Area: " << greenSpaceArea << " sq. meters" << endl;
    }
    
    // Override update eco score
    void updateEcoScore() override {
        // Green features have positive impact (negative value means positive for environment)
        double impact = -50.0;
        impact -= (solarOutput * 0.5);
        if (rainwaterHarvesting) impact -= 20.0;
        impact -= (greenSpaceArea * 0.1);
        setEcoScoreImpact(impact);
    }
};

/**
 * Industrial building class
 */
class IndustrialBuilding : public Building {
private:
    double pollutionRate;
    bool hasPollutionControl;
    
public:
    // Constructor
    IndustrialBuilding(const string& name, double pollution = 75.0, bool control = false)
        : Building(name, 100.0, 20), pollutionRate(pollution), hasPollutionControl(control) {}
    
    // Getters and setters
    double getPollutionRate() const { return pollutionRate; }
    bool getHasPollutionControl() const { return hasPollutionControl; }
    
    void setPollutionRate(double rate) { pollutionRate = rate; }
    void setHasPollutionControl(bool has) { hasPollutionControl = has; }
    
    // Override display info
    void displayInfo() const override {
        cout << "\n----- Industrial Building -----" << endl;
        Building::displayInfo();
        cout << "Pollution Rate: " << pollutionRate << " units" << endl;
        cout << "Pollution Control: " << (hasPollutionControl ? "Installed" : "Not Installed") << endl;
    }
    
    // Override update eco score
    void updateEcoScore() override {
        // Pollution rate affects eco score
        double impact = 100.0;
        impact += pollutionRate;
        if (hasPollutionControl) impact *= 0.5; // Reduce impact by half if control measures are in place
        setEcoScoreImpact(impact);
    }
};

/**
 * Recreational building class
 */
class RecreationalBuilding : public Building {
private:
    double greenArea;
    int visitorCapacity;
    
public:
    // Constructor
    RecreationalBuilding(const string& name, double green = 500.0, int visitors = 100)
        : Building(name, -20.0, visitors), greenArea(green), visitorCapacity(visitors) {}
    
    // Getters and setters
    double getGreenArea() const { return greenArea; }
    int getVisitorCapacity() const { return visitorCapacity; }
    
    void setGreenArea(double area) { greenArea = area; }
    void setVisitorCapacity(int capacity) { 
        visitorCapacity = capacity;
        setCapacity(capacity);
    }
    
    // Override display info
    void displayInfo() const override {
        cout << "\n----- Recreational Building -----" << endl;
        Building::displayInfo();
        cout << "Green Area: " << greenArea << " sq. meters" << endl;
        cout << "Visitor Capacity: " << visitorCapacity << endl;
    }
    
    // Override update eco score
    void updateEcoScore() override {
        // Green area has positive impact, but visitor capacity can increase negative impact
        double impact = -20.0;
        impact -= (greenArea * 0.01);
        impact += (visitorCapacity * 0.05);
        setEcoScoreImpact(impact);
    }
};

/**
 * Educational building class
 */
class EducationalBuilding : public Building {
private:
    int students;
    double energyEfficiency; // 0-100 rating
    
public:
    // Constructor
    EducationalBuilding(const string& name, int numStudents = 0, double efficiency = 70.0)
        : Building(name, 0.0, numStudents), students(numStudents), energyEfficiency(efficiency) {}
    
    // Getters and setters
    int getStudents() const { return students; }
    double getEnergyEfficiency() const { return energyEfficiency; }
    
    void setStudents(int num) { 
        students = num;
        setCapacity(num);
    }
    void setEnergyEfficiency(double eff) { energyEfficiency = eff; }
    
    // Override display info
    void displayInfo() const override {
        cout << "\n----- Educational Building -----" << endl;
        Building::displayInfo();
        cout << "Students: " << students << endl;
        cout << "Energy Efficiency: " << energyEfficiency << "/100" << endl;
    }
    
    // Override update eco score
    void updateEcoScore() override {
        // Students increase impact, but efficiency can decrease it
        double impact = students * 0.1;
        impact *= (1.0 - (energyEfficiency / 100.0)); // Higher efficiency means lower impact
        setEcoScoreImpact(impact);
    }
};

#endif // BUILDINGS_H
