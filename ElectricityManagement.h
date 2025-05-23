#ifndef ElectricityManagement_H
#define ElectricityManagement_H

#include "Services.h"
#include "HousingScheme.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cmath>

// Enum to defing the electricity supply setup for address
enum class ElectricitySupplyStrategy{
    GRID_ONLY, // all consumption from grid
    SOLAR_PRIMARY,  // uses own solar
    WIND_PRIMARY, // uses own wind
    GRID_TIED_SOLAR, // connected to grid for import/export
    GRID_TIED_WIND // connected to grid for import/export
};

class ElectricityManagement: public Services{
    private: // Encapsulation
        Address address;
        double totalConsumption; // in kWh
        double localGeneration; // Energy generated by local sources
        ElectricitySupplyStrategy supplyStrategy;
        // static member -> define outside class in .cpp file
        static double gridUnitPrice; // Price of bill per unit
        static double gridFeedInTariff; // price paid by utility for supplied to grid

        //Helper function to convert enum to string
        string strategyToString() const {
            switch (supplyStrategy) {
                case ElectricitySupplyStrategy::GRID_ONLY: return "Grid Only";
                case ElectricitySupplyStrategy::SOLAR_PRIMARY: return "Solar Primary (Self-Gen)";
                case ElectricitySupplyStrategy::WIND_PRIMARY: return "Wind Primary (Self-Gen)";
                case ElectricitySupplyStrategy::GRID_TIED_SOLAR: return "Grid-Tied Solar";
                case ElectricitySupplyStrategy::GRID_TIED_WIND: return "Grid-Tied Wind";
                default: return "Unknown";
            }
        }

    public:
        ElectricityManagement(Address ad, ElectricitySupplyStrategy strategy,
        double initialConsumption = 0.0, double initialGeneration = 0.0)
        : Services("Electricity"), address(ad), supplyStrategy(strategy),
        totalConsumption(initialConsumption), localGeneration(initialGeneration) {
        if (initialConsumption < 0 || initialGeneration < 0) {
            throw invalid_argument("Initial consumption/generation cannot be negative");
        }
    }

    // Record energy usage
    void addUsage(double kWh){
        if (kWh < 0){
            throw invalid_argument("Usage cannot be negative");
            return;
        }
        totalConsumption += kWh;
    }

    // Record energy Generated locally
    void addLocalGeneration(double kWh){
        if (kWh < 0){
            throw invalid_argument("Local generation cannot be negative");
            return;
        }
        localGeneration += kWh; // -> can be modify to handly only record if strategy supports it
    }

    /*Calculate Net energy exchange with grid over period
    Positive value means net energy DRAWN from gird
    Negative value means net energy supplied To grid*/
    double getNetGridEnergy() const{
        if (supplyStrategy == ElectricitySupplyStrategy::GRID_ONLY){
            return totalConsumption; // all energy from grid
        } else if (supplyStrategy == ElectricitySupplyStrategy::SOLAR_PRIMARY || 
                   supplyStrategy == ElectricitySupplyStrategy::WIND_PRIMARY) {
            return 0.0; // all energy from local generation
        } else if (supplyStrategy == ElectricitySupplyStrategy::GRID_TIED_SOLAR || 
                   supplyStrategy == ElectricitySupplyStrategy::GRID_TIED_WIND) {
            return totalConsumption - localGeneration; // net energy exchange with grid
        } else {
            throw invalid_argument("Invalid electricity supply strategy");
        }
    }

    // Calculate electricity bill based on Grid interaction
    double calculateBill() const{
        double netEnergy = getNetGridEnergy();
        if (netEnergy > 0) {
            return netEnergy * gridUnitPrice; // Bill for energy drawn from grid
        } else if (netEnergy < 0){
            return netEnergy * gridFeedInTariff; // Credit for energy supplied to grid
        }else{
            return 0.0;
        }
    }

    // Getters
    const Address& getAddress() const{return address;}
    double getTotalConsumption() const{return totalConsumption;}
    double getLocalGeneration() const{return localGeneration;}
    ElectricitySupplyStrategy getSupplyStrategy() const {return supplyStrategy;}
    static double getGridUnitPrice(){return gridUnitPrice;}
    static double getGridFeedInTariff(){return gridFeedInTariff;}

    // Setters
    void setSupplyStrategy(ElectricitySupplyStrategy newStrategy){
        supplyStrategy = newStrategy;
        cout << "Electricity supply strategy updated to: " << strategyToString() << endl;
    } // -> can be improved

    static void SetGridUnitPrice(double newPrice){
        if (newPrice < 0){
            throw invalid_argument("Unit price cannot be negative");
        }
        gridUnitPrice = newPrice;
    }

    static void SetGridFeedInTariff(double newTariff){
        if (newTariff < 0){
            throw invalid_argument("Feed-in tariff cannot be negative");
        }
        gridFeedInTariff = newTariff;
    }

    // Overridden Virtual Functions
    void supply() override{
        cout << "Managing electricity (" << strategyToString() << ") "
                << "for address: [";
                address.display();
                cout << "]" << endl;

        if (supplyStrategy == ElectricitySupplyStrategy::SOLAR_PRIMARY || supplyStrategy == ElectricitySupplyStrategy::WIND_PRIMARY) {
            if (totalConsumption > localGeneration) {
                cout << "   Status: Potential energy shortfall (Consumption > Generation)." << std::endl;
                 // Simulation could trigger events here (e.g., use battery, partial blackout)
            } else {
                cout << "   Status: Local generation meeting/exceeding consumption." << std::endl;
            }
        } else { // Grid-connected strategies
             double netGridEnergy = getNetGridEnergy();
             if (netGridEnergy > 0) {
                cout << "   Status: Net drawing power from grid (" << netGridEnergy << " kWh)." << std::endl;
                 // Check if grid is available in the simulation?
             } else {
                cout << "   Status: Net supplying power to grid (" << -netGridEnergy << " kWh) or balanced." << std::endl;
             }
        }
        
    }

    void showStatus() override {
        cout << "--- Electricity Status for Address: [";
        address.display();
        cout<< "] ---" << endl;
        cout << "   Supply Strategy: " << strategyToString() << std::endl;
        cout << "   Total Consumption This Period: " << totalConsumption << " kWh" << std::endl;
        if (supplyStrategy != ElectricitySupplyStrategy::GRID_ONLY) {
            cout << "   Local Generation This Period: " << localGeneration << " kWh" << std::endl;
        }
        double netGridEnergy = getNetGridEnergy();
        cout << "   Net Grid Interaction: " << netGridEnergy << " kWh "
               << (netGridEnergy > 0 ? "(Draw)" : (netGridEnergy < 0 ? "(Export)" : "(Zero)")) << std::endl;

        if (supplyStrategy == ElectricitySupplyStrategy::SOLAR_PRIMARY || supplyStrategy == ElectricitySupplyStrategy::WIND_PRIMARY) {
             std::cout << "   Billing: N/A (Primary Self-Generation)" << std::endl;
        } else {
             std::cout << "   Grid Unit Price (Draw): Rs." << gridUnitPrice << "/kWh" << std::endl;
             std::cout << "   Grid Feed-in Tariff (Export): Rs." << gridFeedInTariff << "/kWh" << std::endl;
             std::cout << "   Estimated Grid Bill/Credit: Rs." << calculateBill() << std::endl;
        }
        cout << "-------------------------------------------" << std::endl;
    }

    virtual ~ElectricityManagement() override = default;
        
};

#endif