#include <iostream>
#include <iomanip>
#include <vector>
#include <regex>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>

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


// ===== Menu System =====
void displayMenu();
void choice(vector<EventRegistration>& reg);
void addRegistration(vector<EventRegistration>& reg);
void searchRegistrationByID(vector<EventRegistration>& reg);
void deleteRegistrationByID(vector<EventRegistration>& reg);
void rewriteToVector(vector<EventRegistration>& reg);
void updateRegistrationByID(vector<EventRegistration>& reg);
// ===== Validation Functions =====
void clearScreen();
void outputRegistration(const EventRegistration& reg);
EventRegistration inputRegistration();
string inputName(const string& prompt);
string inputPhone();
string inputEmail();
int inputGuests();


int main() {
    vector<EventRegistration> reg;
    while (true)
    {
        clearScreen();
        displayMenu();
        choice(reg);
    }

    return 0;
}

void displayMenu() {
    cout << "Event Registration System" << endl;
    cout << "1. Add New Registration" << endl;
    cout << "2. Search Registration" << endl;
    cout << "3. Delete Registration" << endl;
    cout << "4. Update Registration" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

void choice(vector<EventRegistration>& reg) {
    rewriteToVector(reg);
    int c;
    cin >> c;
    cin.ignore();
    switch (c) {
    case 1: addRegistration(reg); break;
    case 2: searchRegistrationByID(reg); break;
    case 3: deleteRegistrationByID(reg); break;
    case 4: updateRegistrationByID(reg); break;
    case 5: cout << "Exiting the program.\n"; exit(0);
    default: cout << "Please enter a valid number.\n";
    }
}

void addRegistration(vector<EventRegistration>& reg) {
    clearScreen();
    EventRegistration registration = inputRegistration();
    int newID = reg.size() + 1001;
    registration.registrationID = to_string(newID);
    reg.push_back(registration);

    ofstream outputFile("registrations.txt");
    for (const auto& r : reg) {
        outputFile << r.registrationID << ","
            << r.manName << ","
            << r.womanName << ","
            << r.phone << ","
            << r.email << ","
            << r.numberOfGuests << ","
            << r.packageType << ","
            << r.specialRequests << endl;
    }
    cout << "Registration added successfully with ID: " << registration.registrationID << endl;
}

void searchRegistrationByID(vector<EventRegistration>& reg) {
    clearScreen();
    string id,enter;
    cout << "Enter Registration ID to search: ";
    cin >> id;
    cin.ignore();
    for (const auto& r : reg) {
        if (id == r.registrationID) {
            cout << "Registration found!\n";
            outputRegistration(r);
			cout << "Press enter key to continue....";
			getline(cin, enter);
            return;
        }
    }
    cout << "No Registration found with this ID.\n";
    cout << "Press enter key to continue....";
    getline(cin, enter);
}

void deleteRegistrationByID(vector<EventRegistration>& reg) {
    clearScreen();

    string id,enter;
    cout << "Enter Registration ID to delete: ";
    cin >> id;
    cin.ignore();
    for (int i = 0; i < reg.size(); i++) {
        if (id == reg[i].registrationID) {
            cout << "Registration Found and Deleted:\n";
            outputRegistration(reg[i]);
			cout << "Press enter key to continue....";
			getline(cin, enter);
            reg.erase(reg.begin() + i);
            ofstream outputFile("registrations.txt");
            for (const auto& r : reg) {
                outputFile << r.registrationID << ","
                    << r.manName << ","
                    << r.womanName << ","
                    << r.phone << ","
                    << r.email << ","
                    << r.numberOfGuests << ","
                    << r.packageType << ","
                    << r.specialRequests << endl;
            }
            return;
        }
    }
    cout << "No Registration found with this ID.\n";
    cout << "Press enter key to continue....";
    getline(cin, enter);
}

void rewriteToVector(vector<EventRegistration>& reg) {
    ifstream inputFile("registrations.txt");

    if (!inputFile) {
        // 文件不存在
        cout << "File not found. Nothing to load." << endl;
        return;
    }

    if (inputFile.peek() == ifstream::traits_type::eof()) {
        // 文件存在但为空
        cout << "File is empty. Nothing to load." << endl;
        return;
    }

    reg.clear();
    string line;
    while (getline(inputFile, line)) {
        if (line.empty()) continue; // skip 空行

        EventRegistration r;
        stringstream ss(line);
        getline(ss, r.registrationID, ',');
        getline(ss, r.manName, ',');
        getline(ss, r.womanName, ',');
        getline(ss, r.phone, ',');
        getline(ss, r.email, ',');
        string guests;
        getline(ss, guests, ',');
        if (!guests.empty())
            r.numberOfGuests = stoi(guests);
        else
            r.numberOfGuests = 0;
        getline(ss, r.packageType, ',');
        getline(ss, r.specialRequests, ',');

        reg.push_back(r);
    }
}


void updateRegistrationByID(vector<EventRegistration>& reg) {
    clearScreen();

    string id, enter;
    cout << "Enter Registration ID to update: ";
    cin >> id;
    cin.ignore();

    for (auto& r : reg) {
        if (id == r.registrationID) {
            cout << "Registration Found:\n";
            outputRegistration(r);

            cout << "\nPress Enter to keep current value.\n";
            string input;

            cout << "Enter new 1st Couple Name (" << r.manName << "): ";
            getline(cin, input);
            if (!input.empty()) r.manName = inputName("Enter 1st Couple Name: ");

            cout << "Enter new 2nd Couple Name (" << r.womanName << "): ";
            getline(cin, input);
            if (!input.empty()) r.womanName = inputName("Enter 2nd Couple Name: ");

            cout << "Enter new Phone (" << r.phone << "): ";
            getline(cin, input);
            if (!input.empty()) r.phone = inputPhone();

            cout << "Enter new Email (" << r.email << "): ";
            getline(cin, input);
            if (!input.empty()) r.email = inputEmail();

            cout << "Enter new Number of Guests (" << r.numberOfGuests << "): ";
            getline(cin, input);
            if (!input.empty()) r.numberOfGuests = inputGuests();

            cout << "Enter new Package (" << r.packageType << "): ";
            getline(cin, input);
            if (!input.empty()) r.packageType = inputPackage();

            cout << "Enter new Special Requests (" << r.specialRequests << "): ";
            getline(cin, input);
            if (!input.empty()) r.specialRequests = input; 

            // 保存到文件
            ofstream outputFile("registrations.txt");
            for (const auto& x : reg) {
                outputFile << x.registrationID << ","
                    << x.manName << ","
                    << x.womanName << ","
                    << x.phone << ","
                    << x.email << ","
                    << x.numberOfGuests << ","
                    << x.packageType << ","
                    << x.specialRequests << endl;
            }
            cout << "Registration updated successfully.\n";
            return;
        }
    }

    cout << "No Registration found with this ID.\n";
    cout << "Press enter key to continue....";
    getline(cin, enter);
}

// ===== Validation Functions =====
string inputName(const string& prompt) {
    string name;
    regex pattern("^[A-Za-z ]+$");
    while (true) {
        cout << prompt;
        getline(cin, name);
        if (regex_match(name, pattern)) return name;
        cout << "Invalid name. Only letters and spaces allowed.\n";
    }
}

string inputPhone() {
    string phone;
    regex pattern("^[0-9]{8,15}$");
    while (true) {
        cout << "Enter phone number: ";
        getline(cin, phone);
        if (regex_match(phone, pattern)) return phone;
        cout << "Invalid phone number. Digits only (8–15).\n";
    }
}

string inputEmail() {
    string email;
    regex pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    while (true) {
        cout << "Enter email: ";
        getline(cin, email);
        if (regex_match(email, pattern)) return email;
        cout << "Invalid email format. Try again.\n";
    }
}

int inputGuests() {
    string input;
    int guests;
    regex pattern("^[0-9]+$");
    while (true) {
        cout << "Enter number of guests: ";
        getline(cin, input);
        if (regex_match(input, pattern)) {
            guests = stoi(input);
            if (guests > 0 && guests <= 1000) return guests;
        }
        cout << "Invalid number of guests. Must be between 1–1000.\n";
    }
}

string inputPackage() {
    string pkg;
    while (true) {
        cout << "Enter package type (Basic, Standard, Premium): ";
        getline(cin, pkg);
        if (pkg == "Basic" || pkg == "Standard" || pkg == "Premium") return pkg;
        cout << "Invalid package type. Must be Basic, Standard, or Premium.\n";
    }
}

string inputSpecialRequest() {
    string req;
    cout << "Enter any special requests (leave blank if none): ";
    getline(cin, req);
    if (req=="")
    {
        req = "None";
    }
    return req;
}

// ===== Input & Output Functions =====
EventRegistration inputRegistration() {
    EventRegistration reg;
    reg.manName = inputName("Enter 1st couple name (Male): ");
    reg.womanName = inputName("Enter 2nd couple name (Female): ");
    reg.phone = inputPhone();
    reg.email = inputEmail();
    reg.numberOfGuests = inputGuests();
    reg.packageType = inputPackage();
    reg.specialRequests = inputSpecialRequest();
    return reg;
}

void outputRegistration(const EventRegistration& reg) {
    cout << "----------------------------------------\n";
    cout << "Registration ID: " << reg.registrationID << endl;
    cout << "1st Couple Name: " << reg.manName << endl;
    cout << "2nd Couple Name: " << reg.womanName << endl;
    cout << "Phone Number: " << reg.phone << endl;
    cout << "Email: " << reg.email << endl;
    cout << "Number of Guests: " << reg.numberOfGuests << endl;
    cout << "Package Type: " << reg.packageType << endl;
    cout << "Special Requests: " << reg.specialRequests << endl;
    cout << "----------------------------------------\n";
}
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}
