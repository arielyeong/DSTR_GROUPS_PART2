#include "PatientAdmission.hpp"
#include <ctime>     // time, localtime, difftime
#include <iomanip>   // put_time, setw, setfill
#include <sstream>   // ostringstream

// ===========================================
// Helper: generate next Patient ID (P001, P002, ...)
// ===========================================
string PatientQueue::generateNextPatientId() {
    ostringstream oss;
    oss << 'P' << setw(3) << setfill('0') << nextPatientNumber++;
    return oss.str();
}

// ===========================================
// Constructor
// ===========================================
PatientQueue::PatientQueue() {
    front = nullptr;
    rear = nullptr;
    size = 0;
    nextPatientNumber = 1;   // Start ID from P001

    cout << "Patient Admission Manager initialized. "
         << "Starting with an empty queue." << endl;
}

// ===========================================
// Destructor
// ===========================================
PatientQueue::~PatientQueue() {
    PatientNode* current = front;
    while (current != nullptr) {
        PatientNode* temp = current;
        current = current->nextAddress;
        delete temp;
    }
    cout << "Patient queue destroyed and memory released." << endl;
}

// ===========================================
// Check empty
// ===========================================
bool PatientQueue::isEmpty() {
    return (front == nullptr);
}

// ==========================================================
// 1) Admit Patient (AUTO ID)
// ==========================================================
void PatientQueue::admitPatient() {
    PatientNode* newPatient = new PatientNode();

    // Auto-generate unique Patient ID like P001, P002, ...
    newPatient->patientID = generateNextPatientId();
    cout << "\nGenerated Patient ID: " << newPatient->patientID << endl;

    // Patient name
    cout << "Enter Patient Name (can include spaces): ";
    getline(cin, newPatient->name);
    while (newPatient->name.empty()) {
        cout << "Name cannot be empty. Enter name again: ";
        getline(cin, newPatient->name);
    }

    // Condition type
    int condChoice;
    cout << "Select Condition Type:\n";
    cout << "  1. Normal\n";
    cout << "  2. Critical\n";
    cout << "  3. Emergency\n";
    cout << "Enter choice (1-3): ";
    cin >> condChoice;

    while (condChoice < 1 || condChoice > 3) {
        cout << "Invalid choice. Enter 1-3: ";
        cin >> condChoice;
    }
    cin.ignore();

    switch (condChoice) {
        case 1: newPatient->conditionType = "Normal"; break;
        case 2: newPatient->conditionType = "Critical"; break;
        case 3: newPatient->conditionType = "Emergency"; break;
    }

    // Time recorded
    time_t now = time(0);
    newPatient->admittedTimeRaw = now;
    tm* localTime = localtime(&now);

    ostringstream oss;
    oss << put_time(localTime, "%Y-%m-%d %H:%M:%S");
    newPatient->admittedAt = oss.str();

    // Insert FIFO
    newPatient->nextAddress = nullptr;
    newPatient->prevAddress = nullptr;

    if (isEmpty()) {
        front = rear = newPatient;
    } else {
        rear->nextAddress = newPatient;
        newPatient->prevAddress = rear;
        rear = newPatient;
    }

    size++;

    cout << "\n*************************************************" << endl;
    cout << "*            NEW PATIENT ADMITTED                *" << endl;
    cout << "*************************************************" << endl;
    cout << "*  Patient ID : " << newPatient->patientID << endl;
    cout << "*  Name       : " << newPatient->name << endl;
    cout << "*  Condition  : " << newPatient->conditionType << endl;
    cout << "*  Admitted   : " << newPatient->admittedAt << endl;
    cout << "*************************************************" << endl;
}

// ==========================================================
// 2) Discharge (FIFO)
// ==========================================================
void PatientQueue::dischargePatient() {
    if (isEmpty()) {
        cout << "No patients to discharge." << endl;
        return;
    }

    cout << "\n===== BEFORE DISCHARGE =====" << endl;
    viewPatients();

    PatientNode* temp = front;

    // Waiting time
    time_t now = time(0);
    int minutesWaited = (int)difftime(now, temp->admittedTimeRaw) / 60;

    cout << "\n*************************************************" << endl;
    cout << "*              DISCHARGING PATIENT              *" << endl;
    cout << "*************************************************" << endl;
    cout << "*  Patient ID : " << temp->patientID << endl;
    cout << "*  Name       : " << temp->name << endl;
    cout << "*  Condition  : " << temp->conditionType << endl;
    cout << "*  Admitted   : " << temp->admittedAt << endl;
    cout << "*  Waiting    : " << minutesWaited << " minute(s)" << endl;
    cout << "*************************************************" << endl;

    // Remove the patient (FIFO)
    front = front->nextAddress;
    if (front != nullptr)
        front->prevAddress = nullptr;
    else
        rear = nullptr;

    delete temp;
    size--;

    cout << "\n===== AFTER DISCHARGE =====" << endl;
    viewPatients();
}

