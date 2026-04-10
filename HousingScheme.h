#ifndef HOUSINGSCHEME_H
#define HOUSINGSCHEME_H

#include "Address.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <iostream>

using namespace std;

// Forward declarations
class PollutionControl;
class City;

/**
 * HousingScheme class that manages residential units
 * Base class for different types of housing
 */
class HousingScheme {
private:
    string schemeName;
    Address location;
    int totalUnits;
    int occupiedUnits;
    double sustainabilityRating; // 0-100 rating

protected:
    // Vector to track building pollution
    vector<double> pollutionReadings;

public:
    // Constructor
    HousingScheme(const string& name, const Address& addr, int units, double rating = 50.0)
        : schemeName(name), location(addr), totalUnits(units), occupiedUnits(0), sustainabilityRating(rating) {
        if (units <= 0) {
            throw invalid_argument("Number of units must be positive");
        }
        if (rating < 0 || rating > 100) {
            throw invalid_argument("Sustainability rating must be between 0 and 100");
        }
    }

    // Virtual destructor
    virtual ~HousingScheme() = default;

    // Accessors and mutators
    string getSchemeName() const { return schemeName; }
    Address getLocation() const { return location; }
    int getTotalUnits() const { return totalUnits; }
    int getOccupiedUnits() const { return occupiedUnits; }
    double getSustainabilityRating() const { return sustainabilityRating; }

    void setSustainabilityRating(double rating) {
        if (rating < 0 || rating > 100) {
            throw invalid_argument("Sustainability rating must be between 0 and 100");
        }
        sustainabilityRating = rating;
    }

    // Occupy a unit
    virtual bool occupyUnit() {
        if (occupiedUnits < totalUnits) {
            occupiedUnits++;
            return true;
        }
        return false;
    }

    // Vacate a unit
    virtual bool vacateUnit() {
        if (occupiedUnits > 0) {
            occupiedUnits--;
            return true;
        }
        return false;
    }

    // Calculate occupancy rate
    double getOccupancyRate() const {
        return static_cast<double>(occupiedUnits) / totalUnits * 100.0;
    }

    // Add pollution reading
    void addPollutionReading(double reading) {
        if (reading < 0) {
            throw invalid_argument("Pollution reading cannot be negative");
        }
        pollutionReadings.push_back(reading);
    }

    // Get average pollution reading
    double getAveragePollution() const {
        if (pollutionReadings.empty()) {
            return 0.0;
        }
        double total = 0.0;
        for (double reading : pollutionReadings) {
            total += reading;
        }
        return total / pollutionReadings.size();
    }

    // Virtual function to display housing scheme details
    virtual void displayDetails() const {
        cout << "Housing Scheme: " << schemeName << endl;
        cout << "Location: " << location << endl;
        cout << "Units: " << occupiedUnits << "/" << totalUnits << " occupied (" 
              << getOccupancyRate() << "%)" << endl;
        cout << "Sustainability Rating: " << sustainabilityRating << "/100" << endl;
        cout << "Average Pollution Level: " << getAveragePollution() << endl;
    }

    // Declare PollutionControl as friend class
    friend class PollutionControl;
    friend class City;
};

// Apartment complex derived class
class ApartmentComplex : public HousingScheme {
private:
    int floors;
    bool hasElevator;
    bool hasSolarPanels;

public:
    ApartmentComplex(const string& name, const Address& addr, int units, int flrs, 
                     bool elevator = true, bool solar = false, double rating = 60.0)
        : HousingScheme(name, addr, units, rating), floors(flrs), 
          hasElevator(elevator), hasSolarPanels(solar) {
        if (flrs <= 0) {
            throw invalid_argument("Number of floors must be positive");
        }
    }

    int getFloors() const { return floors; }
    bool getHasElevator() const { return hasElevator; }
    bool getHasSolarPanels() const { return hasSolarPanels; }

    void installSolarPanels() {
        hasSolarPanels = true;
        // Improve sustainability rating when solar panels are installed
        setSustainabilityRating(getSustainabilityRating() + 15.0 > 100.0 ? 100.0 : getSustainabilityRating() + 15.0);
    }

    // Override display details to include apartment-specific information
    void displayDetails() const override {
        HousingScheme::displayDetails();
        cout << "Type: Apartment Complex" << endl;
        cout << "Floors: " << floors << endl;
        cout << "Elevator: " << (hasElevator ? "Yes" : "No") << endl;
        cout << "Solar Panels: " << (hasSolarPanels ? "Yes" : "No") << endl;
    }
};

// Villa complex derived class
class VillaComplex : public HousingScheme {
private:
    double averagePlotSize; // in square meters
    bool hasSwimmingPool;
    bool hasGreenSpace;

public:
    VillaComplex(const string& name, const Address& addr, int units, 
                double plotSize, bool pool = false, bool green = true, double rating = 40.0)
        : HousingScheme(name, addr, units, rating), averagePlotSize(plotSize), 
          hasSwimmingPool(pool), hasGreenSpace(green) {
        if (plotSize <= 0) {
            throw invalid_argument("Plot size must be positive");
        }
    }

    double getAveragePlotSize() const { return averagePlotSize; }
    bool getHasSwimmingPool() const { return hasSwimmingPool; }
    bool getHasGreenSpace() const { return hasGreenSpace; }

    void addGreenSpace() {
        hasGreenSpace = true;
        // Improve sustainability rating when green space is added
        setSustainabilityRating(getSustainabilityRating() + 10.0 > 100.0 ? 100.0 : getSustainabilityRating() + 10.0);
    }

    // Override display details to include villa-specific information
    void displayDetails() const override {
        HousingScheme::displayDetails();
        cout << "Type: Villa Complex" << endl;
        cout << "Average Plot Size: " << averagePlotSize << " sq. meters" << endl;
        cout << "Swimming Pool: " << (hasSwimmingPool ? "Yes" : "No") << endl;
        cout << "Green Space: " << (hasGreenSpace ? "Yes" : "No") << endl;
    }
};

#endif // HOUSINGSCHEME_H