#ifndef SERVICES_H
#define SERVICES_H

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <limits>

using namespace std;

// Forward declaration
class PollutionControl;
class City;

/**
 * Abstract base class for all city services
 */
class Services {
protected:
    string serviceType;
    bool isActive;
    double reliabilityScore; // 0-100%
    vector<double> serviceReadings; // General service readings for logging
    
public:
    // Constructor
    Services(string type) 
        : serviceType(type), isActive(true), reliabilityScore(100.0) {}
    
    // Virtual destructor for proper inheritance
    virtual ~Services() = default;
    
    // Pure virtual functions to be implemented by derived classes
    virtual void supply() = 0;
    virtual void showStatus() = 0;
    
    // Common functions for all services
    string getServiceType() const { return serviceType; }
    
    bool getIsActive() const { return isActive; }
    
    void setActive(bool active) { isActive = active; }
    
    double getReliabilityScore() const { return reliabilityScore; }
    
    void setReliabilityScore(double score) {
        if (score < 0 || score > 100) {
            throw invalid_argument("Reliability score must be between 0 and 100");
        }
        reliabilityScore = score;
    }
    
    // Add a service reading for monitoring
    void addServiceReading(double reading) {
        serviceReadings.push_back(reading);
    }
    
    // Get average of service readings
    double getAverageReading() const {
        if (serviceReadings.empty()) {
            return 0.0;
        }
        double sum = 0.0;
        for (double reading : serviceReadings) {
            sum += reading;
        }
        return sum / serviceReadings.size();
    }
    
    // Basic display for service info
    virtual void displayServiceInfo() const {
        cout << "Service: " << serviceType << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "Reliability: " << reliabilityScore << "%" << endl;
        cout << "Average Reading: " << getAverageReading() << endl;
    }
    
    // Friend classes
    friend class PollutionControl;
    friend class City;
};

#endif // SERVICES_H