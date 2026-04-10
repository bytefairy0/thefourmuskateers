#ifndef CITIZENS_H
#define CITIZENS_H

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "buildings.h"
#include "transport.h"

using namespace std;

class Citizen {
private:
    string name;
    int age;
    double ecoAwareness;         // 0.0 to 1.0 scale
    string occupation;
    double dailyTravelDistance;  // in km
    int ecoFriendlyDays;
    bool hasGreenBadge;
    double totalDistanceTraveled;
    Building* building;
    Transport* transport;

public:
    Citizen(string n, int a, double e, string job = "Unemployed", double distance = 10.0)
        : name(n), age(a), ecoAwareness(e), occupation(job), dailyTravelDistance(distance),
         ecoFriendlyDays(0), hasGreenBadge(false), totalDistanceTraveled(0.0), building(nullptr), transport(nullptr) {
        if (e < 0.0 || e > 1.0) {
            throw invalid_argument("Eco-awareness must be between 0.0 and 1.0");
        }
    }

    void assignBuilding(Building* b) { building = b; }
    void chooseTransport(Transport* t) { transport = t; }

    void simulateDay() {
        if (building) building->updateEcoScore();
        
        double emissions = 0;
        if (transport) {
            transport->calculateCarbonEmissions();
            emissions = transport->getCarbonEmissions() * dailyTravelDistance;
        }
        
        // Adjust the environmental impact based on eco-awareness
        double adjustedImpact = emissions * (1 - ecoAwareness);
        
        // Count eco-friendly days if emissions are low
        if (adjustedImpact < 5.0) ecoFriendlyDays++;
        
        // Eco-awareness increases slightly over time (learning)
        if (ecoAwareness < 1.0) ecoAwareness += 0.01;
        
        // Track total distance traveled
        totalDistanceTraveled += dailyTravelDistance;
        
        // Award green badge after a week of eco-friendly behavior
        if (ecoFriendlyDays >= 7 && !hasGreenBadge) hasGreenBadge = true;
    }

    double calculateEcoScore() const {
        double buildingScore = building ? building->getEcoScoreImpact() : 0;
        double transportScore = transport ? transport->getCarbonEmissions() * dailyTravelDistance : 0;
        
        // Eco-awareness reduces the negative impact
        return buildingScore + transportScore * (1 - ecoAwareness);
    }

    // Social influence mechanic
    void talkTo(Citizen& other) const {
        if (ecoAwareness > other.ecoAwareness) {
            // The more eco-aware citizen influences the less eco-aware one
            double influenceAmount = (ecoAwareness - other.ecoAwareness) * 0.1;
            other.ecoAwareness += influenceAmount;
            
            // Cap at 1.0
            if (other.ecoAwareness > 1.0) other.ecoAwareness = 1.0;
        }
    }

    void displayInfo() const {
        cout << "Name: " << name << "\n";
        cout << "Age: " << age << "\n";
        cout << "Occupation: " << occupation << "\n";
        cout << "Eco-Awareness: " << ecoAwareness << "\n";
        cout << "Travel per day: " << dailyTravelDistance << " km\n";
        
        if (building) cout << "Building: " << building->getName() << "\n";
        if (transport) cout << "Transport: " << transport->getType() << "\n";
        
        cout << "Eco Score: " << calculateEcoScore() << "\n";
        cout << "Green days: " << ecoFriendlyDays << "\n";
        
        if (hasGreenBadge) cout << "🏅 This citizen has earned a Green Badge!\n";
        
        cout << "Total distance traveled: " << totalDistanceTraveled << " km\n";
    }

    // Getters
    double getTotalDistanceTraveled() const { return totalDistanceTraveled; }
    string getName() const { return name; }
    double getEcoAwareness() const { return ecoAwareness; }
};

#endif  // CITIZENS_H