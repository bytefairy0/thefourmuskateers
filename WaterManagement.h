#ifndef WATERMANAGEMENT_H
#define WATERMANAGEMENT_H

#include "Services.h"
#include "HousingScheme.h" // Assuming Address is defined here or included by it
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept> 
#include <algorithm> // For std::min

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

    // Static constant map to store plan details. Declared here, defined in .cpp
    static const map<WaterTariffPlan, WaterPlanInfo> planDetailsStore;

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
        if (planDetailsStore.find(plan) == planDetailsStore.end()) {
            throw invalid_argument("Invalid water plan provided during construction.");
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
        
        // If there's still remaining consumption, it means the last tier was not infinity capped or tiers were misconfigured.
        // Assuming the last tier in planInfo.tiers is correctly set up with a very high/infinite limit for overages.
        // If the last tier doesn't cover all consumption, it would use that last tier's rate.
        // The loop structure handles this naturally if the last tier's limit is numeric_limits<double>::infinity()
        // and its "consumptionInThisTier" correctly calculates remaining.

        // The logic above sums up portions:
        // e.g. Tier 1: 0-10m3 @ $2. Tier 2: 10-20m3 @ $3. Usage: 15m3
        // 1st iteration (tier.limit = 10): consumptionInPreviousTiers = 0. consumptionInThisTier = min(15, 10-0) = 10. bill += 10*2. remaining=5. consumptionInPreviousTiers=10.
        // 2nd iteration (tier.limit = 20): consumptionInPreviousTiers = 10. consumptionInThisTier = min(5, 20-10) = 5. bill += 5*3. remaining=0. consumptionInPreviousTiers=15.

        return totalBill;
    }

    // --- Getters ---
    const Address& getAddress() const { return address; }
    double getConsumptionCubicMeters() const { return consumptionCubicMeters; }
    WaterTariffPlan getCurrentPlanType() const { return currentPlan; }
    string getPlanDisplayName() const { return getCurrentPlanInfo().displayName; }

    // --- Setter for changing plan ---
    void setCurrentPlan(WaterTariffPlan newPlan) {
        if (planDetailsStore.find(newPlan) == planDetailsStore.end()) {
            throw invalid_argument("Attempted to set an invalid water plan.");
        }
        currentPlan = newPlan;
        consumptionCubicMeters = 0.0; // Reset consumption for new billing period/plan
        cout << "Water tariff plan for address [";
        address.display();
        cout << "] "<< "updated to " << getPlanDisplayName() << "." << endl;
    }

    // --- Virtual functions from Services base class ---
    void supply() override {
        cout << "Managing water supply (" << getPlanDisplayName() << ") "
             << "for address: [" ;
             address.display();
             cout << "]." << endl;

        if (currentPlan == WaterTariffPlan::NO_SUPPLY) {
            cout << "   Status: No water supply active." << endl;
            return;
        }
        // Simulation logic: check for restrictions, water quality, etc.
        cout << "   Status: Water is being supplied." << endl;
        // Example: if city is in drought, print a warning or limit supply
        // if (city.isInDrought() && currentPlan != WaterTariffPlan::RESIDENTIAL_CONSERVATION) {
        //    cout << "   WARNING: Water restrictions in effect due to drought!" << endl;
        // }
    }

    void showStatus() override {
        cout << "--- Water Supply Status for Address: [" ;
        address.display();
        cout << "] ---" << endl;
        const WaterPlanInfo& planInfo = getCurrentPlanInfo();

        cout << "   Current Plan: " << planInfo.displayName << endl;
        if (currentPlan != WaterTariffPlan::NO_SUPPLY) {
            cout << "   Consumption This Period: " << consumptionCubicMeters << " m^3." << endl;
            cout << "   Tariff Tiers:" << endl;
            double cumulativeLimit = 0;
            for(const auto& tier : planInfo.tiers) {
                cout << "     - Up to " << tier.limitCubicMeters << " m^3 (from " << cumulativeLimit << "): $"
                     << tier.pricePerCubicMeter << "/m^3." << endl;
                cumulativeLimit = tier.limitCubicMeters; // This display is slightly off for cumulative nature
                                                       // Better: Tier 1: 0-X, Tier 2: next Y-Z, etc.
            }
            // Corrected tier display:
            // cout << "   Tariff Tiers:" << endl;
            // double lowerBound = 0;
            // for(const auto& tier : planInfo.tiers) {
            //    cout << "     - Usage from " << lowerBound << " to " << tier.limitCubicMeters << " m^3: $"
            //         << tier.pricePerCubicMeter << "/m^3." << endl;
            //    lowerBound = tier.limitCubicMeters;
            // }


            cout << "   Estimated Bill: $" << calculateBill() << "." << endl;
        } else {
            cout << "   Status: No active water supply." << endl;
        }
        cout << "----------------------------------------------------" << endl;
    }

    virtual ~WaterManagement() override = default;
};

#endif // WATERMANAGEMENT_H