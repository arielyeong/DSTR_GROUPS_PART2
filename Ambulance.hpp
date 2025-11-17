#ifndef AMBULANCE_HPP
#define AMBULANCE_HPP

#include <iostream>
#include <string>
using namespace std;

// ambulance struct
struct Ambulance
{
    string id; //make it string to accommodate alphanumeric IDs
    std::string driverName;
    std::string status;
    Ambulance* next;
};

// Circular Queue class
class AmbulanceQueue
{
private:
    Ambulance* front;
    Ambulance* rear;

public:
    AmbulanceQueue(); // constructor
    ~AmbulanceQueue(); // destructor

    bool isEmpty();
    void registerAmbulance(); // add ambulance
    void rotateShift();       // rotate shift
    void displaySchedule();   // display current schedule
    void updateStatus();    // update ambulance status
    void searchAmbulance(); // search ambulance by ID
    void menu();              // menudriven
};

#endif