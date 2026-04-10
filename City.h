#ifndef CITY_H
#define CITY_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <random>
#include <algorithm>
#include <numeric>

#include "buildings.h"
#include "transport.h"
#include "Citizens.h"
#include "HousingScheme.h"
#include "Services.h"
#include "PollutionControl.h"
#include "CityLogger.h"

using namespace std;

// Forward declaration
class Building;
class Transport;
class Citizen;
class HousingScheme;
class Services;
class PollutionControl;

 //City class that manages all aspects of the eco city

class City {
private:
    string name;
    string mayor;
    double budget;
    double ecoScore;
    int day;
    vector<unique_ptr<Building>> buildings;
    vector<unique_ptr<Transport>> vehicles;
    vector<unique_ptr<Citizen>> citizens;
    vector<unique_ptr<HousingScheme>> housingSchemes;
    vector<unique_ptr<Services>> services;
    unique_ptr<PollutionControl> pollutionControl;
    unique_ptr<CityLogger<string>> logger;
    
    // Random number generator
    mt19937 rng;

public:
    // Constructor
    City(const string& cityName, const string& mayorName, double initialBudget): name(cityName), mayor(mayorName), budget(initialBudget),ecoScore(100.0), day(0), rng(random_device{}()) {
        
        // Initialize pollution control
        pollutionControl = make_unique<PollutionControl>();
        
        // Initialize logger
        logger = make_unique<CityLogger<string>>(cityName, cityName + "_log.txt");
        
        // Log initialization
        logger->log("City " + cityName + " established with Mayor " + mayorName);
    }
    
    // Add a building to the city
    void addBuilding(unique_ptr<Building> building) {
        if (building == nullptr) {
            throw invalid_argument("Cannot add a null building");
        }
        
        string buildingName = building->getName();
        buildings.push_back(move(building));
        
        // Log the addition
        logger->log("Added new building: " + buildingName);
    }
    
    // Add a vehicle to the city
    void addTransport(unique_ptr<Transport> vehicle) {
        if (vehicle == nullptr) {
            throw invalid_argument("Cannot add a null transport");
        }
        
        string vehicleType = vehicle->getType();
        vehicles.push_back(move(vehicle));
        
        // Log the addition
        logger->log("Added new transport: " + vehicleType);
    }
    
    // Add a citizen to the city
    void addCitizen(unique_ptr<Citizen> citizen) {
        if (citizen == nullptr) {
            throw invalid_argument("Cannot add a null citizen");
        }
        
        string citizenName = citizen->getName();
        citizens.push_back(move(citizen));
        
        // Log the addition
        logger->log("New citizen joined the city. Total population: " + to_string(citizens.size()));
    }
    
    // Add a housing scheme to the city
    void addHousingScheme(unique_ptr<HousingScheme> housing) {
        if (housing == nullptr) {
            throw invalid_argument("Cannot add a null housing scheme");
        }
        
        string schemeName = housing->getSchemeName();
        housingSchemes.push_back(move(housing));
        
        // Log the addition
        logger->log("Added new housing scheme: " + schemeName);
    }
    
    // Add a service to the city
    void addService(unique_ptr<Services> service) {
        if (service == nullptr) {
            throw invalid_argument("Cannot add a null service");
        }
        
        string serviceType = service->getServiceType();
        services.push_back(move(service));
        
        // Log the addition
        logger->log("Added new service: " + serviceType);
    }
    
