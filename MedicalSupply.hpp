#include "MedicalSupply.hpp"
#include <iostream>
#include <iomanip>  // For nicer output

using namespace std;

MedicalSupply::MedicalSupply() {
    top = -1;  // Empty stack
}

MedicalSupply::~MedicalSupply() {
    // Cleanup if needed (not required for array)
}

void MedicalSupply::addSupplyStock() {
    if (top >= 99) {
        cout << "Stack full! Cannot add more supplies." << endl;
        return;
    }
    SupplyItem item;
    cout << "Enter supply type: ";
    cin >> item.type;
    cout << "Enter quantity: ";
    cin >> item.quantity;
    cout << "Enter batch: ";
    cin >> item.batch;
    stack[++top] = item;  // Push to top
    cout << "Supply added to stock." << endl;
}

void MedicalSupply::useLastAddedSupply() {
    if (top < 0) {
        cout << "No supplies available!" << endl;
        return;
    }
    cout << "Using last added supply: " << stack[top].type << " (Qty: " << stack[top].quantity << ", Batch: " << stack[top].batch << ")" << endl;
    top--;  // Pop (just decrement top)
}

void MedicalSupply::viewCurrentSupplies() {
    if (top < 0) {
        cout << "No supplies in stock." << endl;
        return;
    }
    cout << "\nCurrent Supplies (LIFO order - top first):" << endl;
    cout << left << setw(15) << "Type" << setw(10) << "Quantity" << setw(15) << "Batch" << endl;
    cout << string(40, '-') << endl;
    for (int i = top; i >= 0; i--) {  // Print from top to bottom
        cout << left << setw(15) << stack[i].type << setw(10) << stack[i].quantity << setw(15) << stack[i].batch << endl;
    }
}

void MedicalSupply::menu() {
    int subChoice;
    do {
        cout << "\n--- Medical Supply Management ---" << endl;
        cout << "1. Add Supply Stock" << endl;
        cout << "2. Use Last Added Supply" << endl;
        cout << "3. View Current Supplies" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> subChoice;

        switch (subChoice) {
            case 1: addSupplyStock(); break;
            case 2: useLastAddedSupply(); break;
            case 3: viewCurrentSupplies(); break;
            case 0: cout << "Returning to main menu..." << endl; break;
            default: cout << "Invalid choice!" << endl;
        }
    } while (subChoice != 0);
}