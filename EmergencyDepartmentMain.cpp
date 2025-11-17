#include "EmergencyDepartment.hpp"
#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

// Function to clear screen (cross-platform)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to pause and wait for user input
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Function to display the main menu
void displayMainMenu() {
    cout << "\n";
    cout << "==================================================\n";
    cout << "     EMERGENCY DEPARTMENT MANAGEMENT SYSTEM      \n";
    cout << "==================================================\n";
    cout << " 1. Log Emergency Case                           \n";
    cout << " 2. Process Most Critical Case                   \n";
    cout << " 3. View All Pending Emergency Cases             \n";
    cout << " 4. View Cases Being Processed                   \n";  
    cout << " 5. Display Department Statistics                \n";
    cout << " 6. View Officer Information                     \n";
    cout << " 7. Generate Sample Test Cases                   \n";
    cout << " 8. Clear All Cases (Reset System)               \n";
    cout << " 0. Exit System                                  \n";
    cout << "==================================================\n";
    cout << "Enter your choice: ";
}

// Function to generate sample test cases for demonstration
void generateSampleCases(EmergencyDepartmentOfficer& officer) {
    cout << "\n===== GENERATING SAMPLE TEST CASES =====\n";
    
    // Create a temporary priority queue for testing
    EmergencyPriorityQueue testQueue(15);
    
    // Sample emergency cases with varying priorities
    struct SampleCase {
        string name;
        string type;
        int priority;
        string notes;
    };
    
    SampleCase samples[] = {
        {"Ong Heng Huat", "Cardiac Arrest", 10, "Immediate CPR required"},
        {"Happy Ng", "Severe Trauma", 9, "Multiple injuries from accident"},
        {"Jordon Neil", "Stroke Symptoms", 9, "Time-critical intervention needed"},
        {"Pattrick Ong", "Respiratory Distress", 8, "Severe breathing difficulty"},
        {"Michael Jackson", "Chest Pain", 7, "Possible heart attack"},
        {"Gorila Morzila", "Head Injury", 7, "Concussion symptoms"},
        {"Chin Qin", "Severe Bleeding", 6, "Deep laceration"},
        {"Princess Wong", "Fracture", 5, "Compound fracture - leg"},
        {"Taylor Swift", "Abdominal Pain", 4, "Acute pain, unknown cause"},
        {"Banana Lam", "Minor Burns", 3, "Second-degree burns on arm"},
        {"John Cena", "Allergic Reaction", 6, "Moderate swelling"},
        {"Chia Xia Xia", "Asthma Attack", 7, "Inhaler not effective"}
    };
    
    int numSamples = sizeof(samples) / sizeof(samples[0]);
    
    cout << "Generating " << numSamples << " sample emergency cases...\n\n";
    
    // Insert sample cases
    for (int i = 0; i < numSamples; i++) {
        EmergencyCase newCase(2001 + i, samples[i].name, samples[i].type, 
                             samples[i].priority, samples[i].notes);
        testQueue.insertEmergencyCase(newCase);
        
        cout << "Added: " << samples[i].name 
             << " - " << samples[i].type 
             << " (Priority: " << samples[i].priority << ")\n";
    }
    
    cout << "\n " << numSamples << " sample cases generated successfully!\n";
    
    // Display the cases in priority order
    cout << "\n--- Cases Sorted by Priority ---\n";
    testQueue.displayAllCases();
    
    // Demonstrate processing high-priority cases
    cout << "\n===== PROCESSING HIGH-PRIORITY CASES =====\n";
    
    for (int i = 0; i < 3 && !testQueue.isEmpty(); i++) {
        EmergencyCase processed = testQueue.extractMostCritical();
        cout << "Processed Case #" << (i+1) << ": " 
             << processed.patientName 
             << " (Priority: " << processed.priorityLevel << ")\n";
    }
    
    cout << "\nRemaining cases after processing:\n";
    testQueue.displayAllCases();
}

// Main program
void EmergencyDepartmentOfficer::menu() {
    int choice;
    bool exitProgram = false;
    
    // Display welcome message
    clearScreen();
    cout << "****************************************************************\n";
    cout << "*                                                              *\n";
    cout << "*              HOSPITAL PATIENT CARE MANAGEMENT                *\n";
    cout << "*                                                              *\n";
    cout << "*       Developed for: Role 3 EMERGENCY DEPARTMENT OFFICER     *\n";
    cout << "*            Data Structure: Priority Queue (Heap)             *\n";
    cout << "*                                                              *\n";
    cout << "****************************************************************\n";
    pauseScreen();
    
    // Main program loop
    while (!exitProgram) {
        clearScreen();
        displayMainMenu();
        
        // Input validation for menu choice
        while (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        switch (choice) {
            case 1:
                // Log Emergency Case
                clearScreen();
                logEmergencyCase();
                pauseScreen();
                break;
                
            case 2:
                // Process Most Critical Case
                clearScreen();
                processMostCriticalCase();
                pauseScreen();
                break;
                
            case 3:
                // View All Pending Emergency Cases
                clearScreen();
                viewPendingEmergencyCases();
                pauseScreen();
                break;
                
            case 4:
                // View Cases Being Processed
                clearScreen();
                viewCasesBeingProcessed();
                pauseScreen();
                break;
                
            case 5:
                // Display Department Statistics
                clearScreen();
                cout << "\n===== DEPARTMENT STATISTICS =====\n";
                viewCasesBeingProcessed();
                viewPendingEmergencyCases();
                pauseScreen();
                break;
                
            case 6:
                // View Officer Information
                clearScreen();
                displayOfficerInfo();
                pauseScreen();
                break;    
                
            case 7:
                // Generate Sample Test Cases
                clearScreen();
                generateSampleCases(*this);
                pauseScreen();
                break;
                
            case 8:
                // Clear All Cases 
                clearScreen();
                cout << "\n===== CLEAR ALL CASES =====\n";
                cout << "WARNING: This will remove all pending emergency cases!\n";
                cout << "Are you sure? (Y/N): ";
                char confirm;
                cin >> confirm;
                if (confirm == 'Y' || confirm == 'y') {
                    // Reset the priority queue
                    delete priorityQueue;
                    priorityQueue = new EmergencyPriorityQueue(20);
                    
                    // Reset processed cases
                    delete[] casesBeingProcessed;
                    casesBeingProcessed = new EmergencyCase[maxProcessedCapacity];
                    processedCount = 0;
                    
                    // Reset case ID counter
                    nextCaseID = 1001;
                    
                    cout << "System has been reset. All cases cleared.\n";
                } else {
                    cout << "Operation cancelled.\n";
                }
                pauseScreen();
                break;
                
            case 0:
                // Return to main menu
                clearScreen();
                cout << "\n===== RETURNING TO MAIN MENU =====\n";
                cout << "Returning to Hospital Management System...\n";
                exitProgram = true;
                pauseScreen();
                break;
                
            default:
                cout << "\nInvalid choice! Please select from the menu.\n";
                pauseScreen();
                break;
        }
    }
}