#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <algorithm>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define YELLOW    "\033[33m"             
#define BOLD_YELLOW "\033[1;33m"          
#define ORANGE    "\033[38;5;208m"           

// Include all required headers
#include "buildings.h"
#include "transport.h"
#include "Citizens.h"
#include "Address.h"
#include "HousingScheme.h"
#include "WaterManagement.h"
#include "ElectricityManagement.h"
#include "GasManagement.h"
#include "InternetManagement.h"
#include "PollutionControl.h"
#include "City.h"
#include "CityLogger.h"
#include "Services.h"

using namespace std;

// Function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to display menu
void displayMenu() {
    cout << BOLD << GREEN << "\n-----SMART ECO CITY SIMULATION-----" << RESET << endl;
    cout << CYAN << "1. Create City" << RESET << endl;
    cout << MAGENTA << "2. Add Buildings" << RESET << endl;
    cout << CYAN << "3. Add Citizens" << RESET << endl;
    cout << MAGENTA << "4. Add Transport" << RESET << endl;
    cout << CYAN << "5. Add Housing Scheme" << RESET << endl;
    cout << MAGENTA << "6. Add Services" << RESET << endl;
    cout << CYAN << "7. Simulate Day" << RESET << endl;
    cout << MAGENTA << "8. Simulate Multiple Days" << RESET << endl;
    cout << CYAN << "9. Display City Statistics" << RESET << endl;
    cout << MAGENTA << "10. Generate Detailed Report" << RESET << endl;
    cout << CYAN<< "11. Save City Statistics to File" << RESET << endl;
    cout << MAGENTA << "12. Exit" << RESET << endl;
    cout << YELLOW << "----------------------------" << RESET << endl;
    cout << BOLD << RED << "Enter your choice: " << RESET;
}

// Function to create an address
Address createAddress() {
    int streetNo, houseNo;
    string city, pin;
    
    cout << "Enter street number: ";
    cin >> streetNo;
    cout << "Enter house number: ";
    cin >> houseNo;
    clearInputBuffer();
    cout << "Enter city: ";
    getline(cin, city);
    cout << "Enter PIN code: ";
    getline(cin, pin);
    
    return Address(streetNo, houseNo, city, pin);
}