    // Simulate a single day in the city
    void simulateDay() {
        day++;
        logger->log("Starting simulation for day " + to_string(day));
        
        // Calculate eco score before day activities
        double previousEcoScore = ecoScore;
        
        // Simulate citizens
        for (auto& citizen : citizens) {
            citizen->simulateDay();
            pollutionControl->monitorCitizen(citizen.get());
        }
        
        // Simulate buildings
        for (auto& building : buildings) {
            building->updateEcoScore();
            pollutionControl->monitorBuilding(building.get());
        }
        
        // Simulate vehicles
        for (auto& vehicle : vehicles) {
            pollutionControl->monitorTransport(vehicle.get());
        }
        
        // Simulate services
        for (auto& service : services) {
            pollutionControl->monitorService(service.get());
        }
        
        // Simulate housing
        for (auto& housing : housingSchemes) {
            pollutionControl->monitorHousingScheme(housing.get());
        }
        
        // Update eco score based on all components
        updateEcoScore();
        
        // Apply budget changes
        double dailyCost = calculateDailyOperationCost();
        budget -= dailyCost;
        
        // Log end of day
        logger->log("Day " + to_string(day) + " completed. Eco Score: " + to_string(ecoScore));
    }
    
    // Simulate multiple days
    void simulateDays(int numDays) {
        if (numDays <= 0) {
            throw invalid_argument("Number of days must be positive");
        }
        
        for (int i = 0; i < numDays; i++) {
            simulateDay();
        }
    }
    
    // Update the city's eco score
    void updateEcoScore() {
        // Base score starting point
        double newScore = 100.0;
        
        // Buildings impact
        double buildingImpact = 0.0;
        for (const auto& building : buildings) {
            buildingImpact += building->getEcoScoreImpact();
        }
        // Normalize building impact
        if (!buildings.empty()) {
            buildingImpact /= buildings.size();
        }
        
        // Citizens impact
        double citizenImpact = 0.0;
        for (const auto& citizen : citizens) {
            // Convert 0-100 scale to impact (-50 to +50)
            double impact = (citizen->calculateEcoScore() - 50.0) / -1.0;
            citizenImpact += impact;
        }
        // Normalize citizen impact
        if (!citizens.empty()) {
            citizenImpact /= citizens.size();
        }
        
        // Transport impact
        double transportImpact = 0.0;
        for (const auto& vehicle : vehicles) {
            transportImpact += vehicle->getCarbonEmissions() * 0.1;
        }
        // Normalize transport impact
        if (!vehicles.empty()) {
            transportImpact /= vehicles.size();
        }
        
        // Housing impact
        double housingImpact = 0.0;
        for (const auto& housing : housingSchemes) {
            housingImpact += (100.0 - housing->getSustainabilityRating()) * 0.5;
        }
        // Normalize housing impact
        if (!housingSchemes.empty()) {
            housingImpact /= housingSchemes.size();
        }
        
        // Services impact (lower reliability means higher impact)
        double servicesImpact = 0.0;
        for (const auto& service : services) {
            servicesImpact += (100.0 - service->getReliabilityScore()) * 0.2;
        }
        // Normalize services impact
        if (!services.empty()) {
            servicesImpact /= services.size();
        }
        
        // Pollution impact
        double pollutionImpact = pollutionControl->getAirPollutionLevel() * 0.3 +
                                pollutionControl->getWaterPollutionLevel() * 0.3 +
                                pollutionControl->getNoisePollutionLevel() * 0.2 +
                                pollutionControl->getSolidWasteLevel() * 0.2;
        
        // Calculate final score
        newScore -= (buildingImpact + citizenImpact + transportImpact + housingImpact + servicesImpact + pollutionImpact);
        
        // Ensure score is between 0 and 100
        ecoScore = max(0.0, min(100.0, newScore));
        
        // Log the updated score
        logger->log("Updated eco score: " + to_string(ecoScore));
    }
    
    // Calculate daily operation cost
    double calculateDailyOperationCost() const {
        // Base city operation cost
        double cost = 100.0;
        
        // Buildings maintenance cost
        cost += buildings.size() * 50.0;
        
        // Services operation cost
        for (const auto& service : services) {
            // Different services have different costs
            string type = service->getServiceType();
            if (type == "Water") {
                cost += 75.0;
            } else if (type == "Electricity") {
                cost += 100.0;
            } else if (type == "Gas") {
                cost += 80.0;
            } else if (type == "Internet") {
                cost += 50.0;
            }
        }
        
        // Transport maintenance cost
        cost += vehicles.size() * 30.0;
        
        return cost;
    }
    
