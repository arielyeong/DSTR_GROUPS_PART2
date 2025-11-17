#ifndef MEDICALSUPPLY_HPP
#define MEDICALSUPPLY_HPP

#include <iostream>
#include <string>
using namespace std;

// ======================================
// Supply Item (Node for Stack via Linked List)
// ======================================
struct SupplyItem {
    string type;
    int quantity;
    string batch;
    string expiryDate;
    string remark;
    SupplyItem* next;

    SupplyItem(string t, int q, string b, string e, string r)
        : type(t), quantity(q), batch(b), expiryDate(e), remark(r), next(nullptr) {}
};

// ======================================
// Medical Supply Manager (STACK - LIFO)
// ======================================
class MedicalSupply {
private:
    SupplyItem* top;
    int itemCount;

public:
    MedicalSupply();
    ~MedicalSupply();

    // Core operations
    void menu();
    void addSupplyStock();
    void useLastAddedSupply();
    void viewCurrentSupplies();
    void removeExpiredSupplies();

    // Helpers
    string generateBatchID();
    bool isEmpty() const;
    int getItemCount() const;

    // Sample data loader
    void loadSampleData();
};

#endif

