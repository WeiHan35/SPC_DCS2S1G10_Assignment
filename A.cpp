#include <iomanip>
#include <vector>
#include <regex>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <limits>  
using namespace std;
struct Payment {
	string method;
	bool paid;
	double amount;
};
struct EventRegistration {
	string registrationID;
	string manName;
	string womanName;
	string phone;
	string email;
	int numberOfGuests;
	string packageType;
	string specialRequests;
	Payment paymentInfo;
};
struct Booking {
	string registrationID; // Add this line
	int date;
	int venue;
	int slot;
	string decoTheme;
	double decoCost;
	string vendorOption;
	int vendorCost;
};


// ===== Menu System =====
void displayMenu();
void displayRegistrationMenu();
void registrationChoice(vector<EventRegistration>& reg);
void addRegistration(vector<EventRegistration>& reg);
void searchRegistration(vector<EventRegistration>& registrations);
void deleteRegistration(vector<EventRegistration>& registrations);
void rewriteToVector(vector<EventRegistration>& reg);
void updateRegistrationByID(vector<EventRegistration>& reg);
bool confirmAction(const string& prompt);
// ===== Validation Functions =====
void clearScreen();
void outputRegistration(const EventRegistration& reg);
EventRegistration inputRegistration();
string inputName(const string& prompt);
string inputPhone();
string inputEmail();
int inputGuests();
string inputPackage();
void continuefunc();
//booking related functions
void showAllBookings(const vector<Booking>& bookings,
	const vector<string>& venueNames,
	const vector<string>& timeSlots);
void booking(vector<EventRegistration>& reg,
	vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus,
	const vector<string>& venueNames,
	const vector<string>& timeSlots,
	int MAX_DATES);
string inputSpecialRequest();
void clearInput();
void displayVenues(const vector<string>& venueNames);
void displayTimeSlots(const vector<string>& timeSlots);
void displayDecorationThemes();
void makeBooking(vector<EventRegistration>& reg,
	vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus,
	const vector<string>& venueNames,
	const vector<string>& timeSlots,
	int MAX_DATES);

void showAvailableSlots(const vector<vector<vector<int>>>& bookingStatus,
	const vector<string>& venueNames,
	const vector<string>& timeSlots,
	int date, int MAX_DATES);
void saveBookingsToFile(const vector<Booking>& bookings);
void loadBookingsFromFile(vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus);
void processPayment(vector<EventRegistration>& reg, EventRegistration& currentReg, double totalAmountOfLogistic, int& cashCounter);
void checkPaymentStatusByID(vector<EventRegistration>& reg, vector<Booking>& bookings);
void printReceipt(const EventRegistration& reg, const Booking* booking);
string getVenueName(int venueIndex);
string getTimeSlotName(int slotIndex);
double getPackageCost(const string& packageType);
string checkPaid(bool paid);
void saveRegistrationsToFile(const vector<EventRegistration>& reg);
int getValidatedChoice(int min, int max, const string& prompt);
string getRegistrationIdInput();
EventRegistration* findRegistrationById(vector<EventRegistration>& reg, const string& id);
string generateNewRegistrationId(const vector<EventRegistration>& reg);
void handleRegistrationMenu(vector<EventRegistration>& registrations);
void handleBookingMenu(vector<EventRegistration>& registrations,
	vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus);
void handlePaymentStatusCheck(vector<EventRegistration>& registrations,
	vector<Booking>& bookings);
