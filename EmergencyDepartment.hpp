#ifndef EMERGENCY_DEPARTMENT_HPP
#define EMERGENCY_DEPARTMENT_HPP

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

struct EmergencyCase {
    int caseID;                    
    string patientName;            
    string emergencyType;          
    int priorityLevel;             
    string arrivalTime;            
    string additionalNotes;        
    
    EmergencyCase() : caseID(0), patientName(""), emergencyType(""), 
                      priorityLevel(0), arrivalTime(""), additionalNotes("") {}
    
    EmergencyCase(int id, string name, string type, int priority, string notes = "") 
        : caseID(id), patientName(name), emergencyType(type), 
          priorityLevel(priority), additionalNotes(notes) {
        time_t now = time(0);
        arrivalTime = string(ctime(&now));
        if (!arrivalTime.empty() && arrivalTime.back() == '\n') {
            arrivalTime.pop_back();
        }
    }
};

class EmergencyPriorityQueue {
private:
    EmergencyCase* heap;           
    int capacity;                
    int currentSize;               
    
    // Get parent of current index
    int getParentIndex(int index) {
        return (index - 1) / 2;
    }
    
    // Get left child for current index
    int getLeftChildIndex(int index) {
        return (2 * index) + 1;
    }
    
    // Get right child for current index
    int getRightChildIndex(int index) {
        return (2 * index) + 2;
    }
    
    // Helper function to check if node has left child
    bool hasLeftChild(int index) {
        return getLeftChildIndex(index) < currentSize;
    }
    
    // Helper function to check if node has right child
    bool hasRightChild(int index) {
        return getRightChildIndex(index) < currentSize;
    }
    
    // Helper function to check if node has parent
    bool hasParent(int index) {
        return getParentIndex(index) >= 0;
    }
    
    // Helper function to swap two elements
    void swap(int index1, int index2) {
        EmergencyCase temp = heap[index1];
        heap[index1] = heap[index2];
        heap[index2] = temp;
    }
    
    // Heapify up (bubble up) - used after insertion
    void heapifyUp(int index) {
        // Continue while the node has a parent and violates heap property
        while (hasParent(index) && 
               heap[getParentIndex(index)].priorityLevel < heap[index].priorityLevel) {
            // Swap with parent
            swap(getParentIndex(index), index);
            // Move up to parent's position
            index = getParentIndex(index);
        }
    }
    
    // Heapify down (bubble down) - used after deletion
    void heapifyDown(int index) {
        while (hasLeftChild(index)) {
            // Find the child with higher priority
            int largerChildIndex = getLeftChildIndex(index);
            
            // Check if right child exists and has higher priority
            if (hasRightChild(index) && 
                heap[getRightChildIndex(index)].priorityLevel > heap[largerChildIndex].priorityLevel) {
                largerChildIndex = getRightChildIndex(index);
            }
            
            // If current node has higher or equal priority than both children, stop
            if (heap[index].priorityLevel >= heap[largerChildIndex].priorityLevel) {
                break;
            }
            
            // Swap with the larger child
            swap(index, largerChildIndex);
            // Move down to child's position
            index = largerChildIndex;
        }
    }
    
    void resizeHeap() {
        int newCapacity = capacity * 2;
        EmergencyCase* newHeap = new EmergencyCase[newCapacity];
        
        // Copy existing elements
        for (int i = 0; i < currentSize; i++) {
            newHeap[i] = heap[i];
        }
        
        // Delete old heap and update pointers
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
        
        cout << "\nHeap capacity expanded to " << capacity << " to accommodate more cases.\n";
    }
    
public:
    EmergencyPriorityQueue(int initialCapacity = 10) {
        capacity = initialCapacity;
        currentSize = 0;
        heap = new EmergencyCase[capacity];
    }
    
    ~EmergencyPriorityQueue() {
        delete[] heap;
    }
    
    // Check if queue is empty
    bool isEmpty() {
        return currentSize == 0;
    }
    
    // Check if queue is full
    bool isFull() {
        return currentSize == capacity;
    }
    
    // Get current size
    int getSize() {
        return currentSize;
    }
    
