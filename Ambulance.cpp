#include "Ambulance.hpp"

AmbulanceQueue::AmbulanceQueue()
{
    front = nullptr;
    rear = nullptr;
}

AmbulanceQueue::~AmbulanceQueue()
{
    if (front == nullptr)
        return;

    Ambulance *temp = front;
    do
    {
        Ambulance *del = temp;
        temp = temp->next;
        delete del;
    } while (temp != front);
}

bool AmbulanceQueue::isEmpty()
{
    return front == nullptr;
}

void AmbulanceQueue::registerAmbulance()
{
    Ambulance *newNode = new Ambulance;

    cout << "Enter Ambulance ID: ";
    cin >> newNode->id;
    cin.ignore();

    cout << "Enter Driver Name: ";
    getline(cin, newNode->driverName);

    newNode->status = "Available";
    newNode->next = nullptr;

    // first ambulance
    if (isEmpty())
    {
        front = rear = newNode;
        rear->next = front; // make circular
    }
    else
    {
        rear->next = newNode;
        rear = newNode;
        rear->next = front;
    }

    cout << "Ambulance registered successfully!\n";
}

void AmbulanceQueue::rotateShift()
{
    if (isEmpty())
    {
        cout << "No ambulances to rotate.\n";
        return;
    }

    // Moving front ambulance to the back in circular list
    front = front->next;
    rear = rear->next;

    cout << "Ambulance shift rotated successfully!\n";
}

void AmbulanceQueue::displaySchedule()
{
    if (isEmpty())
    {
        cout << "No ambulances in the schedule.\n";
        return;
    }

    cout << "\nCurrent Ambulance Schedule:\n";
    cout << "========================================\n";
    cout << "ID\tDriver Name\tStatus\n";
    cout << "========================================\n";

    Ambulance *temp = front;

    do
    {
        cout << temp->id << "\t" << temp->driverName
             << "\t\t" << temp->status << endl;

        temp = temp->next;

    } while (temp != front);

    cout << "========================================\n";
}

void AmbulanceQueue::menu()
{
    int choice;

    do
    {
        cout << "\n========== Ambulance Dispatch Menu ==========\n";
        cout << "1. Register Ambulance\n";
        cout << "2. Rotate Ambulance Shift\n";
        cout << "3. Display Ambulance Schedule\n";
        cout << "0. Back to Main Menu\n";
        cout << "=============================================\n";
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
            cout << "Returning to Main Menu...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}