#ifndef InternetManagement_H
#define InternetManagement_H

#include "Services.h"
#include "HousingScheme.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>

enum class InternetPlan{
    NO_SERVICE,
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

        static PlanDetails getPlanDetails(InternetPlan plan){
            switch (plan) {
                case InternetPlan::NO_SERVICE:
                    return {0.0, 0.0, 0, 0, "No Service"};
                case InternetPlan::STANDARD:
                    return {500.0, 100.0, 10, 50, "Standard Plan"};
                case InternetPlan::PREMIUM:
                    return {800.0, 200.0, 80, 100, "Premium Plan"};
                case InternetPlan::BUSINESS_FIBER:
                    return {1500.0, numeric_limits<double>::infinity(), 0.0, 1000, "Business Fiber Plan"};
                default:
                    throw invalid_argument("Invalid Internet Plan");
            }
        }

        string planToString() const{
            return getPlanDetails(currentPlan).displayName;
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
            return getPlanDetails(currentPlan).speedMbps;
        }

        double calculateBill() const{
            PlanDetails details = getPlanDetails(currentPlan);

            if (currentPlan == InternetPlan::NO_SERVICE) {
                return 0.0;
            }
            if (currentPlan == InternetPlan::BUSINESS_FIBER) {
                return details.baseCost; // No data cap for Business Fiber
            }
            double bill = details.baseCost;

            if (!isinf(details.dataCapGB) && dataUsedGB > details.dataCapGB && details.overageCostPerGB > 0) {
                double excessData = dataUsedGB - details.dataCapGB;
                bill += excessData * 10.0; // Rs. 10 per GB overage
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
                 << "for address: [";
                 address.display();
                 cout << "]" << endl;
            if (currentPlan == InternetPlan::NO_SERVICE) {
                cout << "STATUS: No internet service active" << endl;
            } else {
                cout << "Current speed: " << getCurrentSpeed() << " Mbps" << endl;
                cout << "STATUS: Complete" << endl;
            }
        }

        void showStatus() override{
            cout << "\n--- Internet Service ---" << endl;
            PlanDetails details = getPlanDetails(currentPlan);

            cout << "Current Plan: " << details.displayName << endl;
            if (currentPlan != InternetPlan::NO_SERVICE) {
                cout << "Data Used: " << dataUsedGB << " GB" << endl;
                cout << "Speed: " << details.speedMbps << " Mbps" << endl;
            if (!isinf(details.dataCapGB)){
                cout << "Data Cap: " << details.dataCapGB << " GB" << endl;
            } else {
                cout << "Data Cap: Unlimited" << endl;
            }
            if (details.overageCostPerGB > 0) {
                cout << "Overage Cost: Rs. " << details.overageCostPerGB << " per GB" << endl;
            } 
            cout << "Estimated Bill: Rs. " << calculateBill() << endl;
            } else {
                cout << "STATUS: NO active service." << endl;
            }
            cout << "-------------------------" << endl;
        }

        virtual ~InternetManagement() override = default; // Virtual Destructor
};

#endif 