void displayMainMenu();
void displayRegistrationMenu();
void displayBookingMenu();
void displayRegistrationSummary(const EventRegistration& reg);
string inputValidatedString(const string& prompt, const regex& pattern, const string& errorMsg);
void loadRegistrationsFromFile(vector<EventRegistration>& registrations);
EventRegistration parseRegistrationFromLine(const string& line);
bool confirmAction(const string& prompt);
const Booking* findBookingByRegistrationId(const vector<Booking>& bookings, const string& id);
EventRegistration* findRegistrationById(vector<EventRegistration>& reg, const string& id);
int main() {
    vector<EventRegistration> reg;
    const int MAX_DATES = 31;
    const int MAX_VENUES = 5;
    const int MAX_SLOTS = 3;

    vector<string> timeSlots = { "12pm-3pm", "4pm-7pm", "8pm-11pm" };
    vector<string> venueNames = { "Hall A", "Hall B", "Hall C", "Outdoor", "VIP Lounge" };
	// Package costs
	const double SURPRISE_PROPOSAL_COST = 1200.0;
	const double ROMANTIC_DINNER_COST = 1500.0;
	const double FAMILY_GATHERING_COST = 1500.0;

	// Decoration costs
	const double CLASSIC_DECO_COST = 100.0;
	const double RUSTIC_DECO_COST = 150.0;
	const double ELEGANT_DECO_COST = 200.0;
	const double VENDOR_COST = 300.0;
    // bookingStatus[date][venue][slot]
    vector<vector<vector<int>>> bookingStatus(MAX_DATES + 1, vector<vector<int>>(MAX_VENUES, vector<int>(MAX_SLOTS, 0)));

    vector<Booking> bookings;
    int mainChoice;
    rewriteToVector(reg); // Load existing registrations from file
	loadBookingsFromFile(bookings, bookingStatus); // Load existing bookings from file

	while (true) {
		clearScreen();
		displayMainMenu();

		int choice = getValidatedChoice(0, 3, "Enter your choice: ");

		switch (choice) {
		case 1:
			handleRegistrationMenu(reg);
			break;
		case 2:
			handleBookingMenu(reg, bookings, bookingStatus);
			break;
		case 3:
			handlePaymentStatusCheck(reg, bookings);
			break;
		case 0:
			cout << "Thank you for using Event Management System. Goodbye!\n";
			return 0;
		}
	}

    return 0;
}
string generateNewRegistrationId(const vector<EventRegistration>& reg) {
	return to_string(reg.size() + 1000);
}
void handleRegistrationMenu(vector<EventRegistration>& registrations) {
	while (true) {
		clearScreen();
		displayRegistrationMenu();

		int choice = getValidatedChoice(0, 4, "Enter your choice: ");

		switch (choice) {
		case 1: addRegistration(registrations); break;
		case 2: searchRegistration(registrations); break;
		case 3: deleteRegistration(registrations); break;
		case 4: updateRegistrationByID(registrations); break;
		case 0: return; // Back to main menu
		}
	}
}
void loadRegistrationsFromFile(vector<EventRegistration>& registrations) {
	ifstream file("registrations.txt");
	if (!file.is_open()) return;

	registrations.clear();
	string line;

	while (getline(file, line)) {
		if (line.empty()) continue;

		EventRegistration reg = parseRegistrationFromLine(line);
		registrations.push_back(reg);
	}
	file.close();
}

EventRegistration parseRegistrationFromLine(const string& line) {
	EventRegistration reg;
	stringstream ss(line);

	getline(ss, reg.registrationID, ',');
	getline(ss, reg.manName, ',');
	getline(ss, reg.womanName, ',');
	getline(ss, reg.phone, ',');
	getline(ss, reg.email, ',');

	string temp;
	getline(ss, temp, ',');
	reg.numberOfGuests = temp.empty() ? 0 : stoi(temp);

	getline(ss, reg.packageType, ',');
	getline(ss, reg.specialRequests, ',');
	getline(ss, reg.paymentInfo.method, ',');

	getline(ss, temp, ',');
	reg.paymentInfo.paid = (temp == "Paid");

	getline(ss, temp, ',');
	reg.paymentInfo.amount = temp.empty() ? 0.0 : stod(temp);

	return reg;
}
bool confirmAction(const string& message) {
	char choice;
	cout << message << " (y/n): ";
	cin >> choice;
	cin.ignore();
	return (tolower(choice) == 'y');
}

const Booking* findBookingByRegistrationId(const vector<Booking>& bookings, const string& id) {
	for (const auto& booking : bookings) {
		if (booking.registrationID == id) {
			return &booking;
		}
	}
	return nullptr;
}

EventRegistration createNewRegistration() {
	EventRegistration reg;

	cout << "Enter registration details:\n";
	reg.manName = inputName("First partner name: ");
	reg.womanName = inputName("Second partner name: ");
	reg.phone = inputPhone();
	reg.email = inputEmail();
	reg.numberOfGuests = inputGuests();
	reg.packageType = inputPackage();

	cout << "Special requests (optional): ";
	cin.ignore();
	getline(cin, reg.specialRequests);
	if (reg.specialRequests.empty()) {
		reg.specialRequests = "None";
	}

	// Initialize payment info
	double packageCost = getPackageCost(reg.packageType);
	reg.paymentInfo = Payment{ "", false, packageCost };

	return reg;
}
void handleBookingMenu(vector<EventRegistration>& registrations,
	vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus) {
	while (true) {
		clearScreen();
		displayBookingMenu();

		int choice = getValidatedChoice(0, 2, "Enter your choice: ");

		switch (choice) {


				case 1: booking(registrations, bookings, bookingStatus,
			{ "Hall A", "Hall B", "Hall C", "Outdoor", "VIP Lounge" },
					{ "12pm-3pm", "4pm-7pm", "8pm-11pm" }, 31); break;
				case 2: showAllBookings(bookings,
					{ "Hall A", "Hall B", "Hall C", "Outdoor", "VIP Lounge" },
					{ "12pm-3pm", "4pm-7pm", "8pm-11pm" }); break;

		case 0: return; // Back to main menu
		}
	}
}

