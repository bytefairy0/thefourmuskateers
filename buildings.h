#ifndef BUILDING_H
#define BUILDING_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

// Base Class: Building
class Building {
protected: 
    string name;
    int level;
    double energyConsumption;
    double pollutionOutput;
    bool isGreenCertified;

public:
    Building(string n, int lvl) : name(n), level(lvl), isGreenCertified(false) {}
    virtual void simulateDay() = 0;
    virtual double getEcoScoreImpact() const = 0;

    virtual void upgrade() {
        if (level >= 5)
            throw runtime_error("Maximum upgrade level reached for " + name);
        level++;
        energyConsumption -= 5;
        pollutionOutput -= 3;
    }

    virtual void displayStats(ofstream& fout) const {
        fout << "Building: " << name
             << " | Level: " << level
             << " | Energy: " << energyConsumption
             << " | Pollution: " << pollutionOutput
             << " | Green Certified: " << (isGreenCertified ? "Yes" : "No") << endl;
    }

    virtual ~Building() {}
};


// Residential Building
class ResidentialBuilding : public Building {
    int residents;

public:
    ResidentialBuilding(string n, int r)
        : Building(n, 1), residents(r) {
        energyConsumption = 80 + r * 2;
        pollutionOutput = 30 + r * 0.5;
    }

    void simulateDay() override {
        energyConsumption += 2;
        pollutionOutput += 0.5;
    }

    double getEcoScoreImpact() const override {
        return isGreenCertified ? -10.0 : pollutionOutput * 0.1;
    }

    void upgrade() override {
        Building::upgrade();
        if (level >= 3) isGreenCertified = true;
    }
};


// Commercial Building
class CommercialBuilding : public Building {
    double income;

public:
    CommercialBuilding(string n)
        : Building(n, 1), income(1000.0) {
        energyConsumption = 200;
        pollutionOutput = 80;
    }

    void simulateDay() override {
        income += 100;
        energyConsumption += 10;
        pollutionOutput += 5;
    }

    double getEcoScoreImpact() const override {
        return isGreenCertified ? -20.0 : pollutionOutput * 0.15;
    }

    void upgrade() override {
        Building::upgrade();
        income += 500;
        if (level >= 4) isGreenCertified = true;
    }
};


// Green Building
class GreenBuilding : public Building {
public:
    GreenBuilding(string n) : Building(n, 1) {
        energyConsumption = 10;
        pollutionOutput = -20;
        isGreenCertified = true;
    }

    void simulateDay() override {
        pollutionOutput -= 1;
    }

    double getEcoScoreImpact() const override {
        return -50.0;
    }

    void upgrade() override {
        Building::upgrade();
        pollutionOutput -= 5;
    }
};

// Industrial Building
class IndustrialBuilding : public Building {
    int unitsProduced;

public:
    IndustrialBuilding(string n)
        : Building(n, 1), unitsProduced(0) {
        energyConsumption = 300;
        pollutionOutput = 150;
    }

    void simulateDay() override {
        unitsProduced += 50;
        energyConsumption += 20;
        pollutionOutput += 15;
    }

    double getEcoScoreImpact() const override {
        return isGreenCertified ? -10.0 : pollutionOutput * 0.2;
    }

    void upgrade() override {
        Building::upgrade();
        if (level >= 3) isGreenCertified = true;
        pollutionOutput -= 10;
    }
};

// Recreational Building
class RecreationalBuilding : public Building {
    int visitors;

public:
    RecreationalBuilding(string n)
        : Building(n, 1), visitors(0) {
        energyConsumption = 50;
        pollutionOutput = -10;
        isGreenCertified = true;
    }

    void simulateDay() override {
        visitors += 100;
        pollutionOutput -= 1;
    }

    double getEcoScoreImpact() const override {
        return -25.0;
    }
};

// Educational Building
class EducationalBuilding : public Building {
    int students;

public:
    EducationalBuilding(string n, int s)
        : Building(n, 1), students(s) {
        energyConsumption = 100;
        pollutionOutput = 20;
    }

    void simulateDay() override {
        students += 10;
        energyConsumption += 5;
        pollutionOutput += 2;
    }

    double getEcoScoreImpact() const override {
        return isGreenCertified ? -15.0 : pollutionOutput * 0.05;
    }

    void upgrade() override {
        Building::upgrade();
        students += 50;
        if (level >= 2) isGreenCertified = true;
    }
};

#endif