    // Insert a new emergency case 
    void insertEmergencyCase(EmergencyCase newCase) {
        // Check if resize is needed
        if (isFull()) {
            resizeHeap();
        }
        
        // Add new element at the end
        heap[currentSize] = newCase;
        currentSize++;
        
        // Restore heap property by bubbling up
        heapifyUp(currentSize - 1);
    }
    
    // Remove and return the highest priority case (Dequeue)
    EmergencyCase extractMostCritical() {
        if (isEmpty()) {
            throw runtime_error("Cannot extract from empty priority queue!");
        }
        
        // Store the root (highest priority element)
        EmergencyCase mostCritical = heap[0];
        
        // Move last element to root
        heap[0] = heap[currentSize - 1];
        currentSize--;
        
        // Restore heap property by bubbling down
        if (currentSize > 0) {
            heapifyDown(0);
        }
        
        return mostCritical;
    }
    
    // Peek at the highest priority case without removing it
    EmergencyCase peekMostCritical() {
        if (isEmpty()) {
            throw runtime_error("Priority queue is empty!");
        }
        return heap[0];
    }
    
    // Display all emergency cases in priority order (without modifying heap)
    void displayAllCases() {
        if (isEmpty()) {
            cout << "\nNo emergency cases pending.\n";
            return;
        }
        
        // Create a temporary copy of the heap for sorting
        EmergencyCase* tempArray = new EmergencyCase[currentSize];
        for (int i = 0; i < currentSize; i++) {
            tempArray[i] = heap[i];
        }
        
        // Sort the temporary array by priority (bubble sort for simplicity)
        for (int i = 0; i < currentSize - 1; i++) {
            for (int j = 0; j < currentSize - i - 1; j++) {
                if (tempArray[j].priorityLevel < tempArray[j + 1].priorityLevel) {
                    EmergencyCase temp = tempArray[j];
                    tempArray[j] = tempArray[j + 1];
                    tempArray[j + 1] = temp;
                }
            }
        }
        
        // Display sorted cases
        cout << "\n========================================";
        cout << " EMERGENCY CASES BY PRIORITY ";
        cout << "========================================\n";
        cout << left << setw(10) << "Case ID" 
             << setw(25) << "Patient Name"
             << setw(20) << "Emergency Type"
             << setw(10) << "Priority"
             << setw(30) << "Arrival Time" << endl;
        cout << string(95, '-') << endl;
        
        for (int i = 0; i < currentSize; i++) {
            cout << left << setw(10) << tempArray[i].caseID
                 << setw(25) << tempArray[i].patientName
                 << setw(20) << tempArray[i].emergencyType
                 << setw(10) << tempArray[i].priorityLevel
                 << setw(30) << tempArray[i].arrivalTime << endl;
            
            if (!tempArray[i].additionalNotes.empty()) {
                cout << "  Notes: " << tempArray[i].additionalNotes << endl;
            }
        }
        cout << string(95, '=') << endl;
        cout << "Total Cases: " << currentSize << endl;
        
        delete[] tempArray;
    }
    
    // Get statistics about current emergency cases
    void displayStatistics() {
        if (isEmpty()) {
            cout << "\nNo statistics available - no emergency cases.\n";
            return;
        }
        
        int criticalCount = 0;     // Priority 8-10
        int urgentCount = 0;       // Priority 5-7
        int standardCount = 0;     // Priority 1-4
        
        for (int i = 0; i < currentSize; i++) {
            if (heap[i].priorityLevel >= 8) {
                criticalCount++;
            } else if (heap[i].priorityLevel >= 5) {
                urgentCount++;
            } else {
                standardCount++;
            }
        }
        
        cout << "\n======== EMERGENCY DEPARTMENT STATISTICS ========\n";
        cout << "Total Active Cases: " << currentSize << "/" << capacity << endl;
        cout << "Critical Cases (Priority 8-10): " << criticalCount << endl;
        cout << "Urgent Cases (Priority 5-7): " << urgentCount << endl;
        cout << "Standard Cases (Priority 1-4): " << standardCount << endl;
        cout << "Queue Utilization: " << (currentSize * 100.0 / capacity) << "%\n";
        cout << "=================================================\n";
    }
};