void handlePaymentStatusCheck(vector<EventRegistration>& registrations,
	vector<Booking>& bookings) {
	clearScreen();
	cout << "=== Payment Status Check ===\n\n";

	string id = getRegistrationIdInput();
	EventRegistration* reg = findRegistrationById(registrations, id);

	if (reg) {
		const Booking* booking = findBookingByRegistrationId(bookings, id);
		printReceipt(*reg, booking);
	}
	else {
		cout << "No registration found with ID: " << id << "\n";
	}
	continuefunc();
}
void checkPaymentStatusByID(vector<EventRegistration>& reg, vector<Booking>& bookings) {
    clearScreen();
    string id;
    cout << "Enter Registration ID to check payment status: ";
    cin >> id;
    cin.ignore();
    
    for (const auto& r : reg) {
        if (id == r.registrationID) {
            // Find associated booking
            const Booking* associatedBooking = nullptr;
            for (const auto& b : bookings) {
                if (b.registrationID == r.registrationID) {
                    associatedBooking = &b;
                    break;
                }
            }
            
            printReceipt(r, associatedBooking);
            cout << "\nPress Enter to continue...";
            cin.get();
            return;
        }
    }
    
    cout << "No Registration found with ID: " << id << "\n";
    cout << "Press Enter to continue...";
    cin.get();
}
void displayMenu() {
	cout << "=============================\n";
	cout << " Event Management System\n";
	cout << "=============================\n";
	cout << "1. Event Registration\n";
	cout << "2. Event booking on dates and logistic\n";
	cout << "3. Check payment status by ID\n";
	cout << "0. Exit\n";
	cout << "Enter your choice: ";
}

void saveRegistrationsToFile(const vector<EventRegistration>& reg) {
	ofstream outputFile("registrations.txt");
	for (const auto& r : reg) {
		string paid = checkPaid(r.paymentInfo.paid);
		outputFile << r.registrationID << ","
			<< r.manName << ","
			<< r.womanName << ","
			<< r.phone << ","
			<< r.email << ","
			<< r.numberOfGuests << ","
			<< r.packageType << ","
			<< r.specialRequests << ","
			<< r.paymentInfo.method << ","
			<< paid << ","
			<< r.paymentInfo.amount << endl;
	}
	outputFile.close();
}
//choice function modified to include booking parameters
void registrationChoice(vector<EventRegistration>& reg) {
	int choice;
	cin >> choice;
	cin.ignore(); // clear newline from input buffer
	rewriteToVector(reg); // load existing registrations from file
	switch (choice) {
	case 1:
		addRegistration(reg);
		break;
	case 2:
		searchRegistration(reg);
		break;
	case 3:
		deleteRegistration(reg);
		break;
	case 4:
		updateRegistrationByID(reg);
		break;
	case 5:
		cout << "Exiting program." << endl;
		exit(0);
	default:
		cout << "Invalid choice. Try again." << endl;
		return;
	}
}


void addRegistration(vector<EventRegistration>& registrations) {
	clearScreen();
	cout << "=== Add New Registration ===\n\n";
	cin.ignore();
	EventRegistration newReg = inputRegistration();
	newReg.registrationID = generateNewRegistrationId(registrations);

	registrations.push_back(newReg);
	saveRegistrationsToFile(registrations);

	cout << "\n✓ Registration successful! ID: " << newReg.registrationID << "\n";
	displayRegistrationSummary(newReg);
	continuefunc();
}

void displayMainMenu() {
	cout << "========================================\n";
	cout << "       Event Management System\n";
	cout << "========================================\n";
	cout << "1. Registration Management\n";
	cout << "2. Booking & Logistics\n";
	cout << "3. Check Payment Status\n";
	cout << "0. Exit\n";
	cout << "========================================\n";
}

void displayRegistrationMenu() {
	cout << "========================================\n";
	cout << "       Registration Management\n";
	cout << "========================================\n";
	cout << "1. Add New Registration\n";
	cout << "2. Search Registration\n";
	cout << "3. Delete Registration\n";
	cout << "4. Update Registration\n";
	cout << "0. Back to Main Menu\n";
	cout << "========================================\n";
}

void displayBookingMenu() {
	cout << "========================================\n";
	cout << "        Booking & Logistics\n";
	cout << "========================================\n";
	cout << "1. Create New Booking\n";
	cout << "2. View All Bookings\n";
	cout << "0. Back to Main Menu\n";
	cout << "========================================\n";
}

