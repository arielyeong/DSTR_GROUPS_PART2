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
//   RemoveExpiredSupplies    → O(n)
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

// ==========================================================
// Constructor & Destructor
// ==========================================================
MedicalSupply::MedicalSupply() : top(nullptr), itemCount(0) {}

MedicalSupply::~MedicalSupply() {
    while (!isEmpty()) {
        useLastAddedSupply();
    }
}

// ==========================================================
// Batch ID Generator (BID1, BID2...)
// ==========================================================
string MedicalSupply::generateBatchID() {
    static int num = 1;
    stringstream ss;
    ss << "BID" << num++;
    return ss.str();
}

// ==========================================================
// DATE VALIDATION — STRICT
// ==========================================================
bool isValidNumericDate(int y, int m, int d) {
    if (m < 1 || m > 12) return false;
    if (d < 1) return false;

    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    if (leap) daysInMonth[1] = 29;

    return d <= daysInMonth[m - 1];
}

bool isValidDateStrict(const string& date) {
    if (!regex_match(date, regex(R"(\d{4}-\d{2}-\d{2})"))) return false;

    int y, m, d;
    sscanf(date.c_str(), "%d-%d-%d", &y, &m, &d);

    return isValidNumericDate(y, m, d);
}

bool isDateExpired(const string& date) {
    int y, m, d;
    sscanf(date.c_str(), "%d-%d-%d", &y, &m, &d);

    auto now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
    tm* t = localtime(&tt);

    int cy = t->tm_year + 1900;
    int cm = t->tm_mon + 1;
    int cd = t->tm_mday;

    if (y < cy) return true;
    if (y == cy && m < cm) return true;
    if (y == cy && m == cm && d < cd) return true;

    return false;
}

bool isDateInFutureStrict(const string& date) {
    return !isDateExpired(date);
}

// ==========================================================
// Stack Checks
// ==========================================================
bool MedicalSupply::isEmpty() const {
    return top == nullptr;
}

int MedicalSupply::getItemCount() const {
    return itemCount;
}

// ==========================================================
// MENU
// ==========================================================
void MedicalSupply::menu() {
    int choice;

    do {
        cout << "\n==================================================\n";
        cout << "           MEDICAL SUPPLY MANAGEMENT (ROLE 2)      \n";
        cout << "==================================================\n";
        cout << " 1. Add Supply Stock\n";
        cout << " 2. Use Last Added Supply\n";
        cout << " 3. View Current Supplies\n";
        cout << " 4. Remove Expired Supplies\n";  // <-- new option
        cout << " 0. Back to Main Menu\n";
        cout << "==================================================\n";
        cout << " Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addSupplyStock(); break;
            case 2: useLastAddedSupply(); break;
            case 3: viewCurrentSupplies(); break;
            case 4: removeExpiredSupplies(); break;  // <-- new
            case 0:
                cout << " Returning to Main Menu...\n";
                break;
            default:
                cout << " Invalid choice.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (choice != 0);
}

// ==========================================================
// ADD SUPPLY STOCK
// ==========================================================
void MedicalSupply::addSupplyStock() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string type, expiry, remark;
    int qty;

    cout << "\n============ ADD SUPPLY STOCK ============\n";
    cout << " Enter supply type: ";
    getline(cin, type);

    cout << " Enter quantity: ";
    while (!(cin >> qty) || qty <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << " Invalid input! Try again: ";
    }

    cin.ignore();
    cout << " Enter expiry date (YYYY-MM-DD): ";
    getline(cin, expiry);

    while (!isValidDateStrict(expiry) || !isDateInFutureStrict(expiry)) {
        cout << " Invalid or past date. Enter valid future date (YYYY-MM-DD): ";
        getline(cin, expiry);
    }

    cout << " Enter remarks (optional): ";
    getline(cin, remark);

    string batchID = generateBatchID();
    SupplyItem* n = new SupplyItem(type, qty, batchID, expiry, remark);

    n->next = top;
    top = n;
    itemCount++;

    cout << "\n========== SUPPLY ADD TICKET ==========\n";
    cout << " Type        : " << type << "\n";
    cout << " Quantity    : " << qty << "\n";
    cout << " Batch ID    : " << batchID << "\n";
    cout << " Expiry Date : " << expiry << "\n";
    cout << " Remark      : " << remark << "\n";
    cout << "=======================================\n";
}

