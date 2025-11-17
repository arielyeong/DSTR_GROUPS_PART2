#ifndef AMBULANCE_HPP
#define AMBULANCE_HPP

#include <iostream>
#include <string>
using namespace std;

// ambulance struct
struct Ambulance
{
    int id;
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
    void menu();              // menudriven
};

#endif