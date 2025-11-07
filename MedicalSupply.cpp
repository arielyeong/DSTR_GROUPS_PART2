// MedicalSupply.cpp
#include "MedicalSupply.hpp"

MedicalSupply::MedicalSupply() : top(-1), capacity(100), filename("data/medicalsupply.txt") {
    // Load existing data from file on initialization
    loadFromFile();
    cout << "Medical Supply Manager initialized. Loaded " << (top + 1) << " supplies from file." << endl;
}

MedicalSupply::~MedicalSupply() {
    // Save current state to file before destruction
    saveToFile();
}

void MedicalSupply::loadFromFile() {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "No existing file found. Starting with empty stock." << endl;
        return;
    }

    string line;
    top = -1;  // Reset stack
    int loadedCount = 0;

    while (getline(file, line)) {
        // Parse line: assumes format "type,quantity,batch" (comma-separated, no commas in fields)
        size_t comma1 = line.find(',');
        size_t comma2 = line.rfind(',');
        if (comma1 != string::npos && comma2 != string::npos && comma2 > comma1 + 1) {
            string typ = line.substr(0, comma1);
            string qtyStr = line.substr(comma1 + 1, comma2 - comma1 - 1);
            string bat = line.substr(comma2 + 1);

            // Convert quantity to int (handle potential errors with try-catch or check)
            int qty;
            try {
                qty = stoi(qtyStr);
            } catch (...) {
                cout << "Invalid quantity in file line: " << line << ". Skipping." << endl;
                continue;
            }

            if (top < capacity - 1) {
                top++;
                stack[top].type = typ;
                stack[top].quantity = qty;
                stack[top].batch = bat;
                loadedCount++;
            } else {
                cout << "Stack full during load. Truncating older entries." << endl;
                break;
            }
        }
    }
    file.close();
    cout << "Loaded " << loadedCount << " supplies from " << filename << "." << endl;
}

void MedicalSupply::saveToFile() {
    // Create directory if needed? (C++ standard doesn't, assume 'data' folder exists)
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file for saving: " << filename << endl;
        return;
    }

    // Write from bottom (index 0) to top (oldest to newest order for readability)
    for (int i = 0; i <= top; i++) {
        file << stack[i].type << "," << stack[i].quantity << "," << stack[i].batch << endl;
    }
    file.close();
    cout << "Saved " << (top + 1) << " supplies to " << filename << "." << endl;
}

void MedicalSupply::addSupply() {
    if (top >= capacity - 1) {
        cout << "Error: Stack overflow! Maximum capacity (" << capacity << ") reached." << endl;
        return;
    }

    string type, batch;
    int quantity;

    // Input (assumes no spaces in type/batch for simplicity; use cin.ignore() if needed after menu cin)
    cout << "Enter supply type: ";
    cin >> type;
    cout << "Enter quantity: ";
    cin >> quantity;
    cout << "Enter batch ID: ";
    cin >> batch;

    // Push to top of stack (LIFO: last added is on top)
    top++;
    stack[top].type = type;
    stack[top].quantity = quantity;
    stack[top].batch = batch;

    cout << "Supply added successfully: " << type << " (Qty: " << quantity << ", Batch: " << batch << ")" << endl;
    saveToFile();  // Persist after addition
}

void MedicalSupply::useSupply() {
    if (top < 0) {
        cout << "Error: No supplies available. Stack is empty." << endl;
        return;
    }

    // Pop from top (use last added)
    Supply used = stack[top];
    cout << "Using last added supply: " << used.type << " (Qty: " << used.quantity << ", Batch: " << used.batch << ")" << endl;
    top--;

    saveToFile();  // Persist after removal
}

void MedicalSupply::viewSupplies() {
    if (top < 0) {
        cout << "No supplies currently available." << endl;
        return;
    }

    cout << "\n=== Current Supplies (Oldest to Newest) ===" << endl;
    cout << "Total supplies: " << (top + 1) << endl;
    for (int i = 0; i <= top; i++) {
        cout << (i + 1) << ". Type: " << stack[i].type 
             << ", Quantity: " << stack[i].quantity 
             << ", Batch: " << stack[i].batch << endl;
    }
    cout << "========================================" << endl;
}

void MedicalSupply::menu() {
    int choice;
    do {
        cout << "\n--- Medical Supply Management (Role 2) ---" << endl;
        cout << "1. Add Supply Stock" << endl;
        cout << "2. Use 'Last Added' Supply" << endl;
        cout << "3. View Current Supplies" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        // Clear input buffer if needed (for future getline if extended)
        cin.ignore();

        switch (choice) {
            case 1:
                addSupply();
                break;
            case 2:
                useSupply();
                break;
            case 3:
                viewSupplies();
                break;
            case 0:
                cout << "Returning to main menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}