// Emergency Department Officer class to manage the system
class EmergencyDepartmentOfficer {
private:
    EmergencyPriorityQueue* priorityQueue;
    EmergencyCase* casesBeingProcessed;
    int nextCaseID;
    string officerName;
    string departmentCode;
    int processedCount; 
    int maxProcessedCapacity;  
    
public:
    // Constructor
    EmergencyDepartmentOfficer(string name = "Officer", string code = "ED001") {
        priorityQueue = new EmergencyPriorityQueue(20);  // Initial capacity of 20
        nextCaseID = 1001;  // Starting case ID
        officerName = name;
        departmentCode = code;

        maxProcessedCapacity = 20; 
        casesBeingProcessed = new EmergencyCase[maxProcessedCapacity];
        processedCount = 0;
    }
    
    // Destructor
    ~EmergencyDepartmentOfficer() {
        delete priorityQueue;
        delete[] casesBeingProcessed; 
    }
    
    //  Log Emergency Case
    void logEmergencyCase() {
    cout << "\n===== LOG NEW EMERGENCY CASE =====\n";
    
    string patientName, emergencyType, notes;
    int priority;
    
    cout << "Enter Patient Name: ";
    cin.ignore();
    do {
        getline(cin, patientName);
        patientName.erase(0, patientName.find_first_not_of(" \t\n\r"));
        patientName.erase(patientName.find_last_not_of(" \t\n\r") + 1);
        
        if (patientName.empty()) {
            cout << "Patient name cannot be empty! Please enter a valid name: ";
        }
    } while (patientName.empty());
    

    cout << "Enter Emergency Type (e.g., Cardiac, Trauma, Respiratory): ";
    do {
        getline(cin, emergencyType);
        emergencyType.erase(0, emergencyType.find_first_not_of(" \t\n\r"));
        emergencyType.erase(emergencyType.find_last_not_of(" \t\n\r") + 1);
        
        if (emergencyType.empty()) {
            cout << "Emergency type cannot be empty! Please enter a valid type: ";
        }
    } while (emergencyType.empty());
    
    cout << "Enter Priority Level (1-10, where 10 is most critical): ";
    while (!(cin >> priority) || priority < 1 || priority > 10) {
        cout << "Invalid input! Please enter a number between 1 and 10: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    
    // Additional Notes (optional - can be empty)
    cout << "Additional Notes (optional, press Enter to skip): ";
    cin.ignore();
    getline(cin, notes);
    
    // Create and insert the emergency case
    EmergencyCase newCase(nextCaseID++, patientName, emergencyType, priority, notes);
    priorityQueue->insertEmergencyCase(newCase);
    
    cout << "\nEmergency case logged successfully!\n";
    cout << "Case ID: " << newCase.caseID << endl;
    cout << "Patient Name: " << patientName << endl;
    cout << "Emergency Type: " << emergencyType << endl;
    cout << "Priority Level: " << newCase.priorityLevel << endl;
    
    if (priority >= 8) {
        cout << "\n[ALERT] CRITICAL CASE - Immediate attention required!\n";
    } else if (priority >= 5) {
        cout << "\n[URGENT] URGENT CASE - Urgent attention needed.\n";
    } else{
        cout << "\n[STANDARD] STANDARD CASE - Standard case logged.\n";
    }
}
    
    // Process Most Critical Case
    void processMostCriticalCase() {
        cout << "\n===== PROCESS MOST CRITICAL CASE =====\n";
        
        if (priorityQueue->isEmpty()) {
            cout << "No emergency cases to process.\n";
            return;
        }
        
        try {
            // Show the case that will be processed
            EmergencyCase criticalCase = priorityQueue->peekMostCritical();
            
            cout << "\nProcessing the following case:\n";
            cout << string(50, '-') << endl;
            cout << "Case ID: " << criticalCase.caseID << endl;
            cout << "Patient: " << criticalCase.patientName << endl;
            cout << "Emergency: " << criticalCase.emergencyType << endl;
            cout << "Priority: " << criticalCase.priorityLevel << endl;
            cout << "Arrival: " << criticalCase.arrivalTime << endl;
            if (!criticalCase.additionalNotes.empty()) {
                cout << "Notes: " << criticalCase.additionalNotes << endl;
            }
            cout << string(50, '-') << endl;
            
            cout << "\nConfirm processing this case? (Y/N): ";
            char confirm;
            cin >> confirm;
            
            if (confirm == 'Y' || confirm == 'y') {
                // Extract the case from queue
                EmergencyCase processedCase = priorityQueue->extractMostCritical();
                
                if (processedCount < maxProcessedCapacity) {
                    casesBeingProcessed[processedCount] = processedCase;
                    processedCount++;
                }
                cout << "\nCase #" << processedCase.caseID 
                     << " has been processed and removed from queue.\n";
                cout << "Patient " << processedCase.patientName 
                     << " is being attended to by medical staff.\n";
                
                // Show remaining cases count
                cout << "Remaining cases in queue: " << priorityQueue->getSize() << endl;
                cout << "Cases currently being processed: " << processedCount << endl;  

            } else {
                cout << "Case processing cancelled.\n";
            }
            
        } catch (const exception& e) {
            cout << "ERROR " << e.what() << endl;
        }
    }
    void viewCasesBeingProcessed() {
        cout << "\n===== CASES CURRENTLY BEING PROCESSED =====\n";
        
        if (processedCount == 0) {
            cout << "\nNo cases are currently being processed.\n";
            return;
        }
        int criticalCount = 0;     // Priority 8-10
        int urgentCount = 0;       // Priority 5-7
        int standardCount = 0;     // Priority 1-4
        
        for (int i = 0; i < processedCount; i++) {
            if (casesBeingProcessed[i].priorityLevel >= 8) {
                criticalCount++;
            } else if (casesBeingProcessed[i].priorityLevel >= 5) {
                urgentCount++;
            } else {
                standardCount++;
            }
        }
        
        cout << "\n========================================";
        cout << " CASES IN TREATMENT ";
        cout << "========================================\n";
        cout << left << setw(10) << "Case ID" 
             << setw(25) << "Patient Name"
             << setw(20) << "Emergency Type"
             << setw(10) << "Priority"
             << setw(30) << "Started Processing" << endl;
        cout << string(95, '-') << endl;
        
        for (int i = 0; i < processedCount; i++) {
            cout << left << setw(10) << casesBeingProcessed[i].caseID
                 << setw(25) << casesBeingProcessed[i].patientName
                 << setw(20) << casesBeingProcessed[i].emergencyType
                 << setw(10) << casesBeingProcessed[i].priorityLevel
                 << setw(30) << casesBeingProcessed[i].arrivalTime << endl;
            
            if (!casesBeingProcessed[i].additionalNotes.empty()) {
                cout << "  Notes: " << casesBeingProcessed[i].additionalNotes << endl;
            }
        }
        
        cout << string(95, '=') << endl;
        cout << "Total Cases Being Processed: " << processedCount << endl;
        cout << "Critical Cases (Priority 8-10): " << criticalCount << endl;
        cout << "Urgent Cases (Priority 5-7): " << urgentCount << endl;
        cout << "Standard Cases (Priority 1-4): " << standardCount << endl;
    }
    
    // View Pending Emergency Cases
    void viewPendingEmergencyCases() {
        cout << "\n===== VIEW PENDING EMERGENCY CASES =====\n";
        priorityQueue->displayAllCases();
        
        // Also show statistics
        priorityQueue->displayStatistics();
    }
        
    // Get current queue size 
    int getCurrentQueueSize() {
        return priorityQueue->getSize();
    }
    
    // Check if there are critical cases 
    bool hasCriticalCases() {
        if (priorityQueue->isEmpty()) return false;
        return priorityQueue->peekMostCritical().priorityLevel >= 8;
    }
    
    // Display officer information
    void displayOfficerInfo() {
        cout << "\n===== EMERGENCY DEPARTMENT OFFICER INFO =====\n";
        cout << "Officer Name: " << officerName << endl;
        cout << "Department Code: " << departmentCode << endl;
        cout << "Active Cases: " << priorityQueue->getSize() << endl;
        cout << "Cases Being Processed: " << processedCount << endl;
        cout << "Total Cases in System: " << (priorityQueue->getSize() + processedCount) << endl;
        cout << "============================================\n";
    }
    void menu();

};



#endif // EMERGENCY_DEPARTMENT_H