#ifndef CITIZEN_H
#define CITIZEN_H
#include <string>
#include <iostream>
#include "building.h"
#include "transport.h"
using namespace std;

class Citizen {
private:
    string name;
    int age;
    double ecoAwareness;
    string occupation;
    double dailyTravelDistance;
    int ecoFriendlyDays;
    bool hasGreenBadge;
    double totalDistanceTraveled;
    Building* building;
    Transport* transport;

public:
    Citizen(string n,int a,double e,string job="Unemployed",double distance=10.0)
        :name(n),age(a),ecoAwareness(e),occupation(job),dailyTravelDistance(distance),
         ecoFriendlyDays(0),hasGreenBadge(false),totalDistanceTraveled(0.0),building(nullptr),transport(nullptr) {}

    void assignBuilding(Building* b) { building=b; }
    void chooseTransport(Transport* t) { transport=t; }

    void simulateDay() {
        if(building) building->simulateDay();
        double emissions=0;
        if(transport) emissions=transport->calculateCarbonEmissions()*dailyTravelDistance;
        double adjustedImpact=emissions*(1-ecoAwareness);
        if(adjustedImpact<5.0) ecoFriendlyDays++;
        if(ecoAwareness<1.0) ecoAwareness+=0.01;
        totalDistanceTraveled+=dailyTravelDistance;
        if(ecoFriendlyDays>=7 && !hasGreenBadge) hasGreenBadge=true;
    }

    double calculateEcoScore() const {
        double buildingScore=building?building->getEcoScore():0;
        double transportScore=transport?transport->calculateCarbonEmissions()*dailyTravelDistance:0;
        return buildingScore+transportScore*(1-ecoAwareness);
    }

    void talkTo(Citizen& other) const {
        if(ecoAwareness>other.ecoAwareness) {
            double influenceAmount=(ecoAwareness-other.ecoAwareness)*0.1;
            other.ecoAwareness+=influenceAmount;
            if(other.ecoAwareness>1.0) other.ecoAwareness=1.0;
        }
    }

    void displayInfo() const {
        cout<<"Name:"<<name<<"\nAge:"<<age<<"\nOccupation:"<<occupation<<"\nEco-Awareness:"<<ecoAwareness<<"\nTravel per day:"<<dailyTravelDistance<<" km\n";
        if(building) cout<<"Building:"<<building->getName()<<"\n";
        if(transport) cout<<"Transport:"<<transport->getType()<<"\n";
        cout<<"Eco Score:"<<calculateEcoScore()<<"\nGreen days:"<<ecoFriendlyDays<<"\n";
        if(hasGreenBadge) cout<<"🏅 This citizen has earned a Green Badge!\n";
        cout<<"Total distance traveled:"<<totalDistanceTraveled<<" km\n";
    }

    double getTotalDistanceTraveled() const { return totalDistanceTraveled; }
};

#endif  // CITIZEN_H