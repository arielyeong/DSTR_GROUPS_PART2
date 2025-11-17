#pragma once
#include <iostream>
#include <string>
#include <ctime>    // for time_t
using namespace std;

// node structure for each patient in the queue (doubly linked list queue)
struct PatientNode {
    string patientID;           // unique ID entered by user
    string name;                // patient name
    string conditionType;       // Normal / Critical / Emergency
    string admittedAt;          // formatted admission time as text
    time_t admittedTimeRaw;     // raw admission timestamp for waiting time calc
    PatientNode* nextAddress;   //show next patient
    PatientNode* prevAddress;   //show previous patient
};

// PatientQueue class to manage the queue operations
class PatientQueue {
private: 
    PatientNode* front;
    PatientNode* rear;
    int size;

    int nextPatientNumber;          // counter for auto ID (1 -> P001, 2 -> P002, ...)
    string generateNextPatientId(); // helper to format ID22
    
public:
    PatientQueue();   // Constructor: initializes empty queue
    ~PatientQueue();  // Destructor: frees all nodes

    bool isEmpty();          // Check if the queue is empty

    void admitPatient();     // 1. Add new patient to the rear of the queue
    void dischargePatient(); // 2. Remove (dequeue) the patient at the front
    void viewPatients();     // 3. Display all patients from front to rear
    void searchPatientById();// 4. Extra: search a patient by Patient ID
    void peekNextPatient();   

};

class PatientAdmission {
public:
    void menu();  // show patient admission menu and use PatientQueue inside
};