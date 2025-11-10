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
};

// Circular Queue class
class AmbulanceQueue
{
private:
    static const int MAX = 10; // arrange the number of ambulance (max=10)
    Ambulance queue[MAX];
    int front, rear, count; //shows the start, end. current ambulance how many

public:
    AmbulanceQueue(); // constructor
    bool isFull();
    bool isEmpty();
    void registerAmbulance(); // add ambulance
    void rotateShift();       // rotate shift
    void displaySchedule();   // display current schedule
    void menu();              // menudriven
};

#endif