// ==========================================================
// USE LAST ADDED SUPPLY
// ==========================================================
void MedicalSupply::useLastAddedSupply() {
    if (isEmpty()) {
        cout << "\n No supplies available!\n";
        return;
    }

    SupplyItem* item = top;

    cout << "\n============ USE LAST ADDED SUPPLY ============\n";
    cout << " Type        : " << item->type << "\n";
    cout << " Qty in Stock: " << item->quantity << "\n";
    cout << " Batch ID    : " << item->batch << "\n";
    cout << " Expiry Date : " << item->expiryDate << "\n";
    cout << "\n===============================================\n";

    int useQty;
    cout << " Enter quantity to use: ";
    while (!(cin >> useQty) || useQty <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << " Invalid input! Enter a number: ";
    }

    if (useQty > item->quantity) {
        cout << " Not enough quantity! Maximum available: " << item->quantity << "\n";
        return;
    }

    item->quantity -= useQty;

    cout << "\n========== SUPPLY USAGE TICKET ==========\n";
    cout << " Used Quantity : " << useQty << "\n";
    cout << " Batch ID      : " << item->batch << "\n";
    cout << " Type          : " << item->type << "\n";
    cout << "=========================================\n";

    if (item->quantity == 0) {
    cout << "\n----------------------------------------------\n";
    cout << " The supply '" << item->type << "' (Batch " << item->batch 
         << ") has been completely used.\n";
    cout << " It is now removed from the storage stack.\n";
    cout << "----------------------------------------------\n";

    top = top->next;
    delete item;
    itemCount--;
}
}

// ==========================================================
// REMOVE EXPIRED SUPPLIES
// ==========================================================
void MedicalSupply::removeExpiredSupplies() {
    if (isEmpty()) {
        cout << "\n No supplies to check.\n";
        return;
    }

    cout << "\n============ REMOVE EXPIRED SUPPLIES ============\n";

    SupplyItem* curr = top;
    SupplyItem* prev = nullptr;
    int removedCount = 0;

    while (curr != nullptr) {
        if (isDateExpired(curr->expiryDate)) {
            cout << " [REMOVED] " << curr->type << " (Batch " << curr->batch 
                 << ") Expired on " << curr->expiryDate << "\n";

            removedCount++;

            if (curr == top) {
                top = curr->next;
                delete curr;
                curr = top;
            } else {
                prev->next = curr->next;
                delete curr;
                curr = prev->next;
            }

            itemCount--;
        } 
        else {
            prev = curr;
            curr = curr->next;
        }
    }

    if (removedCount == 0) {
        cout << " No expired supplies found.\n";
    } else {
        cout << "----------------------------------------------\n";
        cout << " Total expired supplies removed: " << removedCount << "\n";
        cout << "----------------------------------------------\n";
    }
}

// ==========================================================
// VIEW CURRENT SUPPLIES
// ==========================================================
void MedicalSupply::viewCurrentSupplies() {
    if (isEmpty()) {
        cout << "\n==================================================\n";
        cout << "           CURRENT SUPPLIES (EMPTY)                \n";
        cout << "==================================================\n";
        return;
    }

    cout << "\n==================================================\n";
    cout << "           CURRENT SUPPLIES (LAST ADDED FIRST)     \n";
    cout << "==================================================\n";

    SupplyItem* cur = top;
    int num = 1;

    while (cur != nullptr) {
        cout << "\n--------------- TICKET #" << num << " -----------------\n";
        cout << " Type        : " << cur->type << "\n";
        cout << " Quantity    : " << cur->quantity << "\n";
        cout << " Batch ID    : " << cur->batch << "\n";
        cout << " Expiry Date : " << cur->expiryDate << "\n";
        cout << " Remark      : " << (cur->remark.empty() ? "None" : cur->remark) << "\n";
        cout << "----------------------------------------------\n";

        cur = cur->next;
        num++;
    }
}

// ==========================================================
// SAMPLE DATA
// ==========================================================
void MedicalSupply::loadSampleData() {
    // Add one expired item for demonstration:
    SupplyItem* expired = new SupplyItem("Old Mask", 30, generateBatchID(), "2023-01-01", "Expired item");

    SupplyItem* s1 = new SupplyItem("Mask", 200, generateBatchID(), "2026-05-30", "N95 hospital grade");
    SupplyItem* s2 = new SupplyItem("Gloves", 40, generateBatchID(), "2026-02-10", "Latex-free");
    SupplyItem* s3 = new SupplyItem("Syringe", 10, generateBatchID(), "2027-01-01", "5ml sterile");

    expired->next = top; top = expired; itemCount++;
    s1->next = top; top = s1; itemCount++;
    s2->next = top; top = s2; itemCount++;
    s3->next = top; top = s3; itemCount++;
}