    // Display city statistics
    void displayStatistics() const {
        cout << "\n===== CITY STATISTICS: " << name << " =====" << endl;
        cout << "Mayor: " << mayor << endl;
        cout << "Day: " << day << endl;
        cout << "Budget: $" << budget << endl;
        cout << "Eco Score: " << ecoScore << "/100" << endl;
        cout << "Population: " << citizens.size() << endl;
        cout << "Buildings: " << buildings.size() << endl;
        cout << "Housing Schemes: " << housingSchemes.size() << endl;
        cout << "Vehicles: " << vehicles.size() << endl;
        cout << "Services: " << services.size() << endl;
        
        // Show pollution levels
        pollutionControl->generateReport();
    }
    
    // Generate detailed report about city status
    void generateDetailedReport() const {
        cout << "\n================ DETAILED CITY REPORT =================" << endl;
        cout << "City: " << name << " | Mayor: " << mayor << " | Day: " << day << endl;
        cout << "Budget: $" << budget << " | Eco Score: " << ecoScore << "/100" << endl;
        
        cout << "\n--- POPULATION (" << citizens.size() << " citizens) ---" << endl;
        if (citizens.empty()) {
            cout << "No citizens in the city." << endl;
        } else {
            // Display average eco score
            double totalEcoScore = 0.0;
            for (const auto& citizen : citizens) {
                totalEcoScore += citizen->calculateEcoScore();
            }
            double avgEcoScore = totalEcoScore / citizens.size();
            cout << "Average Citizen Eco Score: " << avgEcoScore << "/100" << endl;
            
            // Show top 5 citizens if available
            cout << "Top Citizens by Eco Score:" << endl;
            vector<const Citizen*> sortedCitizens;
            for (const auto& citizen : citizens) {
                sortedCitizens.push_back(citizen.get());
            }
            
            sort(sortedCitizens.begin(), sortedCitizens.end(), 
                 [](const Citizen* a, const Citizen* b) {
                     return a->calculateEcoScore() > b->calculateEcoScore();
                 });
            
            for (size_t i = 0; i < min(size_t(5), sortedCitizens.size()); i++) {
                cout << i+1 << ". " << sortedCitizens[i]->getName() 
                     << " (" << sortedCitizens[i]->calculateEcoScore() << "/100)" << endl;
            }
        }
        
        cout << "\n--- BUILDINGS (" << buildings.size() << " buildings) ---" << endl;
        if (buildings.empty()) {
            cout << "No buildings in the city." << endl;
        } else {
            // Count building types
            int residential = 0, commercial = 0, green = 0, industrial = 0, recreational = 0, educational = 0;
            
            for (const auto& building : buildings) {
                if (dynamic_cast<ResidentialBuilding*>(building.get())) residential++;
                else if (dynamic_cast<CommercialBuilding*>(building.get())) commercial++;
                else if (dynamic_cast<GreenBuilding*>(building.get())) green++;
                else if (dynamic_cast<IndustrialBuilding*>(building.get())) industrial++;
                else if (dynamic_cast<RecreationalBuilding*>(building.get())) recreational++;
                else if (dynamic_cast<EducationalBuilding*>(building.get())) educational++;
            }
            
            cout << "Residential: " << residential << endl;
            cout << "Commercial: " << commercial << endl;
            cout << "Green: " << green << endl;
            cout << "Industrial: " << industrial << endl;
            cout << "Recreational: " << recreational << endl;
            cout << "Educational: " << educational << endl;
        }
        
        cout << "\n--- HOUSING (" << housingSchemes.size() << " schemes) ---" << endl;
        if (housingSchemes.empty()) {
            cout << "No housing schemes in the city." << endl;
        } else {
            // Count housing types and calculate average sustainability
            int apartments = 0, villas = 0;
            double totalSustainability = 0.0;
            int totalUnits = 0, occupiedUnits = 0;
            
            for (const auto& housing : housingSchemes) {
                if (dynamic_cast<ApartmentComplex*>(housing.get())) apartments++;
                else if (dynamic_cast<VillaComplex*>(housing.get())) villas++;
                
                totalSustainability += housing->getSustainabilityRating();
                totalUnits += housing->getTotalUnits();
                occupiedUnits += housing->getOccupiedUnits();
            }
            
            double avgSustainability = totalSustainability / housingSchemes.size();
            double occupancyRate = (totalUnits > 0) ? (double(occupiedUnits) / totalUnits * 100.0) : 0.0;
            
            cout << "Apartment Complexes: " << apartments << endl;
            cout << "Villa Complexes: " << villas << endl;
            cout << "Average Sustainability: " << avgSustainability << "/100" << endl;
            cout << "Total Units: " << totalUnits << " (" << occupiedUnits << " occupied, " 
                 << occupancyRate << "% occupancy)" << endl;
        }
        
        cout << "\n--- SERVICES (" << services.size() << " services) ---" << endl;
        if (services.empty()) {
            cout << "No services in the city." << endl;
        } else {
            for (const auto& service : services) {
                cout << "Service: " << service->getServiceType() 
                     << " (Reliability: " << service->getReliabilityScore() << "%)" << endl;
            }
        }
        
        cout << "\n--- TRANSPORT (" << vehicles.size() << " vehicles) ---" << endl;
        if (vehicles.empty()) {
            cout << "No vehicles in the city." << endl;
        } else {
            // Count vehicle types
            map<string, int> vehicleCounts;
            double totalEmissions = 0.0;
            
            for (const auto& vehicle : vehicles) {
                vehicleCounts[vehicle->getType()]++;
                totalEmissions += vehicle->getCarbonEmissions();
            }
            
            for (const auto& [type, count] : vehicleCounts) {
                cout << type << "s: " << count << endl;
            }
            
            cout << "Total Carbon Emissions: " << totalEmissions << " kg CO2" << endl;
        }
        
        cout << "\n--- POLLUTION CONTROL ---" << endl;
        pollutionControl->generateReport();
        
        cout << "\n=====================================================\n" << endl;
    }
    