void displayRegistrationSummary(const EventRegistration& reg) {
	const int WIDTH = 40;
	cout << string(WIDTH, '=') << "\n";
	cout << "         Registration Summary\n";
	cout << string(WIDTH, '=') << "\n";
	cout << left << setw(18) << "ID:" << reg.registrationID << "\n";
	cout << left << setw(18) << "Couple:" << reg.manName << " & " << reg.womanName << "\n";
	cout << left << setw(18) << "Phone:" << reg.phone << "\n";
	cout << left << setw(18) << "Email:" << reg.email << "\n";
	cout << left << setw(18) << "Guests:" << reg.numberOfGuests << "\n";
	cout << left << setw(18) << "Package:" << reg.packageType << "\n";
	cout << left << setw(18) << "Requests:" << reg.specialRequests << "\n";
	cout << string(WIDTH, '=') << "\n";
}
void searchRegistration(vector<EventRegistration>& registrations) {
	clearScreen();
	cout << "=== Search Registration ===\n\n";

	string id = getRegistrationIdInput();
	EventRegistration* reg = findRegistrationById(registrations, id);

	if (reg) {
		cout << "✓ Registration found!\n";
		displayRegistrationSummary(*reg);
	}
	else {
		cout << "No registration found with ID: " << id << "\n";
	}
	continuefunc();
}

void deleteRegistration(vector<EventRegistration>& registrations) {
	clearScreen();
	cout << "=== Delete Registration ===\n\n";

	string id = getRegistrationIdInput();

	for (auto it = registrations.begin(); it != registrations.end(); ++it) {
		if (it->registrationID == id) {
			cout << "Registration to be deleted:\n";
			displayRegistrationSummary(*it);

			if (confirmAction("Are you sure you want to delete this registration?")) {
				registrations.erase(it);
				saveRegistrationsToFile(registrations);
				cout << "✓ Registration deleted successfully!\n";
			}
			else {
				cout << "✗ Deletion cancelled.\n";
			}
			continuefunc();
			return;
		}
	}

	cout << "No registration found with ID: " << id << "\n";
	continuefunc();
}


void rewriteToVector(vector<EventRegistration>& reg) {
	ifstream inputFile("registrations.txt");

	// If file doesn't exist, nothing to load
	if (!inputFile) return;
	// Clear existing data
	//if file empty, nothing to load
	if (inputFile.peek() == ifstream::traits_type::eof()) {
		inputFile.close();
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
		getline(ss, r.paymentInfo.method, ',');
		// write the paymen into the vector from file
		string paidStr;
		getline(ss, paidStr, ',');
		if (!paidStr.empty())
			r.paymentInfo.paid = (paidStr == "Paid"); // Check for "Paid" string instead
		else
			r.paymentInfo.paid = false;

		string amountStr;
		getline(ss, amountStr, ',');
		if (!amountStr.empty())
			r.paymentInfo.amount = stod(amountStr);
		else
			r.paymentInfo.amount = 0.0;



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
			if (!input.empty()) { r.packageType = inputPackage(); }

			cout << "Enter new Special Requests (" << r.specialRequests << "): ";
			getline(cin, input);
			if (!input.empty()) r.specialRequests = input;
			string paid = checkPaid(r.paymentInfo.paid);
			// 保存到文件
			saveRegistrationsToFile(reg); // Use the existing function instead
			cout << "Registration updated successfully.\n";
			return;
		}
	}

	cout << "No Registration found with this ID.\n";
	cout << "Press enter key to continue....";
	getline(cin, enter);
}
string getRegistrationIdInput() {
	string id;
	cout << "Enter Registration ID: ";
	cin >> id;
	cin.ignore();
	return id;
}
EventRegistration* findRegistrationById(vector<EventRegistration>& reg, const string& id) {
	for (auto& r : reg) {
		if (r.registrationID == id) {
			return &r;
		}
	}
	return nullptr;
}
int getValidatedChoice(int min, int max, const string& prompt) {
	int choice;
	while (true) {
		cout << prompt;
		cin >> choice;
		if (cin.fail() || choice < min || choice > max) {
			cout << "Invalid choice! Please enter a number between " << min << " and " << max << ".\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		return choice;
	}
}
string checkPaid(bool paid) {
	if (paid) {
		return "Paid";
	}
	return "Pending Payment";
}

// ===== Validation Functions =====
string inputValidatedString(const string& prompt, const regex& pattern, const string& errorMsg) {
	string input;
	while (true) {
		cout << prompt;
		getline(cin, input);
		if (regex_match(input, pattern)) {
			return input;
		}
		cout << errorMsg << "\n";
	}
}

string inputName(const string& prompt) {
	return inputValidatedString(prompt,
		regex("^[A-Za-z ]+$"),
		"Invalid name. Only letters and spaces allowed.");
}

string inputPhone() {
	return inputValidatedString("Enter phone number: ",
		regex("^[0-9]{8,15}$"),
		"Invalid phone number. Must be 8-15 digits.");
}

string inputEmail() {
	return inputValidatedString("Enter email: ",
		regex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)"),
		"Invalid email format.");
}

