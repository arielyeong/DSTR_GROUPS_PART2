#ifndef MEDICAL_SUPPLY_HPP
#define MEDICAL_SUPPLY_HPP

#include <iostream>
#include <string>

struct SupplyItem {
    std::string type;
    int quantity;
    std::string batch;
};

class MedicalSupply {
private:
    SupplyItem stack[100];  // Fixed-size array simulating stack (no STL)
    int top;                // Stack top index

public:
    MedicalSupply();        // Constructor
    ~MedicalSupply();       // Destructor (optional, but good practice)
    void addSupplyStock();  // Func 1: Add new supply
    void useLastAddedSupply(); // Func 2: Remove/pop last added
    void viewCurrentSupplies(); // Func 3: Display stack
    void menu();            // Menu for your role
};

#endif