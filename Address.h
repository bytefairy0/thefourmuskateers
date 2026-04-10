#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/**
 * Address struct with polymorphic display functionality
 * Represents a physical location in the city
 */
struct Address {
    int streetNo;
    int houseNo;
    string city;
    string pin;

    // Constructor
    Address(int sNo = 0, int hNo = 0, string c = "", string p = "")
        : streetNo(sNo), houseNo(hNo), city(c), pin(p) {}

    // Detailed display with all information
    void display() const {
        cout << streetNo << "-" << houseNo << ", " << city << ", " << pin;
    }

    // Function overloading - simplified display
    string display(bool simplified) const {
        if (simplified) {
            return to_string(houseNo) + ", St." + to_string(streetNo);
        } else {
            stringstream ss;
            ss << streetNo << "-" << houseNo << ", " << city << ", " << pin;
            return ss.str();
        }
    }

    // Operator overloading for comparison
    bool operator==(const Address& other) const {
        return (streetNo == other.streetNo && 
                houseNo == other.houseNo && 
                city == other.city && 
                pin == other.pin);
    }

    // Operator overloading for streaming
    friend ostream& operator<<(ostream& os, const Address& addr) {
        os << addr.streetNo << "-" << addr.houseNo << ", " << addr.city << ", " << addr.pin;
        return os;
    }
};

#endif // ADDRESS_H