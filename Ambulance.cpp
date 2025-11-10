#include "Ambulance.hpp"

AmbulanceQueue::AmbulanceQueue()
{
    front = 0;
    rear = -1;
    count = 0;
}

// Ambulance menu (3 function)
void AmbulanceQueue::menu()
{
    int choice;
    do
    {
        cout << "\n========== Ambulance Dispatch Menu ==========" << endl;
        cout << "1. Register Ambulance" << endl;
        cout << "2. Rotate Ambulance Shift" << endl;
        cout << "3. Display Ambulance Schedule" << endl;
        cout << "0. Back to Main Menu\n"
             << endl;
        cout << "=============================================" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            registerAmbulance();
            break;
        case 2:
            rotateShift();
            break;
        case 3:
            displaySchedule();
            break;
        case 0:
            cout << "Returning to Main Menu..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}

bool AmbulanceQueue::isFull()
{
    return count == MAX;
}

bool AmbulanceQueue::isEmpty()
{
    return count == 0;
}

void AmbulanceQueue::registerAmbulance()
{
    if (isFull())
    {
        cout << "Ambulance queue is full. Cannot register more ambulances." << endl;
        return;
    }

    Ambulance newAmbulance;
    cout << "Enter Ambulance ID: ";
    cin >> newAmbulance.id;
    cout << "Enter Driver Name: ";
    cin.ignore();
    getline(cin, newAmbulance.driverName);
    newAmbulance.status = "Available";

    rear = (rear + 1) % MAX;
    queue[rear] = newAmbulance;
    count++;

    cout << "Ambulance registered successfully!" << endl;
}

void AmbulanceQueue::rotateShift()
{
    if (isEmpty())
    {
        cout << "No ambulances to rotate." << endl;
        return;
    }

    // move the front ambulance to the end of the queue
    Ambulance temp = queue[front];
    front = (front + 1) % MAX;
    rear = (rear + 1) % MAX;
    queue[rear] = temp;

    cout << "Ambulance shift rotated successfully!\n";
}

void AmbulanceQueue::displaySchedule()
{
    if (isEmpty())
    {
        cout << "No ambulances in the schedule." << endl;
        return;
    }

    cout << "\nCurrent Ambulance Schedule:" << endl;
    cout << "========================================" << endl;
    cout << "ID\tDriver Name\tStatus" << endl;
    cout << "========================================" << endl;

    for (int i = 0; i < count; i++)
    {
        int index = (front + i) % MAX;
        cout << queue[index].id << "\t" << queue[index].driverName
             << "\t\t" << queue[index].status << endl;
    }
    cout << "========================================" << endl;
}