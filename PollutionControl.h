#ifndef POLLUTIONCONTROL_H
#define POLLUTIONCONTROL_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include <iomanip>

#include "buildings.h"
#include "transport.h"
#include "Citizens.h"
#include "Services.h"
#include "HousingScheme.h"

using namespace std;

class Building;
class Transport;
class Citizen;
class Services;
class HousingScheme;

// PollutionControl class that monitors and regulates pollution levels in the city
//This is a friend class to all major components
 
class PollutionControl {
private:
    // Pollution levels
    double airPollutionLevel;
    double waterPollutionLevel;
    double noisePollutionLevel;
    double solidWasteLevel;
    
    // Threshold - limit for each pollution level
    const double AIR_POLLUTION_THRESHOLD = 50.0;
    const double WATER_POLLUTION_THRESHOLD = 30.0;
    const double NOISE_POLLUTION_THRESHOLD = 60.0;
    const double SOLID_WASTE_THRESHOLD = 40.0;
    
    // creating a file to entere the date and time for each entry 
    ofstream logFile;
    
    // Private helper to log events
    void logEvent(const string& event) {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        
        if (logFile.is_open()) {
            logFile << "[" << buf << "] " << event << endl;
        }
    }
    
public:
    // Constructor
    PollutionControl()  : airPollutionLevel(0.0), waterPollutionLevel(0.0), noisePollutionLevel(0.0), solidWasteLevel(0.0) {
        // Open log file
        logFile.open("pollution_control.log", ios::app);
        if (!logFile.is_open()) {
            cout << "Warning: cant open pollution control log file!!!" << endl;
        }
        logEvent("Pollution Control System initialized");
    }
    
    // Destructor to close log file
    ~PollutionControl() {
        if (logFile.is_open()) {
            logEvent("Pollution Control System shutdown!!");
            logFile.close();
        }
    }
    
    // Monitor Building pollution
    void monitorBuilding(const Building* building) {
        if (!building) return;

        // Increment air pollution based on building's eco score impact
        double impact = building->getEcoScoreImpact();
        airPollutionLevel += (impact > 0) ? impact * 0.1 : 0;
        
        logEvent("Monitored building: " + string(building->getName()) + ", Impact: " + to_string(impact));
        
        // Alert if threshold exceeded
        checkThresholds();
    }
    
    // Monitor Transport pollution
    void monitorTransport(const Transport* transport) {
        if (!transport) return;

        // Increment air pollution based on vehicle's carbon emissions
        double emissions = transport->getCarbonEmissions();
        airPollutionLevel += emissions * 0.05;
        noisePollutionLevel += (emissions > 10) ? emissions * 0.02 : 0;
        
        logEvent("Monitored transport: " + string(transport->getType()) +  ", Emissions: " + to_string(emissions));
        
        // Alert if threshold exceeded
        checkThresholds();
    }
    
    // Monitor Citizen activities
    void monitorCitizen(const Citizen* citizen) {
        if (!citizen) return;

        // Using the eco score to measure citizen impact
        double ecoScore = citizen->calculateEcoScore();
        double distance = citizen->getTotalDistanceTraveled();
        
        // Adjust pollution levels based on citizen activity
        airPollutionLevel += (distance > 20) ? distance * 0.01 : 0;
        
        logEvent("Monitored citizen with eco score: " + to_string(ecoScore) + ", Distance traveled: " + to_string(distance));
        
        // Alert if threshold exceeded
        checkThresholds();
    }
    
    // Monitor Services (Water, Electricity, etc.)
    void monitorService(const Services* service) {
        if (!service) return;

        // Different services affect different pollution types
        string serviceType = service->getServiceType();
        double reading = service->getAverageReading();

        if (serviceType == "Water") {
            waterPollutionLevel += reading * 0.02;
        } 
        else if (serviceType == "Electricity") {
            airPollutionLevel += reading * 0.03;
        }
        else if (serviceType == "Gas") {
            airPollutionLevel += reading * 0.04;
        }
        
        logEvent("Monitored service: " + serviceType + ", Reading: " + to_string(reading));
        
        // Alert if threshold exceeded
        checkThresholds();
    }
    
    // Monitor Housing Scheme
    void monitorHousingScheme(const HousingScheme* housing) {
        if (!housing) return;

        // Housing affects multiple pollution types
        airPollutionLevel += housing->getAveragePollution() * 0.02;
        waterPollutionLevel += housing->getAveragePollution() * 0.01;
        solidWasteLevel += housing->getOccupiedUnits() * 0.1;
        
        logEvent("Monitored housing scheme: " + housing->getSchemeName() + ", Average pollution: " + to_string(housing->getAveragePollution()));
        
        // Alert if threshold exceeded
        checkThresholds();
    }
    
