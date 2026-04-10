#ifndef WATERMANAGEMENT_H
#define WATERMANAGEMENT_H

#include "Services.h"
#include "Address.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept> 
#include <algorithm>
#include <limits>

using namespace std;

// different tariff plans for water usage
enum class WaterTariffPlan {
    NO_SUPPLY,
    RESIDENTIAL_CONSERVATION, // Lower usage, incentivized rates
    RESIDENTIAL_STANDARD,
    COMMERCIAL_STANDARD
};

// Represents a single tier in a tiered pricing structure
struct TariffTier {
    double limitCubicMeters; // Consumption limit for this tier (e.g., first 10 m^3)
                             // For the last tier, this can be numeric_limits<double>::infinity()
    double pricePerCubicMeter; // Price for consumption within this tier
};

// Holds all details for a specific water tariff plan
struct WaterPlanInfo {
    string displayName;
    vector<TariffTier> tiers; // List of pricing tiers
};

class WaterManagement : public Services {
private:
    Address address;
    double consumptionCubicMeters;
    WaterTariffPlan currentPlan;

    // Static plan details map
    static map<WaterTariffPlan, WaterPlanInfo> planDetailsStore;

    // Helper to get plan details from the static map
    const WaterPlanInfo& getCurrentPlanInfo() const {
        auto it = planDetailsStore.find(currentPlan);
        if (it == planDetailsStore.end()) {
            throw runtime_error("Water plan details not found for current plan.");
        }
        return it->second;
    }

public:
    // Constructor
    WaterManagement(Address ad, WaterTariffPlan plan = WaterTariffPlan::NO_SUPPLY, double initialConsumption = 0.0)
        : Services("Water"), address(ad), currentPlan(plan), consumptionCubicMeters(initialConsumption) {
        if (initialConsumption < 0) {
            throw invalid_argument("Initial water consumption cannot be negative.");
        }
    }

    // Method to add water consumption
    void addConsumption(double cubicMeters) {
        if (cubicMeters < 0) {
            throw invalid_argument("Water consumption to add cannot be negative.");
        }
        consumptionCubicMeters += cubicMeters;
    }

    // Calculate the bill based on tiered pricing
    double calculateBill() const {
        if (currentPlan == WaterTariffPlan::NO_SUPPLY) {
            return 0.0;
        }

        const WaterPlanInfo& planInfo = getCurrentPlanInfo();
        double totalBill = 0.0;
        double remainingConsumption = consumptionCubicMeters;
        double consumptionInPreviousTiers = 0.0;

        for (const auto& tier : planInfo.tiers) {
            if (remainingConsumption <= 0) break;

            // Determine how much consumption falls into this tier
            // The actual limit of this tier relative to the start of consumption
            double tierUpperBoundary = tier.limitCubicMeters;
            // Consumption applicable to this tier calculation step
            double consumptionInThisTier = min(remainingConsumption, tierUpperBoundary - consumptionInPreviousTiers);
            
            if (consumptionInThisTier < 0) consumptionInThisTier = 0; // Should not happen with correct tier setup

            totalBill += consumptionInThisTier * tier.pricePerCubicMeter;
            remainingConsumption -= consumptionInThisTier;
            consumptionInPreviousTiers += consumptionInThisTier; // Update for next tier's perspective
        }

        return totalBill;
    }

    // --- Getters ---
    const Address& getAddress() const { return address; }
    double getConsumptionCubicMeters() const { return consumptionCubicMeters; }
    WaterTariffPlan getCurrentPlanType() const { return currentPlan; }
    string getPlanDisplayName() const { return getCurrentPlanInfo().displayName; }

    // --- Setter for changing plan ---
    void setCurrentPlan(WaterTariffPlan newPlan) {
        currentPlan = newPlan;
        consumptionCubicMeters = 0.0; // Reset consumption for new billing period/plan
        cout << "Water tariff plan for address [" << address << "] updated to " << getPlanDisplayName() << "." << endl;
    }

    // --- Virtual functions from Services base class ---
    void supply() override {
        cout << "Managing water supply (" << getPlanDisplayName() << ") "
             << "for address: [" << address << "]." << endl;

        if (currentPlan == WaterTariffPlan::NO_SUPPLY) {
            cout << "   Status: No water supply active." << endl;
            return;
        }
        cout << "   Status: Water is being supplied." << endl;
    }

    void showStatus() override {
        cout << "--- Water Supply Status for Address: [" << address << "] ---" << endl;
        const WaterPlanInfo& planInfo = getCurrentPlanInfo();

        cout << "   Current Plan: " << planInfo.displayName << endl;
        if (currentPlan != WaterTariffPlan::NO_SUPPLY) {
            cout << "   Consumption This Period: " << consumptionCubicMeters << " m^3." << endl;
            cout << "   Tariff Tiers:" << endl;
            double cumulativeLimit = 0;
            for(const auto& tier : planInfo.tiers) {
                cout << "     - Up to " << tier.limitCubicMeters << " m^3 (from " << cumulativeLimit << "): $"
                     << tier.pricePerCubicMeter << "/m^3." << endl;
                cumulativeLimit = tier.limitCubicMeters;
            }
            cout << "   Estimated Bill: $" << calculateBill() << "." << endl;
        } else {
            cout << "   Status: No active water supply." << endl;
        }
        cout << "----------------------------------------------------" << endl;
    }

    virtual ~WaterManagement() override = default;
};

// Initialize static plan details map with plan information and tiered pricing
map<WaterTariffPlan, WaterPlanInfo> WaterManagement::planDetailsStore = {
    {WaterTariffPlan::NO_SUPPLY, {
        "No Supply",
        {}  // No tiers for no supply
    }},
    {WaterTariffPlan::RESIDENTIAL_CONSERVATION, {
        "Residential Conservation",
        {
            {5.0, 0.80},    // First 5 m³ at $0.80/m³
            {10.0, 1.00},   // Next 5 m³ (up to 10 total) at $1.00/m³
            {20.0, 1.30},   // Next 10 m³ (up to 20 total) at $1.30/m³
            {numeric_limits<double>::infinity(), 2.00}  // All usage above 20 m³ at $2.00/m³
        }
    }},
    {WaterTariffPlan::RESIDENTIAL_STANDARD, {
        "Residential Standard",
        {
            {10.0, 1.00},   // First 10 m³ at $1.00/m³
            {25.0, 1.20},   // Next 15 m³ (up to 25 total) at $1.20/m³
            {numeric_limits<double>::infinity(), 1.50}  // All usage above 25 m³ at $1.50/m³
        }
    }},
    {WaterTariffPlan::COMMERCIAL_STANDARD, {
        "Commercial Standard",
        {
            {50.0, 1.50},   // First 50 m³ at $1.50/m³
            {100.0, 1.80},  // Next 50 m³ (up to 100 total) at $1.80/m³
            {numeric_limits<double>::infinity(), 2.20}  // All usage above 100 m³ at $2.20/m³
        }
    }}
};

#endif // WATERMANAGEMENT_H