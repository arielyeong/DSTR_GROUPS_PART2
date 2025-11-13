// ============================================================================
// MedicalSupply.cpp
// Implementation for Role 2: Medical Supply Manager (STACK via Linked List)
// ----------------------------------------------------------------------------
// Re-Why LINKED LIST + STACK (LIFO)?
// The linked list is used to implement the stack, ensuring dynamic memory usage 
// and efficient operations (push and pop). Each supply item is a node with 
// a pointer to the next node, making adding/removing items fast with no 
// fixed size limitations like arrays.
//
// ----------------------------------------------------------------------------
// Complexity Summary:
//   Push (Add Supply)        → O(1)
//   Pop (Use Last Supply)    → O(1)
//   View (Traverse Stack)    → O(n)
//   File I/O (Save/Load)     → O(n)
// ============================================================================

#include "MedicalSupply.hpp"
#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include <regex>
#include <chrono>

using namespace std;
using namespace std::chrono;

MedicalSupply::MedicalSupply() : top(nullptr), itemCount(0) {}

MedicalSupply::~MedicalSupply() {
    while (!isEmpty()) {
        useLastAddedSupply();
    }
}

string MedicalSupply::generateBatchID() {
    // Manually set batch ID to "BID1" and increment yourself
    static int batchNumber = 1;
    stringstream batchID;
    batchID << "BID" << batchNumber++;
    return batchID.str();
}

bool MedicalSupply::isEmpty() const {
    return top == nullptr;
}

int MedicalSupply::getItemCount() const {
    return itemCount;
}

bool isValidDate(const string& date) {
    // Validate date format as YYYY-MM-DD using regular expression
    regex datePattern(R"(\d{4}-\d{2}-\d{2})");
    return regex_match(date, datePattern);
}

// Function to compare the input date with the current date
bool isDateInFuture(const string& date) {
    // Split the input date into year, month, and day
    int year, month, day;
    sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);

    // Get the current date using std::chrono
    auto today = system_clock::now();
    time_t today_time = system_clock::to_time_t(today);
    struct tm *local_time = localtime(&today_time);

    // Compare the input date with today's date
    if (year < (1900 + local_time->tm_year)) {
        return false;  // The input year is in the past
    } else if (year == (1900 + local_time->tm_year)) {
        if (month < (local_time->tm_mon + 1)) {
            return false;  // The input month is in the past (same year)
        } else if (month == (local_time->tm_mon + 1)) {
            if (day <= local_time->tm_mday) {
                return false;  // The input day is today or in the past
            }
        }
    }

    return true;  // The date is in the future
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

        switch (choice) {
            case 1:
                addSupplyStock();
                break;
            case 2:
                useLastAddedSupply();
                break;
            case 3:
                viewCurrentSupplies();
                break;
            case 0:
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (choice != 0);
}

void MedicalSupply::addSupplyStock() {
    string type, expiryDate, remark;
    int quantity;

    cout << "\n--- Add Supply Stock ---" << endl;

    // Clear input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter supply type (e.g., Mask, Gloves, Syringe, Medicine): ";
    getline(cin, type);

    cout << "Enter quantity: ";
    cin >> quantity;

    // Validate quantity
    while (quantity <= 0) {
        cout << "Quantity must be positive. Please enter a valid quantity: ";
        cin >> quantity;
    }

    // Generate unique batch ID automatically (using BID1, BID2, etc.)
    string batch = generateBatchID();

    // Get expiry date and validate
    cout << "Enter expiry date (YYYY-MM-DD): ";
    cin.ignore();
    getline(cin, expiryDate);

    // Date validation
    while (!isValidDate(expiryDate) || !isDateInFuture(expiryDate)) {
        cout << "Invalid or past expiry date. Please enter a valid future expiry date (YYYY-MM-DD): ";
        getline(cin, expiryDate);
    }

    // Get remark
    cout << "Enter any remarks (optional): ";
    getline(cin, remark);

    // Create new supply item
    SupplyItem* newItem = new SupplyItem(type, quantity, batch, expiryDate, remark);

    // Push to stack (LIFO - Last In First Out)
    newItem->next = top;
    top = newItem;
    itemCount++;

    cout << "✓ Supply stock added successfully!" << endl;
    cout << "Added: " << quantity << " " << type << " (Batch: " << batch 
         << ", Expiry Date: " << expiryDate << ", Remark: " << remark << ")" << endl;
    cout << "Total supply items in storage: " << itemCount << endl;
}

void MedicalSupply::useLastAddedSupply() {
    if (isEmpty()) {
        cout << "\nNo supplies available to use! Storage is empty." << endl;
        return;
    }

    cout << "\n--- Using Last Added Supply ---" << endl;

    // Get the top item (last added)
    SupplyItem* itemToUse = top;

    int useQuantity;
    cout << "Using: " << itemToUse->quantity << " " << itemToUse->type 
         << " (Batch: " << itemToUse->batch << ", Expiry Date: " << itemToUse->expiryDate 
         << ", Remark: " << itemToUse->remark << ")" << endl;

    cout << "Enter quantity to use: ";
    cin >> useQuantity;

    // Validate quantity
    if (useQuantity <= 0 || useQuantity > itemToUse->quantity) {
        cout << "Invalid quantity. Please enter a value between 1 and " << itemToUse->quantity << endl;
        return;
    }

    // Subtract the used quantity
    itemToUse->quantity -= useQuantity;

    // If the quantity is now zero, pop the item from the stack
    if (itemToUse->quantity == 0) {
        top = top->next;
        delete itemToUse;
        itemCount--;
    }

    cout << "Supply used successfully!" << endl;
    cout << "Remaining supply items in storage: " << itemCount << endl;
}

void MedicalSupply::viewCurrentSupplies() {
    if (isEmpty()) {
        cout << "\nStorage is empty. No supplies available." << endl;
        return;
    }

    cout << "\n--- Current Supplies (Last Added First) ---" << endl;
    cout << "Total items: " << itemCount << endl;
    cout << "----------------------------------------" << endl;

    SupplyItem* current = top;
    int position = 1;

    while (current != nullptr) {
        cout << position << ". Type: " << current->type << endl;
        cout << "   Quantity: " << current->quantity << endl;
        cout << "   Batch: " << current->batch << endl;
        cout << "   Expiry Date: " << current->expiryDate << endl;
        cout << "   Remark: " << (current->remark.empty() ? "No remarks" : current->remark) << endl;
        cout << "   ---" << endl;

        current = current->next;
        position++;
    }
}

