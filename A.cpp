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

void displayMenu();
void choice();
void addRegisteration();

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

	displayMenu();
	choice();





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
		addRegisteration();
		break;
    case 2:
        break;
    case 3:
		break;
    case 4:
		break;


    default:
		cout << "Please enter a valid number." << endl;

    }
}

void addRegisteration() {
	EventRegistration registeration;
    vector<EventRegistration> reg;
    ifstream inputFile("registrations.txt");
	ofstream outputFile("registrations.txt");
    if (!inputFile) {
        cout << "Error opening file." << endl;
        return ; 
    }
	cin.ignore(); 
    cout << "Enter 1st couple name(Male):";
	getline(cin, registeration.manName);
	cout << "Enter 2nd couple name:(Female)";
	getline(cin, registeration.womanName);
	cout << "Enter phone number:";
	getline(cin, registeration.phone);
	cout << "Enter email:";
	getline(cin, registeration.email);
	cout << "Enter number of guests:";
	cin >> registeration.numberOfGuests;
	cout << "Enter package type (Basic, Standard, Premium):";
	cin >> registeration.packageType;
	cout << "Enter any special requests:";
	cin.ignore(); // To ignore the newline character left in the buffer
	getline(cin, registeration.specialRequests);
	// Generate a unique registration ID
    





    registeration.registrationID = "REG" + to_string(reg.size() + 1);
    // Display the registration details
    cout << "Registration Details:" << endl;
    cout << "Registration ID: " << registeration.registrationID << endl;
    cout << "Couple Names: " << registeration.manName << " and " << registeration.womanName << endl;
    cout << "Phone: " << registeration.phone << endl;
    cout << "Email: " << registeration.email << endl;
    cout << "Number of Guests: " << registeration.numberOfGuests << endl;
    cout << "Package Type: " << registeration.packageType << endl;
	cout << "Special Requests: " << registeration.specialRequests << endl;
	reg.push_back(registeration);
	// Save the new registration to the file
	//check if file is empty
	//another way to check if file is empty


    if (inputFile.peek() == ifstream::traits_type::eof()) {
        outputFile << "RegistrationID,ManName,WomanName,Phone,Email,NumberOfGuests,PackageType,SpecialRequests" << endl;
	}
    //check if file is not empty
    else {
        string line;
        while (getline(inputFile, line)) {
            outputFile << line << endl;
        }
	}
	outputFile << registeration.registrationID << "," << registeration.manName << "," <<
        registeration.womanName << "," << registeration.phone << "," << registeration.email << "," <<
        registeration.numberOfGuests << "," << registeration.packageType << "," << registeration.specialRequests << endl;

    inputFile.close();

}
