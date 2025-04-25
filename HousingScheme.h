#ifndef features.H
#define features.H

#include <iostream>
using namespace std;

struct Address{};
struct Owner{};
// Housing scheme 
class HousingScheme{
    // private data -> protected to give access to child class
    protected:
        Address address;
        Owner owner;
    // public data:
    public:
        
};

class Flats : public HousingScheme{
    public:
        double rent; // diff b/w float and rednt
}