// ==========================================================
// 3) View All Patients
// ==========================================================
void PatientQueue::viewPatients() {
    if (isEmpty()) {
        cout << "No patients in queue." << endl;
        return;
    }

    cout << "\n================ CURRENT PATIENT QUEUE ================" << endl;

    PatientNode* current = front;
    int index = 1;

    int normal = 0, critical = 0, emergency = 0;

    time_t now = time(0);

    while (current != nullptr) {
        int minutesWaited = (int)difftime(now, current->admittedTimeRaw) / 60;

        cout << index << ") "
             << "ID: " << current->patientID
             << " | Name: " << current->name
             << " | Condition: " << current->conditionType
             << " | Admitted: " << current->admittedAt
             << " | Waiting: " << minutesWaited << " mins"
             << endl;

        if (current->conditionType == "Normal") normal++;
        if (current->conditionType == "Critical") critical++;
        if (current->conditionType == "Emergency") emergency++;

        current = current->nextAddress;
        index++;
    }

    cout << "------------------------------------------------------" << endl;
    cout << "Summary -> Normal: " << normal
         << ", Critical: " << critical
         << ", Emergency: " << emergency << endl;
}

// ==========================================================
// 4) Search Patient by ID
// ==========================================================
void PatientQueue::searchPatientById() {
    if (isEmpty()) {
        cout << "No patients to search." << endl;
        return;
    }

    string searchId;
    cout << "Enter Patient ID to search (e.g., P001): ";
    cin >> searchId;

    PatientNode* current = front;
    time_t now = time(0);

    while (current != nullptr) {
        if (current->patientID == searchId) {

            int minutesWaited = (int)difftime(now, current->admittedTimeRaw) / 60;

            cout << "\n*************************************************" << endl;
            cout << "*                 PATIENT FOUND                 *" << endl;
            cout << "*************************************************" << endl;
            cout << "*  Patient ID : " << current->patientID << endl;
            cout << "*  Name       : " << current->name << endl;
            cout << "*  Condition  : " << current->conditionType << endl;
            cout << "*  Admitted   : " << current->admittedAt << endl;
            cout << "*  Waiting    : " << minutesWaited << " minute(s)" << endl;
            cout << "*************************************************" << endl;

            return;
        }
        current = current->nextAddress;
    }

    cout << "No patient found with ID: " << searchId << endl;
}

// ==========================================================
// 5) Peek next (front) patient
// ==========================================================
void PatientQueue::peekNextPatient() {
    if (isEmpty()) {
        cout << "No patients to peek." << endl;
        return;
    }

    PatientNode* p = front;

    time_t now = time(0);
    int minutesWaited = (int)difftime(now, p->admittedTimeRaw) / 60;

    cout << "\n*************************************************" << endl;
    cout << "*             NEXT PATIENT (FRONT)              *" << endl;
    cout << "*************************************************" << endl;
    cout << "*  Patient ID : " << p->patientID << endl;
    cout << "*  Name       : " << p->name << endl;
    cout << "*  Condition  : " << p->conditionType << endl;
    cout << "*  Admitted   : " << p->admittedAt << endl;
    cout << "*  Waiting    : " << minutesWaited << " minute(s)" << endl;
    cout << "*************************************************" << endl;
}

// ==========================================================
// MENU
// ==========================================================
void PatientAdmission::menu() {
    PatientQueue patientQueue;

    int choice = 0;
    do {
        cout << "\n========================================================" << endl;
        cout << "            PATIENT ADMISSION MANAGEMENT MENU           " << endl;
        cout << "========================================================" << endl;
        cout << "  [1] Admit New Patient" << endl;
        cout << "  [2] Discharge Patient (FIFO)" << endl;
        cout << "  [3] View All Patients" << endl;
        cout << "  [4] Search Patient by ID" << endl;
        cout << "  [5] Peek Next Patient (Front)" << endl;
        cout << "  [6] Back to Main Menu" << endl;
        cout << "========================================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: patientQueue.admitPatient(); break;
            case 2: patientQueue.dischargePatient(); break;
            case 3: patientQueue.viewPatients(); break;
            case 4: patientQueue.searchPatientById(); break;
            case 5: patientQueue.peekNextPatient(); break;
            case 6: 
                cout << "Returning to Main Menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }

    } while (choice != 6);
}
