#ifndef ELECTRICITYMANAGEMENT_H
#define ELECTRICITYMANAGEMENT_H

#include "Services.h"
#include "Address.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <cmath>

// Forward declaration
class Services;

enum class ElectricityPlan {
    NO_SERVICE,
    BASIC,
    STANDARD,
    PREMIUM,
    RENEWABLE
};

struct ElectricityTariff {
    double baseCost;
    double pricePerUnit;
    double renewablePercentage;
    string displayName;
};

class ElectricityManagement : public Services {
private:
    Address address;
    double totalUsageKWh;
    ElectricityPlan currentPlan;
    static map<ElectricityPlan, ElectricityTariff> planDetails;

public:
    // Constructor
    ElectricityManagement(Address ad, ElectricityPlan plan = ElectricityPlan::NO_SERVICE, double initialUsage = 0.0)
        : Services("Electricity"), address(ad), currentPlan(plan), totalUsageKWh(initialUsage) {
        if (initialUsage < 0) {
            throw invalid_argument("Initial electricity usage cannot be negative.");
        }
    }

    // Add electricity usage
    void addUsage(double kWh) {
        if (kWh < 0) {
            throw invalid_argument("Electricity usage cannot be negative.");
        }
        totalUsageKWh += kWh;
    }

    // Calculate bill
    double calculateBill() const {
        if (currentPlan == ElectricityPlan::NO_SERVICE) {
            return 0.0;
        }
        
        auto& tariff = planDetails[currentPlan];
        return tariff.baseCost + (totalUsageKWh * tariff.pricePerUnit);
    }

    // Calculate carbon footprint
    double calculateCarbonFootprint() const {
        if (currentPlan == ElectricityPlan::NO_SERVICE) {
            return 0.0;
        }
        
        // Get renewable percentage
        double renewable = planDetails[currentPlan].renewablePercentage;
        
        // Carbon emission factor for standard grid electricity (kg CO2 per kWh)
        const double STANDARD_EMISSION_FACTOR = 0.5;
        
        // Adjust based on renewable percentage
        double emissionFactor = STANDARD_EMISSION_FACTOR * (1.0 - renewable / 100.0);
        
        return totalUsageKWh * emissionFactor;
    }

    // --- Getters ---
    const Address& getAddress() const { return address; }
    double getTotalUsageKWh() const { return totalUsageKWh; }
    ElectricityPlan getCurrentPlan() const { return currentPlan; }
    string getPlanName() const { return planDetails[currentPlan].displayName; }

    // --- Setters ---
    void setCurrentPlan(ElectricityPlan newPlan) {
        currentPlan = newPlan;
        totalUsageKWh = 0.0; // Reset usage when plan changes
        cout << "Electricity plan changed to: " << getPlanName() << endl;
    }

    // --- Virtual Functions ---
    void supply() override {
        cout << "Managing electricity supply (" << getPlanName() << ") "
             << "for address: [" << address << "]" << endl;
        if (currentPlan == ElectricityPlan::NO_SERVICE) {
            cout << "STATUS: No electricity service active" << endl;
        } else {
            cout << "Current usage: " << totalUsageKWh << " kWh" << endl;
            cout << "STATUS: Power supply active" << endl;
        }
    }

    void showStatus() override {
        cout << "\n--- Electricity Service ---" << endl;
        cout << "Current Plan: " << getPlanName() << endl;
        if (currentPlan != ElectricityPlan::NO_SERVICE) {
            cout << "Usage: " << totalUsageKWh << " kWh" << endl;
            cout << "Renewable Percentage: " << planDetails[currentPlan].renewablePercentage << "%" << endl;
            cout << "Carbon Footprint: " << calculateCarbonFootprint() << " kg CO2" << endl;
            cout << "Estimated Bill: $" << calculateBill() << endl;
        } else {
            cout << "STATUS: No active service." << endl;
        }
        cout << "-------------------------" << endl;
    }

    virtual ~ElectricityManagement() override = default;
};

// Initialize static member
map<ElectricityPlan, ElectricityTariff> ElectricityManagement::planDetails = {
    {ElectricityPlan::NO_SERVICE, {0.0, 0.0, 0.0, "No Service"}},
    {ElectricityPlan::BASIC, {10.0, 0.12, 5.0, "Basic Plan"}},
    {ElectricityPlan::STANDARD, {15.0, 0.10, 10.0, "Standard Plan"}},
    {ElectricityPlan::PREMIUM, {25.0, 0.09, 20.0, "Premium Plan"}},
    {ElectricityPlan::RENEWABLE, {30.0, 0.11, 100.0, "100% Renewable Plan"}}
};

#endif // ELECTRICITYMANAGEMENT_H