    // Check all thresholds and issue warnings
    void checkThresholds() {
        if (airPollutionLevel > AIR_POLLUTION_THRESHOLD) {
            cout << "WARNING: Air pollution level exceeded threshold!" << endl;
            logEvent("WARNING: Air pollution level exceeded threshold: " + to_string(airPollutionLevel));
        }
        
        if (waterPollutionLevel > WATER_POLLUTION_THRESHOLD) {
            cout << "WARNING: Water pollution level exceeded threshold!" << endl;
            logEvent("WARNING: Water pollution level exceeded threshold: " + to_string(waterPollutionLevel));
        }
        
        if (noisePollutionLevel > NOISE_POLLUTION_THRESHOLD) {
            cout << "WARNING: Noise pollution level exceeded threshold!" << endl;
            logEvent("WARNING: Noise pollution level exceeded threshold: " + to_string(noisePollutionLevel));
        }
        
        if (solidWasteLevel > SOLID_WASTE_THRESHOLD) {
            cout << "WARNING: Solid waste level exceeded threshold!" << endl;
            logEvent("WARNING: Solid waste level exceeded threshold: " + to_string(solidWasteLevel));
        }
    }
    
    // Generate pollution report
    void generateReport() const {
        cout << "\n----- Pollution Control Report -----" << endl;
        cout << "Air Pollution Level: " << airPollutionLevel << " / 100" << endl;
        cout << "Water Pollution Level: " << waterPollutionLevel << " / 100" << endl;
        cout << "Noise Pollution Level: " << noisePollutionLevel << " / 100" << endl;
        cout << "Solid Waste Level: " << solidWasteLevel << " / 100" << endl;
        
        // Overall status
        double avgPollution = (airPollutionLevel+waterPollutionLevel+noisePollutionLevel+solidWasteLevel)/4.0;
        
        cout << "Overall Pollution Status: ";
        if (avgPollution < 20) {
            cout << "Excellent - Very low pollution levels!" << endl;
        } else if (avgPollution < 40) {
            cout << "Good - Moderate pollution levels!" << endl;
        } else if (avgPollution < 60) {
            cout << "Warning - High pollution levels!" << endl;
        } else if (avgPollution < 80) {
            cout << "Danger - Very high pollution levels!" << endl;
        } else {
            cout << "Critical - Extreme pollution levels!" << endl;
        }
        cout << "----------------------------------" << endl;
    }
    
    // Implement pollution reduction measures
    void implementReductionMeasures() {
        double reductionFactor = 0.1; // Reduce by 10%
        
        if (airPollutionLevel > AIR_POLLUTION_THRESHOLD) {
            airPollutionLevel *= (1 - reductionFactor);
            cout << "Implementing air pollution reduction measures..." << endl;
            logEvent("Air pollution reduction measures implemented. New level: " + to_string(airPollutionLevel));
        }
        
        if (waterPollutionLevel > WATER_POLLUTION_THRESHOLD) {
            waterPollutionLevel *= (1 - reductionFactor);
            cout << "Implementing water pollution reduction measures..." << endl;
            logEvent("Water pollution reduction measures implemented. New level: " + to_string(waterPollutionLevel));
        }
        
        if (noisePollutionLevel > NOISE_POLLUTION_THRESHOLD) {
            noisePollutionLevel *= (1 - reductionFactor);
            cout << "Implementing noise reduction measures..." << endl;
            logEvent("Noise pollution reduction measures implemented. New level: " + to_string(noisePollutionLevel));
        }
        
        if (solidWasteLevel > SOLID_WASTE_THRESHOLD) {
            solidWasteLevel *= (1 - reductionFactor);
            cout << "Implementing waste management measures..." << endl;
            logEvent("Waste management measures implemented. New level: " + to_string(solidWasteLevel));
        }
    }
    
    // Getters for pollution levels
    double getAirPollutionLevel() const { return airPollutionLevel; }
    double getWaterPollutionLevel() const { return waterPollutionLevel; }
    double getNoisePollutionLevel() const { return noisePollutionLevel; }
    double getSolidWasteLevel() const { return solidWasteLevel; }
};

#endif // POLLUTIONCONTROL_H