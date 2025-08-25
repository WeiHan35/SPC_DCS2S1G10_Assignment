#include <iostream>
#include <iomanip>
#include <format>
#include <vector>
#include <regex>
#include <sstream>
#include <algorithm>
#include<string>
#include<cstring>
#include<fstream>
#include <map>


using namespace std;

struct EventRegistration {
    string registrationID;
    string manName;
    string womanName;
    string phone;
    string email;
    int numberOfGuests;
    string packageType;
    string specialRequests;
};
int main() {

    vector<EventRegistration> registrations;

    ifstream inputFile("registrations.txt");
    if (!inputFile) {
        cerr << "Error opening file." << endl;
        return 1;
    }
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        EventRegistration reg;
        getline(ss, reg.registrationID, ',');
        getline(ss, reg.manName, ',');
        getline(ss, reg.womanName, ',');
        getline(ss, reg.phone, ',');
        getline(ss, reg.email, ',');
        string guestsStr;
        getline(ss, guestsStr, ',');
        reg.numberOfGuests = stoi(guestsStr);
        getline(ss, reg.packageType, ',');
        getline(ss, reg.specialRequests, ',');
        registrations.push_back(reg);

    }
    inputFile.close();
    cout << left << setw(15) << "Reg ID"
        << setw;



	return 0;

}

void displayMenu() {
    cout << "Event Registration System" << endl;
    cout << "1. View All Registrations" << endl;
    cout << "2. Search Registration by ID" << endl;
    cout << "3. Add New Registration" << endl;
    cout << "4. Update Registration" << endl;
    cout << "5. Delete Registration" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
}

void choice() {
    int choice;
    cin >> choice;
    switch (choice)
    {
    case 1:


    case 2:

    case 3:
    case 4:


    default:
        break;
    }
}

void addRegisteration() {
    vector<EventRegistration> registrations;


}
