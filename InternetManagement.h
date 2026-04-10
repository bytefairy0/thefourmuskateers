#ifndef INTERNETMANAGEMENT_H
#define INTERNETMANAGEMENT_H

#include "Services.h"
#include "Address.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <map>

using namespace std;

enum class InternetPlan{
    NO_SERVICE,
    BASIC,
    STANDARD,
    PREMIUM,
    BUSINESS_FIBER
};

struct PlanDetails{
    double baseCost;
    double dataCapGB;
    double overageCostPerGB; // For overage calculation
    int speedMbps;
    string displayName;
};

class InternetManagement: public Services{
    private:
        Address address;
        double dataUsedGB;
        InternetPlan currentPlan;
        static map<InternetPlan, PlanDetails> planDetailsMap;

        string planToString() const{
            auto it = planDetailsMap.find(currentPlan);
            if (it != planDetailsMap.end()) {
                return it->second.displayName;
            }
            return "Unknown Plan";
        }

    public:
        InternetManagement(Address ad, InternetPlan plan = InternetPlan::NO_SERVICE, double initialDataUsage = 0.0)
            : Services("Internet"), address(ad), currentPlan(plan), dataUsedGB(initialDataUsage) {
                if (initialDataUsage < 0) {
                    throw invalid_argument("Initial data usage cannot be negative.");
                }
        }

        void addDataUsage(double data) {
            if (data < 0) {
                throw invalid_argument("Data usage cannot be negative.");
            }
            dataUsedGB += data;
        }

        int getCurrentSpeed() const {
            auto it = planDetailsMap.find(currentPlan);
            if (it != planDetailsMap.end()) {
                return it->second.speedMbps;
            }
            return 0;
        }

        double calculateBill() const{
            auto it = planDetailsMap.find(currentPlan);
            if (it == planDetailsMap.end()) {
                return 0.0; // No plan or unknown plan
            }
            
            const PlanDetails& details = it->second;

            if (currentPlan == InternetPlan::NO_SERVICE) {
                return 0.0;
            }
            if (currentPlan == InternetPlan::BUSINESS_FIBER) {
                return details.baseCost; // No data cap for Business Fiber
            }
            double bill = details.baseCost;

            if (!std::isinf(details.dataCapGB) && dataUsedGB > details.dataCapGB && details.overageCostPerGB > 0) {
                double excessData = dataUsedGB - details.dataCapGB;
                bill += excessData * details.overageCostPerGB;
            }

            return bill;
        }

        // --- Getters ---
        const Address& getAddress() const {return address;}
        double getDataUsedGB() const {return dataUsedGB;}
        InternetPlan getCurrentPlan() const {return currentPlan;}

        // --- Setters ---
        void setCurrentPlan(InternetPlan newPlan) {
            currentPlan = newPlan;
            dataUsedGB = 0.0; // Reset data usage when plan changes
            cout << "Internet plan changed to: " << planToString() << endl;
        }

        // --- Virtual Functions ---
        void supply() override {
            cout << "Managing internet Service (" << planToString() << ") "
                 << "for address: [" << address << "]" << endl;
            if (currentPlan == InternetPlan::NO_SERVICE) {
                cout << "STATUS: No internet service active" << endl;
            } else {
                cout << "Current speed: " << getCurrentSpeed() << " Mbps" << endl;
                cout << "STATUS: Complete" << endl;
            }
        }

        void showStatus() override{
            cout << "\n--- Internet Service ---" << endl;
            auto it = planDetailsMap.find(currentPlan);
            if (it == planDetailsMap.end()) {
                cout << "Current Plan: Unknown" << endl;
                cout << "STATUS: Error in plan details" << endl;
                cout << "-------------------------" << endl;
                return;
            }
            
            const PlanDetails& details = it->second;

            cout << "Current Plan: " << details.displayName << endl;
            if (currentPlan != InternetPlan::NO_SERVICE) {
                cout << "Data Used: " << dataUsedGB << " GB" << endl;
                cout << "Speed: " << details.speedMbps << " Mbps" << endl;
                if (!std::isinf(details.dataCapGB)){
                    cout << "Data Cap: " << details.dataCapGB << " GB" << endl;
                } else {
                    cout << "Data Cap: Unlimited" << endl;
                }
                if (details.overageCostPerGB > 0) {
                    cout << "Overage Cost: Rs. " << details.overageCostPerGB << " per GB" << endl;
                } 
                cout << "Estimated Bill: Rs. " << calculateBill() << endl;
            } else {
                cout << "STATUS: No active service." << endl;
            }
            cout << "-------------------------" << endl;
        }

        virtual ~InternetManagement() override = default; // Virtual Destructor
};

// Initialize the static member with plan details
map<InternetPlan, PlanDetails> InternetManagement::planDetailsMap = {
    {InternetPlan::NO_SERVICE, {0.0, 0.0, 0.0, 0, "No Service"}},
    {InternetPlan::BASIC, {29.99, 50.0, 1.0, 25, "Basic Plan"}},
    {InternetPlan::STANDARD, {49.99, 100.0, 1.0, 50, "Standard Plan"}},
    {InternetPlan::PREMIUM, {79.99, 200.0, 0.5, 100, "Premium Plan"}},
    {InternetPlan::BUSINESS_FIBER, {199.99, numeric_limits<double>::infinity(), 0.0, 1000, "Business Fiber Plan"}}
};

#endif // INTERNETMANAGEMENT_H
