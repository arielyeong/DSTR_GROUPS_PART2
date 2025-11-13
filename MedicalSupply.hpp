#ifndef MEDICALSUPPLY_HPP
#define MEDICALSUPPLY_HPP

#include <string>

class SupplyItem {
public:
    std::string type;
    int quantity;
    std::string batch;
    std::string expiryDate;  // Expiry date for each supply item
    std::string remark;      // Additional remark for the supply item
    SupplyItem* next;

    SupplyItem(const std::string& type, int quantity, const std::string& batch,
                const std::string& expiryDate, const std::string& remark)
        : type(type), quantity(quantity), batch(batch), expiryDate(expiryDate), 
          remark(remark), next(nullptr) {}
};

class MedicalSupply {
private:
    SupplyItem* top;  // Top of the stack
    int itemCount;

    bool isEmpty() const;  // Check if the stack is empty
    std::string generateBatchID();  // Generate unique batch ID

public:
    MedicalSupply();
    ~MedicalSupply();

    void menu();  // Display menu for medical supply management
    void addSupplyStock();  // Add new supply to the stack
    void useLastAddedSupply();  // Use the most recent supply (LIFO)
    void viewCurrentSupplies();  // View the list of all supplies

    int getItemCount() const;  // Get total number of supplies
};

#endif
