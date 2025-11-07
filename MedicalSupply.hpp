// MedicalSupply.hpp
#ifndef MEDICALSUPPLY_HPP
#define MEDICALSUPPLY_HPP

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Supply {
    string type;
    int quantity;
    string batch;
};

class MedicalSupply {
private:
    Supply stack[100];  // Fixed-size array for stack implementation (LIFO)
    int top;            // Index of the top element
    int capacity;       // Maximum capacity of the stack
    string filename;    // Path to the data file

public:
    MedicalSupply();  // Constructor: initializes and loads from file
    ~MedicalSupply(); // Destructor: saves to file on exit

    void menu();      // Displays the sub-menu for Medical Supply Management
    void addSupply(); // Functionality 1: Add new supply to the top of the stack
    void useSupply(); // Functionality 2: Remove (pop) the most recently added supply
    void viewSupplies(); // Functionality 3: Display all supplies from bottom (oldest) to top (newest)

private:
    void saveToFile();   // Saves the current stack to the text file
    void loadFromFile(); // Loads the stack from the text file on initialization
};

#endif