int main() {
    // Smart pointer to our city
    unique_ptr<City> ecoCity = nullptr;
    
    int choice;
    bool running = true;

    cout << BOLD << ORANGE;
    cout << "============================================================" << endl;
    cout << "           WELCOME TO THE SMART CITY ECO SIMULATION!!!         " << endl;
    cout << "============================================================" << RESET << endl;
    
    while(running) {
        displayMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            clearInputBuffer();
            continue;
        }
        
        clearInputBuffer();
        
        switch(choice) {
            case 1: {
                if (ecoCity != nullptr) {
                    cout << "City already exists. Do you want to create a new one? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    if (confirm != 'y' && confirm != 'Y') {
                        clearInputBuffer();
                        break;
                    }
                    clearInputBuffer();
                }
                
                string cityName, mayor;
                double budget;
                
                cout << "Enter city name: ";
                getline(cin, cityName);
                cout << "Enter mayor name: ";
                getline(cin, mayor);
                cout << "Enter initial budget: $";
                cin >> budget;
                clearInputBuffer();
                
                try {
                    ecoCity = make_unique<City>(cityName, mayor, budget);
                    cout << "City " << cityName << " created successfully!" << endl;
                } catch (const exception& e) {
                    cerr << "Error creating city: " << e.what() << endl;
                }
                break;
            }
            case 2: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                int buildingType;
                string name;
                
                cout << "Select building type:" << endl;
                cout << "1. Residential Building" << endl;
                cout << "2. Commercial Building" << endl;
                cout << "3. Green Building" << endl;
                cout << "4. Industrial Building" << endl;
                cout << "5. Recreational Building" << endl;
                cout << "6. Educational Building" << endl;
                cout << "Enter choice: ";
                cin >> buildingType;
                clearInputBuffer();
                
                cout << "Enter building name: ";
                getline(cin, name);
                
                try {
                    unique_ptr<Building> building = nullptr;
                    
                    switch(buildingType) {
                        case 1: {
                            int residents;
                            cout << "Enter number of residents: ";
                            cin >> residents;
                            building = make_unique<ResidentialBuilding>(name, residents);
                            break;
                        }
                        case 2:
                            building = make_unique<CommercialBuilding>(name);
                            break;
                        case 3:
                            building = make_unique<GreenBuilding>(name);
                            break;
                        case 4:
                            building = make_unique<IndustrialBuilding>(name);
                            break;
                        case 5:
                            building = make_unique<RecreationalBuilding>(name);
                            break;
                        case 6: {
                            int students;
                            cout << "Enter number of students: ";
                            cin >> students;
                            building = make_unique<EducationalBuilding>(name, students);
                            break;
                        }
                        default:
                            cout << "Invalid building type." << endl;
                            clearInputBuffer();
                            break;
                    }
                    
                    if (building) {
                        ecoCity->addBuilding(move(building));
                        cout << "Building added successfully!" << endl;
                    }
                } catch (const exception& e) {
                    cerr << "Error adding building: " << e.what() << endl;
                }
                break;
            }
            case 3: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                string name, occupation;
                int age;
                double ecoAwareness, distance;
                
                cout << "Enter citizen name: ";
                getline(cin, name);
                cout << "Enter age: ";
                cin >> age;
                clearInputBuffer();
                cout << "Enter occupation: ";
                getline(cin, occupation);
                cout << "Enter eco-awareness level (0.0-1.0): ";
                cin >> ecoAwareness;
                cout << "Enter daily travel distance: ";
                cin >> distance;
                clearInputBuffer();
                
                try {
                    auto citizen = make_unique<Citizen>(name, age, ecoAwareness, occupation, distance);
                    ecoCity->addCitizen(move(citizen));
                    cout << "Citizen added successfully!" << endl;
                } catch (const exception& e) {
                    cerr << "Error adding citizen: " << e.what() << endl;
                }
                break;
            }
            case 4: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                vector<Transport*> cityVehicles;
                cout << "Adding vehicles to the city:" << endl;
                
                try {
                    manageCityVehicles(cityVehicles);
                    
                    for (auto vehicle : cityVehicles) {
                        ecoCity->addTransport(unique_ptr<Transport>(vehicle));
                    }
                    
                    cout << "Vehicles added to the city successfully!" << endl;
                } catch (const exception& e) {
                    cerr << "Error adding vehicles: " << e.what() << endl;
                    // Clean up in case of error
                    for (auto vehicle : cityVehicles) {
                        delete vehicle;
                    }
                }
                break;
            }
            case 5: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                int housingType;
                string name;
                int units;
                
                cout << "Select housing scheme type:" << endl;
                cout << "1. Apartment Complex" << endl;
                cout << "2. Villa Complex" << endl;
                cout << "Enter choice: ";
                cin >> housingType;
                clearInputBuffer();
                
                cout << "Enter housing scheme name: ";
                getline(cin, name);
                cout << "Enter number of units: ";
                cin >> units;
                clearInputBuffer();
                
                // Create address
                Address location = createAddress();
                
                try {
                    unique_ptr<HousingScheme> housing = nullptr;
                    
                    switch(housingType) {
                        case 1: {
                            int floors;
                            char hasElevator, hasSolar;
                            
                            cout << "Enter number of floors: ";
                            cin >> floors;
                            cout << "Has elevator? (y/n): ";
                            cin >> hasElevator;
                            cout << "Has solar panels? (y/n): ";
                            cin >> hasSolar;
                            clearInputBuffer();
                            
                            housing = make_unique<ApartmentComplex>(
                                name, location, units, floors,
                                (hasElevator == 'y' || hasElevator == 'Y'),
                                (hasSolar == 'y' || hasSolar == 'Y')
                            );
                            break;
                        }
                        case 2: {
                            double plotSize;
                            char hasPool, hasGreen;
                            
                            cout << "Enter average plot size (in sq. meters): ";
                            cin >> plotSize;
                            cout << "Has swimming pool? (y/n): ";
                            cin >> hasPool;
                            cout << "Has green space? (y/n): ";
                            cin >> hasGreen;
                            clearInputBuffer();
                            
                            housing = make_unique<VillaComplex>(
                                name, location, units, plotSize,
                                (hasPool == 'y' || hasPool == 'Y'),
                                (hasGreen == 'y' || hasGreen == 'Y')
                            );
                            break;
                        }
                        default:
                            cout << "Invalid housing type." << endl;
                            break;
                    }
                    
                    if (housing) {
                        ecoCity->addHousingScheme(move(housing));
                        cout << "Housing scheme added successfully!" << endl;
                    }
                } catch (const exception& e) {
                    cerr << "Error adding housing scheme: " << e.what() << endl;
                }
                break;
            }
            case 6: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                int serviceType;
                
                cout << "Select service type:" << endl;
                cout << "1. Water Management" << endl;
                cout << "2. Electricity Management" << endl;
                cout << "3. Gas Management" << endl;
                cout << "4. Internet Management" << endl;
                cout << "Enter choice: ";
                cin >> serviceType;
                clearInputBuffer();
                
                // Create address
                Address location = createAddress();
                
                try {
                    unique_ptr<Services> service = nullptr;
                    
                    switch(serviceType) {
                        case 1: {
                            int planType;
                            cout << "Select water plan:" << endl;
                            cout << "1. No Supply" << endl;
                            cout << "2. Residential Conservation" << endl;
                            cout << "3. Residential Standard" << endl;
                            cout << "4. Commercial Standard" << endl;
                            cout << "Enter choice: ";
                            cin >> planType;
                            
                            WaterTariffPlan plan;
                            switch(planType) {
                                case 1: plan = WaterTariffPlan::NO_SUPPLY; break;
                                case 2: plan = WaterTariffPlan::RESIDENTIAL_CONSERVATION; break;
                                case 3: plan = WaterTariffPlan::RESIDENTIAL_STANDARD; break;
                                case 4: plan = WaterTariffPlan::COMMERCIAL_STANDARD; break;
                                default: plan = WaterTariffPlan::NO_SUPPLY;
                            }
                            
                            service = make_unique<WaterManagement>(location, plan);
                            break;
                        }
                        case 2: {
                            int planType;
                            cout << "Select electricity plan:" << endl;
                            cout << "1. No Service" << endl;
                            cout << "2. Basic" << endl;
                            cout << "3. Standard" << endl;
                            cout << "4. Premium" << endl;
                            cout << "5. Renewable" << endl;
                            cout << "Enter choice: ";
                            cin >> planType;
                            
                            ElectricityPlan plan;
                            switch(planType) {
                                case 1: plan = ElectricityPlan::NO_SERVICE; break;
                                case 2: plan = ElectricityPlan::BASIC; break;
                                case 3: plan = ElectricityPlan::STANDARD; break;
                                case 4: plan = ElectricityPlan::PREMIUM; break;
                                case 5: plan = ElectricityPlan::RENEWABLE; break;
                                default: plan = ElectricityPlan::NO_SERVICE;
                            }
                            
                            service = make_unique<ElectricityManagement>(location, plan);
                            break;
                        }
                        case 3: {
                            service = make_unique<GasManagement>(location);
                            break;
                        }
                        case 4: {
                            int planType;
                            cout << "Select internet plan:" << endl;
                            cout << "1. No Service" << endl;
                            cout << "2. Basic" << endl;
                            cout << "3. Standard" << endl;
                            cout << "4. Premium" << endl;
                            cout << "5. Business Fiber" << endl;
                            cout << "Enter choice: ";
                            cin >> planType;
                            
                            InternetPlan plan;
                            switch(planType) {
                                case 1: plan = InternetPlan::NO_SERVICE; break;
                                case 2: plan = InternetPlan::BASIC; break;
                                case 3: plan = InternetPlan::STANDARD; break;
                                case 4: plan = InternetPlan::PREMIUM; break;
                                case 5: plan = InternetPlan::BUSINESS_FIBER; break;
                                default: plan = InternetPlan::NO_SERVICE;
                            }
                            
                            service = make_unique<InternetManagement>(location, plan);
                            break;
                        }
                        default:
                            cout << "Invalid service type." << endl;
                            break;
                    }
                    
                    if (service) {
                        ecoCity->addService(move(service));
                        cout << "Service added successfully!" << endl;
                    }
                } catch (const exception& e) {
                    cerr << "Error adding service: " << e.what() << endl;
                }
                break;
            }
            case 7: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                try {
                    ecoCity->simulateDay();
                    cout << "Day simulated successfully!" << endl;
                } catch (const exception& e) {
                    cerr << "Error simulating day: " << e.what() << endl;
                }
                break;
            }
            case 8: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                int days;
                cout << "Enter number of days to simulate: ";
                cin >> days;
                clearInputBuffer();
                
                try {
                    ecoCity->simulateDays(days);
                    cout << days << " days simulated successfully!" << endl;
                } catch (const exception& e) {
                    cerr << "Error simulating days: " << e.what() << endl;
                }
                break;
            }
            case 9: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                ecoCity->displayStatistics();
                break;
            }
            case 10: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                ecoCity->generateDetailedReport();
                break;
            }
            case 11: {
                if (!ecoCity) {
                    cout << "Please create a city first." << endl;
                    break;
                }
                
                string filename;
                cout << "Enter filename to save statistics: ";
                getline(cin, filename);
                
                if (ecoCity->saveStatisticsToFile(filename)) {
                    cout << "Statistics saved to " << filename << " successfully!" << endl;
                } else {
                    cout << "Failed to save statistics to file." << endl;
                }
                break;
            }
            case 12: {
                cout << "Exiting simulation. Thank you for using Smart Eco City Simulator!" << endl;
                running = false;
                break;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    }
    
    return 0;
}