int inputGuests() {
	int guests;
	while (true) {
		cout << "Enter number of guests (1-1000): ";
		cin >> guests;
		if (cin.fail() || guests < 1 || guests > 1000) {
			cout << "Invalid number. Must be between 1 and 1000.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		return guests;
	}
}

string inputPackage() {
	cout << "\nPackage Options:\n";
	cout << "1. Surprise Proposal (RM 1200   )\n";
	cout << "2. Romantic Dinner (RM 1500   )\n";
	cout << "3. Family Gathering (RM 1500)\n";

	int choice = getValidatedChoice(1, 3, "Select package: ");

	switch (choice) {
	case 1: return "Surprise Proposal";
	case 2: return "Romantic Dinner";
	case 3: return "Family Gathering";
	default: return "Surprise Proposal"; // fallback
	}
}

string inputSpecialRequest() {
	string req;
	cin.ignore();
	cout << "Enter any special requests (leave blank if none): ";
	getline(cin, req);
	if (req == "")
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
	double pkgcost = 0.0;
	if (reg.packageType == "Family Gathering")
	{
		pkgcost = 1500.0;
	}
	else if (reg.packageType == "Romantic Dinner")
	{
		pkgcost = 1500.0;
	}
	else if (reg.packageType == "Surprise Proposal")
	{
		pkgcost = 1200.0;
	}
	reg.paymentInfo = Payment{ "", false, pkgcost }; // initialize payment info with package cost
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
	system("cls"); // For Windows
}


void booking(vector<EventRegistration>& reg, vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus,
	const vector<string>& venueNames,
	const vector<string>& timeSlots,
	int MAX_DATES) {

	int choice;
	do {
		clearScreen();
		cout << "\n==== Event Booking & Logistics System ====\n";
		cout << "1. Make a Booking\n";
		cout << "2. View All Bookings\n";
		cout << "0. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;

		if (cin.fail()) {
			cout << "Invalid input! Please enter a number.\n";
			clearInput();
			continue;
		}

		switch (choice) {
		case 1:
			makeBooking(reg, bookings, bookingStatus, venueNames, timeSlots, MAX_DATES);
			break;
		case 2:
			showAllBookings(bookings, venueNames, timeSlots);
			break;
		case 0:
			cout << "Goodbye!\n";
			break;
		default:
			cout << "Invalid choice.\n";
		}

	} while (choice != 0);
}
void clearInput() {
	cin.clear();
	cin.ignore();
}

void displayVenues(const vector<string>& venueNames) {
	cout << "Available Venues:\n";
	for (int i = 0; i < (int)venueNames.size(); i++) {
		cout << i + 1 << ". " << venueNames[i] << endl;
	}
}

void displayTimeSlots(const vector<string>& timeSlots) {
	cout << "Available Time Slots:\n";
	for (int i = 0; i < (int)timeSlots.size(); i++) {
		cout << i + 1 << ". " << timeSlots[i] << endl;
	}
}

void displayDecorationThemes() {
	cout << "Decoration Themes:\n";
	cout << "1. Classic (RM 100)\n";
	cout << "2. Rustic  (RM 150)\n";
	cout << "3. Elegant (RM 200)\n";
}

// Save bookings to file
void saveBookingsToFile(const vector<Booking>& bookings) {
	ofstream outFile("bookings.txt");
	for (const Booking& b : bookings) {
		outFile << b.registrationID << ","  // Add this line
			<< b.date << ","
			<< b.venue << ","
			<< b.slot << ","
			<< b.decoTheme << ","
			<< b.decoCost << ","
			<< b.vendorOption << ","
			<< b.vendorCost << "\n";
	}
	outFile.close();
}

// Load bookings from file
void loadBookingsFromFile(vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus) {
	ifstream inFile("bookings.txt");
	if (!inFile) return;

	// Add this line to clear existing bookings first
	bookings.clear();
	// Also clear the booking status
	for (auto& dateSlots : bookingStatus) {
		for (auto& venueSlots : dateSlots) {
			fill(venueSlots.begin(), venueSlots.end(), 0);
		}
	}

	Booking b;
	char comma;
	while (getline(inFile, b.registrationID, ',') &&
		inFile >> b.date >> comma >> b.venue >> comma >> b.slot >> comma) {
		getline(inFile, b.decoTheme, ',');
		inFile >> b.decoCost >> comma;
		getline(inFile, b.vendorOption, ',');
		inFile >> b.vendorCost;
		inFile.ignore();

		bookings.push_back(b);
		bookingStatus[b.date][b.venue][b.slot] = 1;
	}
	inFile.close();
}

void makeBooking(vector <EventRegistration>& reg,
	vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus,
	const vector<string>& venueNames,
	const vector<string>& timeSlots,
	int MAX_DATES) {

	// First, get the registration ID
	string regID;
	cout << "Enter Registration ID for this booking: ";
	cin >> regID;
	cin.ignore();

	// Find the registration
	EventRegistration* currentReg = nullptr;
	for (auto& r : reg) {
		if (r.registrationID == regID) {
			currentReg = &r;
			break;
		}
	}

	if (currentReg == nullptr) {
		cout << "Registration ID not found! Please register first.\n";
		continuefunc();
		return;
	}

	cout << "Booking for: " << currentReg->manName << " & " << currentReg->womanName << "\n\n";

	int date, venue, slot;
	int themeChoice, vendorChoice;
	Booking newBooking;

	// Set the registration ID for this booking
	newBooking.registrationID = regID;

	// Date validation
	do {
		cout << "Enter booking date (1-" << MAX_DATES << "): ";
		cin >> date;
		if (cin.fail() || date < 1 || date > MAX_DATES) {
			cout << "Invalid date! Please enter 1-" << MAX_DATES << ".\n";
			clearInput();
			continue;
		}
		break;
	} while (true);

	// Venue validation
	displayVenues(venueNames);
	do {
		cout << "Select venue (1-" << venueNames.size() << "): ";
		cin >> venue;
		if (cin.fail() || venue < 1 || venue >(int)venueNames.size()) {
			cout << "Invalid venue! Please enter between 1 and " << venueNames.size() << ".\n";
			clearInput();
			continue;
		}
		break;
	} while (true);

	// Slot validation and availability check
	displayTimeSlots(timeSlots);
	do {
		cout << "Select time slot (1-" << timeSlots.size() << "): ";
		cin >> slot;
		if (cin.fail() || slot < 1 || slot >(int)timeSlots.size()) {
			cout << "Invalid slot! Please enter between 1 and " << timeSlots.size() << ".\n";
			clearInput();
			continue;
		}

		// Convert to 0-based indexing for checking availability
		int venueIndex = venue - 1;
		int slotIndex = slot - 1;

		// Check if this combination is already booked
		if (bookingStatus[date][venueIndex][slotIndex] == 1) {
			cout << "\n*** BOOKING CONFLICT ***\n";
			cout << "Sorry! " << venueNames[venueIndex]
				<< " is already booked for date " << date
				<< " at " << timeSlots[slotIndex] << ".\n";
			cout << "Please choose a different venue or time slot.\n\n";

			// Show available alternatives
			showAvailableSlots(bookingStatus, venueNames, timeSlots, date, MAX_DATES);
			continue; // Ask for slot selection again
		}

		// If we reach here, the slot is available
		break;
	} while (true);

	// Convert to 0-based indexing for final booking
	venue--;
	slot--;

	newBooking.date = date;
	newBooking.venue = venue;
	newBooking.slot = slot;

	// Decoration validation
	displayDecorationThemes();
	do {
		cout << "Choose table decoration theme (1-3): ";
		cin >> themeChoice;
		if (cin.fail() || themeChoice < 1 || themeChoice > 3) {
			cout << "Invalid theme choice!\n";
			clearInput();
			continue;
		}
		break;
	} while (true);

	switch (themeChoice) {
	case 1: newBooking.decoTheme = "Classic"; newBooking.decoCost = 100; break;
	case 2: newBooking.decoTheme = "Rustic"; newBooking.decoCost = 150; break;
	case 3: newBooking.decoTheme = "Elegant"; newBooking.decoCost = 200; break;
	}

	// Vendor validation
	do {
		cout << "Vendor Options:\n";
		cout << "1. Self-arranged (RM 0)\n";
		cout << "2. Organizer-arranged (RM 300)\n";
		cout << "Select vendor option (1-2): ";
		cin >> vendorChoice;
		if (cin.fail() || (vendorChoice != 1 && vendorChoice != 2)) {
			cout << "Invalid vendor choice!\n";
			clearInput();
			continue;
		}
		break;
	} while (true);

	if (vendorChoice == 1) {
		newBooking.vendorOption = "Self-arranged";
		newBooking.vendorCost = 0;
	}
	else {
		newBooking.vendorOption = "Organizer-arranged";
		newBooking.vendorCost = 300;
	}

	// Mark as booked BEFORE saving to prevent race conditions
	bookingStatus[date][venue][slot] = 1;

	// Save to vector
	bookings.push_back(newBooking);

	// Save immediately to file
	saveBookingsToFile(bookings);

	// Show summary
	cout << "\n*** BOOKING CONFIRMED ***\n";
	cout << "Date: " << date
		<< "\nVenue: " << venueNames[venue]
		<< "\nTime Slot: " << timeSlots[slot]
		<< "\nDecoration: " << newBooking.decoTheme << " (RM " << newBooking.decoCost << ")"
		<< "\nVendor: " << newBooking.vendorOption << " (RM " << newBooking.vendorCost << ")"
		<< "\nTotal Cost: RM " << (newBooking.decoCost + newBooking.vendorCost) << "\n";

	continuefunc();
	static int cashCounter = 0;
	processPayment(reg, *currentReg, newBooking.decoCost + newBooking.vendorCost, cashCounter);
}

void showAllBookings(const vector<Booking>& bookings,
	const vector<string>& venueNames,
	const vector<string>& timeSlots) {
    
    if (bookings.empty()) {
        cout << "\nNo bookings yet.\n";
        cout << "Press Enter to continue...";
        cin.ignore(); // Just ignore the leftover newline
        cin.get();    // Wait for Enter
        clearScreen();
        return;
    }
    
    cout << "\nAll Bookings:\n";
    cout << "========================================\n";
    for (const Booking& b : bookings) {
        cout << "Registration ID: " << b.registrationID
            << ", Date: " << b.date
            << ", Venue: " << venueNames[b.venue]
            << ", Time: " << timeSlots[b.slot]
            << ", Deco: " << b.decoTheme
            << ", Vendor: " << b.vendorOption
            << ", Total: RM " << (b.decoCost + b.vendorCost)
            << "\n";
    }
    cout << "========================================\n";
    cout << "Press Enter to continue...";
    cin.ignore(); // Ignore leftover newline from menu choice
    cin.get();    // Wait for user to press Enter
    clearScreen();
}

void continuefunc() {
	cin.ignore();
	cout << endl;
	cout << "Press Enter to continue...";
	cin.get();// wait for enter key
	clearScreen();
}
void processPayment(vector<EventRegistration>& reg, EventRegistration& currentReg, double totalAmountOfLogistic, int& cashCounter) {
	if (reg.empty()) {
		cout << "No registrations found. Please register first.\n";
		continuefunc();
		return;
	}
	double amount = currentReg.paymentInfo.amount + totalAmountOfLogistic;

	while (true)
	{
		cout << "\n--- Payment Section ---\n";
		cout << "Total amount to pay: RM" << fixed << setprecision(2) << amount << endl;
		cout << "Choose Payment Method ( 1 = Bank Card / 2 = FPX / 3 = Cash ): ";

		string method;
		cin >> method;

		for (auto& c : method) c = tolower(c); // normalize

		if (method == "card" || method == "1") {
			string cardNumber;
			cout << "Enter 16-digit card number: ";
			cin >> cardNumber;

			regex cardPattern("^\\d{16}$");
			if (!regex_match(cardNumber, cardPattern)) {
				cout << "Invalid card number format. Payment failed." << endl;
				continue;
			}
			currentReg.paymentInfo.method = "Card";
			currentReg.paymentInfo.amount = amount;
			currentReg.paymentInfo.paid = true;
			cout << "Payment successful!" << endl;
			break; // Exit the loop
		}
		else if (method == "fpx" || method == "2") {
			cout << "FPX payment selected. Proceeding..." << endl;
			currentReg.paymentInfo.method = "FPX";
			currentReg.paymentInfo.amount = amount;
			currentReg.paymentInfo.paid = true;
			cout << "Payment successful!" << endl;
			break; // Exit the loop
		}
		else if (method == "cash" || method == "3") {
			cout << "Cash payment selected. Payment will be collected on event day." << endl;
			currentReg.paymentInfo.method = "Cash";
			currentReg.paymentInfo.amount = amount;
			currentReg.paymentInfo.paid = false;
			cashCounter++;
			cout << "Cash payment registered!" << endl;
			break; // Exit the loop
		}
		else {
			cout << "Invalid payment method. Please choose again." << endl;
			continue;
		}
	}

	// Update the registration in the vector
	for (auto& r : reg) {
		if (r.registrationID == currentReg.registrationID) {
			r.paymentInfo = currentReg.paymentInfo;
			break;
		}
	}

	// Save updated registrations to file
	saveRegistrationsToFile(reg);

	cout << "Payment information saved successfully!" << endl;
	continuefunc();
}
void showAvailableSlots(const vector<vector<vector<int>>>& bookingStatus,
	const vector<string>& venueNames,
	const vector<string>& timeSlots,
	int date, int MAX_DATES) {

	cout << "Available slots for date " << date << ":\n";
	bool hasAvailable = false;

	for (int v = 0; v < (int)venueNames.size(); v++) {
		for (int s = 0; s < (int)timeSlots.size(); s++) {
			if (bookingStatus[date][v][s] == 0) {
				cout << "- " << venueNames[v] << " at " << timeSlots[s] << "\n";
				hasAvailable = true;
			}
		}
	}

	if (!hasAvailable) {
		cout << "No available slots for date " << date << ". Please try a different date.\n";
	}
	cout << "\n";
}

// New printReceipt function
void printReceipt(const EventRegistration& reg, const Booking* booking) {
    const int WIDTH = 50;
    
    // Header
    cout << string(WIDTH, '=') << "\n";
    cout << setw(WIDTH/2 + 10) << "EVENT PROPOSAL MANAGEMENT" << "\n";
    cout << setw(WIDTH/2 + 8) << "Payment Receipt" << "\n";
    cout << string(WIDTH, '=') << "\n\n";
    
    // Registration Details
    cout << "REGISTRATION DETAILS:\n";
    cout << string(WIDTH, '-') << "\n";
    cout << left << setw(20) << "Registration ID:" << reg.registrationID << "\n";
    cout << left << setw(20) << "Couple Names:" << reg.manName << " & " << reg.womanName << "\n";
    cout << left << setw(20) << "Phone:" << reg.phone << "\n";
    cout << left << setw(20) << "Email:" << reg.email << "\n";
    cout << left << setw(20) << "Guests:" << reg.numberOfGuests << " people\n";
    cout << left << setw(20) << "Package:" << reg.packageType << "\n";
    cout << left << setw(20) << "Special Requests:" << reg.specialRequests << "\n\n";
    
    // Booking Details (if exists)
    if (booking) {
        cout << "BOOKING DETAILS:\n";
        cout << string(WIDTH, '-') << "\n";
        cout << left << setw(20) << "Event Date:" << "Day " << booking->date << "\n";
        cout << left << setw(20) << "Venue:" << getVenueName(booking->venue) << "\n";
        cout << left << setw(20) << "Time Slot:" << getTimeSlotName(booking->slot) << "\n";
        cout << left << setw(20) << "Decoration:" << booking->decoTheme << "\n";
        cout << left << setw(20) << "Vendor Service:" << booking->vendorOption << "\n\n";
    }
    
    // Payment Summary
    cout << "PAYMENT SUMMARY:\n";
    cout << string(WIDTH, '-') << "\n";
    
    // Calculate costs
    double packageCost = getPackageCost(reg.packageType);
    double decorationCost = booking ? booking->decoCost : 0.0;
    double vendorCost = booking ? booking->vendorCost : 0.0;
    double totalCost = reg.paymentInfo.amount;
    
    cout << left << setw(30) << "Package (" + reg.packageType + "):" 
         << "RM " << right << setw(8) << fixed << setprecision(2) << packageCost << "\n";
    
    if (booking) {
        if (decorationCost > 0) {
            cout << left << setw(30) << "Decoration (" + booking->decoTheme + "):" 
                 << "RM " << right << setw(8) << fixed << setprecision(2) << decorationCost << "\n";
        }
        if (vendorCost > 0) {
            cout << left << setw(30) << "Vendor Service:" 
                 << "RM " << right << setw(8) << fixed << setprecision(2) << vendorCost << "\n";
        }
    }
    
    cout << string(WIDTH, '-') << "\n";
    cout << left << setw(30) << "TOTAL AMOUNT:" 
         << "RM " << right << setw(8) << fixed << setprecision(2) << totalCost << "\n";
    cout << string(WIDTH, '-') << "\n\n";
    
    // Payment Status
    cout << "PAYMENT INFORMATION:\n";
    cout << string(WIDTH, '-') << "\n";
    cout << left << setw(20) << "Payment Method:" << reg.paymentInfo.method << "\n";
    cout << left << setw(20) << "Payment Status:";
    
    if (reg.paymentInfo.paid) {
        cout << " ✓ PAID\n";
        cout << left << setw(20) << "Status:" << "Payment Completed\n";
    } else {
        cout << " PENDING\n";
        if (reg.paymentInfo.method == "Cash") {
            cout << left << setw(20) << "Status:" << "Cash payment on event day\n";
        } else {
            cout << left << setw(20) << "Status:" << "Payment processing...\n";
        }
    }
    
    // Footer
    cout << "\n" << string(WIDTH, '=') << "\n";
    cout << setw(WIDTH/2 + 8) << "Thank you for choosing us!" << "\n";
    cout << setw(WIDTH/2 + 12) << "Have a wonderful celebration!" << "\n";
    cout << string(WIDTH, '=') << "\n";
}

// Helper functions to replace the switch statements
string getVenueName(int venueIndex) {
    const vector<string> venues = {"Hall A", "Hall B", "Hall C", "Outdoor", "VIP Lounge"};
    if (venueIndex >= 0 && venueIndex < venues.size()) {
        return venues[venueIndex];
    }
    return "Unknown Venue";
}

string getTimeSlotName(int slotIndex) {
    const vector<string> timeSlots = {"12pm-3pm", "4pm-7pm", "8pm-11pm"};
    if (slotIndex >= 0 && slotIndex < timeSlots.size()) {
        return timeSlots[slotIndex];
    }
    return "Unknown Time";
}

double getPackageCost(const string& packageType) {
    if (packageType == "Surprise Proposal") return 1200.0;
    if (packageType == "Romantic Dinner") return 1500.0;
    if (packageType == "Family Gathering") return 1500.0;
    return 0.0;
}