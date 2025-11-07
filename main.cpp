#include <iostream>
#include "PatientAdmission.hpp"
#include "MedicalSupply.hpp"
#include "Emergency.hpp"
#include "Ambulance.hpp"

using namespace std;

class MedicalSupply;
class Emergency;
class Ambulance;
class PatientAdmission;

int main() {
    // PatientAdmission pa;
    MedicalSupply ms;
    // Emergency ed;
    // Ambulance ad;

    int choice;
    do {
        cout << "=========================================" << endl;
        cout << "HOSPITAL PATIENT CARE MANAGEMENT SYSTEM" << endl;
        cout << "=========================================" << endl;
        cout << "1. Patient Admission (Role 1)" << endl;
        cout << "2. Medical Supply Management (Role 2)" << endl;
        cout << "3. Emergency Department Management (Role 3)" << endl;
        cout << "4. Ambulance Dispatch (Role 4)" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                pa.menu();
                break;
            case 2:
                ms.menu();
                break;
            case 3:
                ed.menu();
                break;
            case 4:
                ad.menu();
                break;
            case 0:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}