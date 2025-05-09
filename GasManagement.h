#ifndef GASMANAGEMENT_H 
#define GASMANAGEMENT_H

#include "Services.h"       
#include "HousingScheme.h"  
#include <string>
#include <iostream>
#include <stdexcept> 
using namespace std;      

class GasManagement : public Services {
private:
    Address address;          
    double totalConsumption;  // Total gas consumed in cubic meters (m³)
    static double gridUnitPrice;

public:
    GasManagement(Address ad, double initialConsumption = 0.0)
        : Services("Gas"), 
        address(ad),
        totalConsumption(initialConsumption)
    {
        if (initialConsumption < 0) {
            throw invalid_argument("Initial gas consumption cannot be negative.");
        }
        cout << "Gas Management initialized for address: [" << address.display() << "]" << std::endl;
    }

    
    void addUsage(double m3) {
        if (m3 < 0) {
            throw std::invalid_argument("Gas usage cannot be negative.");
            // return; // Unreachable code after throw
        }
        totalConsumption += m3;
        // std::cout << "Added " << m3 << " m³ gas usage for address: [" << address.display() << "]. Total: " << totalConsumption << " m³." << std::endl; // Optional logging
    }

    // --- Billing ---
    double calculateBill() const { // Marked const as it doesn't modify the object's state
        if (totalConsumption <= 0) {
            return 0.0; // No charge if no consumption
        }
        return totalConsumption * gridUnitPrice;
    }

    // --- Getters (Accessors) ---
    const Address& getAddress() const { return address; }
    double getTotalConsumption() const { return totalConsumption; }
    static double getGridUnitPrice() { return gridUnitPrice; } // Static getter

    // --- Setters (Mutators) ---
    static void SetGridUnitPrice(double newPrice) {
        if (newPrice < 0) {
            throw invalid_argument("Gas unit price cannot be negative.");
        }
        gridUnitPrice = newPrice;
        // std::cout << "Static Gas Grid Unit Price updated to: $" << gridUnitPrice << std::endl; // Optional logging
    }

    // --- Overridden Virtual Functions ---
    void supply() override {
        std::cout << "Managing Gas supply for address: [" << address.display() << "]" << std::endl;
        // Future logic could involve checking grid availability, pressure, etc.
        std::cout << "   Status: Grid connection active. Current consumption rate implies stable supply." << std::endl;
    }

    // Displays the current status of the gas service for this address.
    void showStatus() override { 
        std::cout << "--- Gas Status for Address: [" << address.display() << "] ---" << std::endl;
        std::cout << "   Total Consumption This Period: " << totalConsumption << " m³" << std::endl;
        std::cout << "   Grid Unit Price: $" << gridUnitPrice << "/m³" << std::endl;
        std::cout << "   Estimated Grid Bill: $" << calculateBill() << std::endl;
        std::cout << "-------------------------------------------" << std::endl;
    }

    // --- Virtual Destructor ---
        virtual ~GasManagement() override = default; 
};

#endif 