    // Save statistics to a file
    bool saveStatisticsToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << " for writing" << endl;
            return false;
        }
        
        // Current timestamp
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        
        file << "===== CITY STATISTICS: " << name << " =====" << endl;
        file << "Report generated on: " << buf << endl;
        file << "Mayor: " << mayor << endl;
        file << "Day: " << day << endl;
        file << "Budget: $" << budget << endl;
        file << "Eco Score: " << ecoScore << "/100" << endl;
        file << "Population: " << citizens.size() << endl;
        file << "Buildings: " << buildings.size() << endl;
        file << "Housing Schemes: " << housingSchemes.size() << endl;
        file << "Vehicles: " << vehicles.size() << endl;
        file << "Services: " << services.size() << endl;
        
        // Add pollution data to file
        file << "Air Pollution Level: " << pollutionControl->getAirPollutionLevel() << endl;
        file << "Water Pollution Level: " << pollutionControl->getWaterPollutionLevel() << endl;
        file << "Noise Pollution Level: " << pollutionControl->getNoisePollutionLevel() << endl;
        file << "Solid Waste Level: " << pollutionControl->getSolidWasteLevel() << endl;
        
        file.close();
        return true;
    }
    
    // Implement pollution reduction measures
    void implementPollutionMeasures() {
        pollutionControl->implementReductionMeasures();
        budget -= 200.0; // Cost of pollution measures
        updateEcoScore(); // Update eco score after measures
        logger->log("Implemented pollution reduction measures");
    }
    
    // Getters
    string getName() const { return name; }
    string getMayor() const { return mayor; }
    double getBudget() const { return budget; }
    double getEcoScore() const { return ecoScore; }
    int getDay() const { return day; }
    int getPopulation() const { return citizens.size(); }
    
    // Display log entries
    void displayLogs() const {
        if (logger) {
            logger->displayAllLogs();
        }
    }
};

#endif // CITY_H
