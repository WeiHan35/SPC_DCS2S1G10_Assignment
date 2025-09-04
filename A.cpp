#include <iomanip>
#include <vector>
#include <regex>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <limits>  
#include <ctime>
using namespace std;
struct DateInfo {
	int year = 0;
	int month = 0;
	int day = 0;
	int maxDays = 0;
};
struct Payment {
	string method = "";
	bool paid = false;
	double amount = 0.0;
};

struct EventRegistration {
	string registrationID = "";
	string manName = "";
	string womanName = "";
	string phone = "";
	string email = "";
	int numberOfGuests = 0;
	string packageType = "";
	string specialRequests = "";
	Payment paymentInfo;
};

struct Booking {
	string registrationID = "";
	int date = 0;
	int venue = 0;
	int slot = 0;
	string decoTheme = "";
	double decoCost = 0.0;
	string vendorOption = "";
	int vendorCost = 0;
};

// ===== Main Function =====
void displayMainMenu();
void displayHelp();
void handleRegistrationMenu(vector<EventRegistration>& registrations);
void handleBookingMenu(vector<EventRegistration>& registrations,
	vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus);
void handlePaymentStatusCheck(vector<EventRegistration>& registrations,
	vector<Booking>& bookings);
//Registration related functions
void displayRegistrationMenu();
void addRegistration(vector<EventRegistration>& reg);
void searchRegistration(vector<EventRegistration>& registrations);
void deleteRegistration(vector<EventRegistration>& registrations);
void showAllRegistrations(const vector<EventRegistration>& reg);
void loadRegistrationsFromFile(vector<EventRegistration>& registrations);
void updateRegistrationByID(vector<EventRegistration>& reg);
void displayRegistrationIdReminder(const string& regID);
void outputRegistration(const EventRegistration& reg);
EventRegistration inputRegistration();
void saveRegistrationsToFile(const vector<EventRegistration>& reg);

EventRegistration* findRegistrationById(vector<EventRegistration>& reg, const string& id);
void displayRegistrationSummary(const EventRegistration& reg);
string generateNewRegistrationId(const vector<EventRegistration>& reg);
EventRegistration parseRegistrationFromLine(const string& line);
string getRegistrationIdInput();

// ===== Registration Function Declarations =====
string inputName(const string& prompt);
string inputPhone();
string inputEmail();
int inputGuests();
string inputPackage();
string inputSpecialRequest();
string checkPaid(bool paid);


//booking related functions
void displayBookingMenu();
void makeBooking(vector<EventRegistration>& reg,
	vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus,
	const vector<string>& venueNames,
	const vector<string>& timeSlots,
	int MAX_DATES);
void showAllBookings(const vector<Booking>& bookings,
	const vector<string>& venueNames,
	const vector<string>& timeSlots);
void displayVenues(const vector<string>& venueNames);
void saveBookingsToFile(const vector<Booking>& bookings);
void loadBookingsFromFile(vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus);
const Booking* findBookingByRegistrationId(const vector<Booking>& bookings, const string& id);
void showAvailableSlots(const vector<vector<vector<int>>>& bookingStatus,
	const vector<string>& venueNames,
	const vector<string>& timeSlots,
	int date, int MAX_DATES);
bool hasExistingBooking(const vector<Booking>& bookings, const string& regID);
void displayTimeSlots(const vector<string>& timeSlots);
void displayDecorationThemes();
DateInfo getCurrentDate();
string formatDateWithDay(int dateNumber, int month, int year);

//Booking  validation fucntions
string getVenueName(int venueIndex);
string getTimeSlotName(int slotIndex);
double getPackageCost(const string& packageType);

// Date validation functions
bool isDateAfterCurrent(int month, int year, int day);
int getDaysInMonth(int month, int year);
bool isLeapYear(int year);
DateInfo validateMonthYear();
int inputDateWithValidation(int MAX_DATES, const DateInfo& selectedDate);
string getDayOfWeek(int day, int month, int year);

//Payment related functions
void processPayment(vector<EventRegistration>& reg, EventRegistration& currentReg, double totalAmountOfLogistic, int& cashCounter);
void printReceipt(const EventRegistration& reg, const Booking* booking);

//Utility functions
void continuefunc();
void clearScreen();
void clearInput();
bool confirmAction(const string& prompt);

// Input validation utility
int getValidatedChoice(int min, int max, const string& prompt);
string inputValidatedString(const string& prompt, const regex& pattern, const string& errorMsg);




//string inputName();
//string inputPhone();
//string inputEmail();
//int inputGuests();
//string inputPackage();
//string inputSpecialRequest();
//string checkPaid();



int main() {
	vector<EventRegistration> reg;
	const int MAX_DATES = 31;
	const int MAX_VENUES = 5;
	const int MAX_SLOTS = 3;

	vector<string> timeSlots = { "12pm-3pm", "4pm-7pm", "8pm-11pm" };
	vector<string> venueNames = { "Hall A", "Hall B", "Hall C", "Outdoor", "VIP Lounge" };

	// bookingStatus[date][venue][slot]
	vector<vector<vector<int>>> bookingStatus(MAX_DATES + 1, vector<vector<int>>(MAX_VENUES, vector<int>(MAX_SLOTS, 0)));

	vector<Booking> bookings;
	loadRegistrationsFromFile(reg); // Use this instead
	loadBookingsFromFile(bookings, bookingStatus); // Load existing bookings from file
	while (true) {
		clearScreen();
		displayMainMenu();

		int choice = getValidatedChoice(0, 4, "Enter your choice: ");

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
		case 4:
			clearScreen();

			displayHelp();
			continuefunc();
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

		int choice = getValidatedChoice(0, 5, "Enter your choice: ");

		switch (choice) {
		case 1: addRegistration(registrations); break;
		case 2: searchRegistration(registrations); break;
		case  3: showAllRegistrations(registrations); break;
		case 4: deleteRegistration(registrations); break;
		case 5: updateRegistrationByID(registrations); break;
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

void handleBookingMenu(vector<EventRegistration>& registrations,
	vector<Booking>& bookings,
	vector<vector<vector<int>>>& bookingStatus) {
	while (true) {
		clearScreen();
		displayBookingMenu();

		int choice = getValidatedChoice(0, 2, "Enter your choice: ");

		switch (choice) {


		case 1: makeBooking(registrations, bookings, bookingStatus,
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

void addRegistration(vector<EventRegistration>& registrations) {
	clearScreen();
	cout << "=== Add New Registration ===\n\n";
	cin.ignore();
	EventRegistration newReg = inputRegistration();
	newReg.registrationID = generateNewRegistrationId(registrations);

	registrations.push_back(newReg);
	saveRegistrationsToFile(registrations);

	cout << "\nRegistration successful! ID: " << newReg.registrationID << "\n";
	displayRegistrationIdReminder(newReg.registrationID);
	clearScreen();
	displayRegistrationSummary(newReg);
	continuefunc();
}

void displayMainMenu() {
	cout << "========================================\n";
	cout << "       Propose Event Management System\n";
	cout << "========================================\n";
	cout << "1. Register for Customers\n";
	cout << "2. Booking & Logistics for event\n";
	cout << "3. Check Payment Status\n";
	cout << "4. Help & Guidelines\n";
	cout << "0. Exit\n";
	cout << "========================================\n";
}

void displayRegistrationMenu() {
	cout << "========================================\n";
	cout << "       Registration Management\n";
	cout << "========================================\n";
	cout << "1. Add New Registration\n";
	cout << "2. Search Registration\n";
	cout << "3. View All Registrations\n";
	cout << "4. Delete Registration\n";
	cout << "5. Update Registration\n";
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
		cout << "Registration found!\n";
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
				cout << "Registration deleted successfully!\n";
			}
			else {
				cout << "Deletion cancelled.\n";
			}
			continuefunc();
			return;
		}
	}

	cout << "No registration found with ID: " << id << "\n";
	continuefunc();
}




void updateRegistrationByID(vector<EventRegistration>& reg) {
	clearScreen();

	string id;
	cout << "Enter Registration ID to update: ";
	cin >> id;
	cin.ignore();

	for (auto& r : reg) {
		if (id == r.registrationID) {
			cout << "Registration Found:\n";
			outputRegistration(r);

			cout << "\n=== Update Registration Details ===\n";
			cout << "Press Enter to keep current value, or type 'y' to update:\n\n";

			string input;

			// Update 1st Couple Name
			cout << "Current 1st Groom-to-be: " << r.manName << "\n";
			cout << "Update 1st couple name? (y/n): ";
			getline(cin, input);
			if (!input.empty() && tolower(input[0]) == 'y') {
				r.manName = inputName("Enter new 1st Groom-to-be name: ");
				cout << "Name updated successfully!\n";
			}
			cout << "\n";

			// Update 2nd Couple Name
			cout << "Current 2nd Bridge-to-be: " << r.womanName << "\n";
			cout << "Update 2nd couple name? (y/n): ";
			getline(cin, input);
			if (!input.empty() && tolower(input[0]) == 'y') {
				r.womanName = inputName("Enter new 2nd Bridge-to-be name: ");
				cout << "Name updated successfully!\n";
			}
			cout << "\n";

			// Update Phone
			cout << "Current Phone: " << r.phone << "\n";
			cout << "Update phone number? (y/n): ";
			getline(cin, input);
			if (!input.empty() && tolower(input[0]) == 'y') {
				r.phone = inputPhone();
				cout << "Phone updated successfully!\n";
			}
			cout << "\n";

			// Update Email
			cout << "Current Email: " << r.email << "\n";
			cout << "Update email? (y/n): ";
			getline(cin, input);
			if (!input.empty() && tolower(input[0]) == 'y') {
				r.email = inputEmail();
				cout << "Email updated successfully!\n";
			}
			cout << "\n";

			// Update Number of Guests
			cout << "Current Number of Guests: " << r.numberOfGuests << "\n";
			cout << "Update number of guests? (y/n): ";
			getline(cin, input);
			if (!input.empty() && tolower(input[0]) == 'y') {
				r.numberOfGuests = inputGuests();
				cout << "Number of guests updated successfully!\n";
			}
			cout << "\n";

			// Update Package Type
			cout << "Current Package: " << r.packageType << "\n";
			cout << "Update package? (y/n): ";
			getline(cin, input);
			if (!input.empty() && tolower(input[0]) == 'y') {
				string oldPackage = r.packageType;
				r.packageType = inputPackage();

				// Update package cost based on new package
				if (r.packageType == "Surprise Propose") {
					r.paymentInfo.amount = 1200.0;
				}
				else if (r.packageType == "Romantic Dinner") {
					r.paymentInfo.amount = 1500.0;
				}
				else if (r.packageType == "Family Gathering") {
					r.paymentInfo.amount = 1500.0;
				}

				cout << "Package updated from '" << oldPackage << "' to '" << r.packageType << "'!\n";
			}
			cout << "\n";

			// Update Special Requests
			cout << "Current Special Requests: " << r.specialRequests << "\n";
			cout << "Update special requests? (y/n): ";
			getline(cin, input);
			if (!input.empty() && tolower(input[0]) == 'y') {
				// Use a simple input for special requests since there's no validation function for it
				cout << "Enter new special requests (leave blank for 'None'): ";
				string newRequests;
				getline(cin, newRequests);
				if (newRequests.empty()) {
					newRequests = "None";
				}
				r.specialRequests = newRequests;
				cout << "Special requests updated successfully!\n";
			}
			cout << "\n";

			// Save changes
			saveRegistrationsToFile(reg);
			cout << "=== Registration Updated Successfully! ===\n";

			// Show updated registration
			cout << "\nUpdated Registration:\n";
			displayRegistrationSummary(r);

			continuefunc();
			return;
		}
	}

	cout << "No Registration found with ID: " << id << "\n";
	continuefunc();
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
			clearInput();
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

string inputValidatedString(const string& prompt, const regex& pattern, const string& errorMsg) {
	string input;
	while (true) {
		cout << prompt;
		clearInput();
		getline(cin, input);

		//check if input matches the pattern and is not empty with only spaces and make the if statement more simple and understandable
		if (regex_match(input, pattern) && !input.empty() && input.find_first_not_of(' ') != string::npos) {
			return input;
		}
		cout << errorMsg << "\n";
	}
}

string inputName(const string& prompt) {

	return inputValidatedString(prompt,
		regex("^[A-Za-z ]{1,50}$"),
		"Invalid name. Only letters and spaces allowed (max 50 characters).");
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
	cout << "1. Surprise Propose (RM 1200   )\n";
	cout << "2. Romantic Dinner (RM 1500   )\n";
	cout << "3. Family Gathering (RM 1500)\n";

	int choice = getValidatedChoice(1, 3, "Select package: ");

	switch (choice) {
	case 1: return "Surprise Propose";
	case 2: return "Romantic Dinner";
	case 3: return "Family Gathering";
	default: return "Surprise Propose"; // fallback
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
	reg.manName = inputName("Enter 1st Groom-to-be name : ");
	reg.womanName = inputName("Enter 2nd Bridge-to-be name : ");
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
	else if (reg.packageType == "Surprise Propose")
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

void showAllBookings(const vector<Booking>& bookings,
	const vector<string>& venueNames,
	const vector<string>& timeSlots) {

	if (bookings.empty()) {
		cout << "\nNo bookings yet.\n";
		cout << "Press Enter to continue...";
		cin.ignore();
		cin.get();
		clearScreen();
		return;
	}
	clearScreen();
	cout << "\nAll Bookings:\n";
	cout << "========================================\n";
	for (const Booking& b : bookings) {
		cout << "Registration ID: " << b.registrationID
			<< "\nDate: " << formatDateWithDay(b.date, 12, 2024)  // Updated this line
			<< "\nVenue: " << venueNames[b.venue]
			<< "\nTime: " << timeSlots[b.slot]
			<< "\nDecoration: " << b.decoTheme
			<< "\nVendor: " << b.vendorOption
			<< "\nTotal Cost: RM " << (b.decoCost + b.vendorCost)
			<< "\n" << string(40, '-') << "\n";
	}
	cout << "========================================\n";
	cout << "Press Enter to continue...";
	cin.ignore();
	cin.get();
	clearScreen();
}

void continuefunc() {
	cin.clear();
	cout << "\nPress Enter to continue...";
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
			cin.ignore();
			continuefunc();
			break; // Exit the loop
		}
		else if (method == "fpx" || method == "2") {
			cout << "FPX payment selected. Proceeding..." << endl;
			currentReg.paymentInfo.method = "FPX";
			currentReg.paymentInfo.amount = amount;
			currentReg.paymentInfo.paid = true;
			cout << "Payment successful!" << endl;
			cin.ignore();

			continuefunc();

			break; // Exit the loop
		}
		else if (method == "cash" || method == "3") {
			cout << "Cash payment selected. Payment will be collected on event day." << endl;
			currentReg.paymentInfo.method = "Cash";
			currentReg.paymentInfo.amount = amount;
			currentReg.paymentInfo.paid = false;
			cashCounter++;
			cout << "Cash payment registered!" << endl;
			cin.ignore();

			continuefunc();

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

	cout << "Available slots for " << formatDateWithDay(date, 12, 2024) << ":\n";
	bool hasAvailable = false;

	for (int v = 0; v < (int)venueNames.size(); v++) {
		for (int s = 0; s < (int)timeSlots.size(); s++) {
			if (bookingStatus[date][v][s] == 0) {
				cout << venueNames[v] << " at " << timeSlots[s] << "\n";
				hasAvailable = true;
			}
		}
	}

	if (!hasAvailable) {
		cout << "No available slots for " << formatDateWithDay(date, 12, 2024) << "\n";
		cout << "Please try a different date.\n";
	}
	cout << "\n";
}

void printReceipt(const EventRegistration& reg, const Booking* booking) {
	const int WIDTH = 50;

	// Header
	cout << string(WIDTH, '=') << "\n";
	cout << setw(WIDTH / 2 + 10) << "EVENT PROPOSE MANAGEMENT" << "\n";
	cout << setw(WIDTH / 2 + 8) << "Payment Receipt" << "\n";
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

	// Booking Details (if exists) or No Booking message
	if (booking) {
		cout << "BOOKING DETAILS:\n";
		cout << string(WIDTH, '-') << "\n";
		cout << left << setw(20) << "Event Date:" << formatDateWithDay(booking->date, 12, 2024) << "\n";
		cout << left << setw(20) << "Venue:" << getVenueName(booking->venue) << "\n";
		cout << left << setw(20) << "Time Slot:" << getTimeSlotName(booking->slot) << "\n";
		cout << left << setw(20) << "Decoration:" << booking->decoTheme << "\n";
		cout << left << setw(20) << "Vendor Service:" << booking->vendorOption << "\n\n";
	}
	else {
		cout << "BOOKING STATUS:\n";
		cout << string(WIDTH, '-') << "\n";
		cout << "*** NO BOOKING MADE YET ***\n";
		cout << "This registration has not made any event booking.\n";
		cout << "To complete your propose event setup:\n";
		cout << "1. Go to 'Booking & Logistics' from main menu\n";
		cout << "2. Select 'Create New Booking'\n";
		cout << "3. Use Registration ID: " << reg.registrationID << "\n";
		cout << string(WIDTH, '-') << "\n\n";
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
	else {
		// Show what costs will be added when booking is made
		cout << left << setw(30) << "Decoration Cost:" << "RM " << right << setw(8) << "TBD*\n";
		cout << left << setw(30) << "Vendor Service Cost:" << "RM " << right << setw(8) << "TBD*\n";
		cout << left << setw(30) << "*To Be Determined after booking" << "\n";
	}

	cout << string(WIDTH, '-') << "\n";
	cout << left << setw(30) << "CURRENT TOTAL:"
		<< "RM " << right << setw(8) << fixed << setprecision(2) << totalCost << "\n";

	if (!booking) {
		cout << left << setw(30) << "(Package fee only)" << "\n";
	}

	cout << string(WIDTH, '-') << "\n\n";

	// Payment Status
	cout << "PAYMENT INFORMATION:\n";
	cout << string(WIDTH, '-') << "\n";
	if (reg.paymentInfo.method == "")
	{
		cout << left << setw(20) << "Payment Method:" << "PENDING\n";
	}
	else
	{
		cout << left << setw(20) << "Payment Method:" << reg.paymentInfo.method << "\n";
	}
	cout << left << setw(20) << "Payment Status:";

	if (reg.paymentInfo.paid) {
		cout << "PAID!\n";
		cout << left << setw(20) << "Status:" << "Payment Completed\n";
	}
	else {
		cout << " PENDING\n";
		if (reg.paymentInfo.method == "Cash") {
			cout << left << setw(20) << "Status:" << "Cash payment on event day\n";
		}
		else if (!booking) {
			cout << left << setw(20) << "Status:" << "Complete booking to finalize payment\n";
		}
	}

	// Footer
	cout << "\n" << string(WIDTH, '=') << "\n";
	cout << setw(WIDTH / 2 + 8) << "Thank you for choosing us!" << "\n";
	if (booking) {
		cout << setw(WIDTH / 2 + 12) << "Have a wonderful celebration!" << "\n";
	}
	else {
		cout << setw(WIDTH / 2 + 10) << "Please complete your booking!" << "\n";
	}
	cout << string(WIDTH, '=') << "\n";
}

// Helper functions to replace the switch statements
string getVenueName(int venueIndex) {
	const vector<string> venues = { "Hall A", "Hall B", "Hall C", "Outdoor", "VIP Lounge" };
	if (venueIndex >= 0 && venueIndex < venues.size()) {
		return venues[venueIndex];
	}
	return "Unknown Venue";
}

string getTimeSlotName(int slotIndex) {
	const vector<string> timeSlots = { "12pm-3pm", "4pm-7pm", "8pm-11pm" };
	if (slotIndex >= 0 && slotIndex < timeSlots.size()) {
		return timeSlots[slotIndex];
	}
	return "Unknown Time";
}

double getPackageCost(const string& packageType) {
	if (packageType == "Surprise Propose") return 1200.0;
	if (packageType == "Romantic Dinner") return 1500.0;
	if (packageType == "Family Gathering") return 1500.0;
	return 0.0;
}

void displayHelp() {
	clearScreen();
	cout << "========================================\n";
	cout << "           HELP & GUIDANCE\n";
	cout << "========================================\n\n";

	cout << "1. Registration Management:\n";
	cout << "   - Add New Registration: Input couple names, contact info, number of guests, package type, and special requests.\n";
	cout << "   - Search Registration: Find a registration by its unique ID.\n";
	cout << "   - View All Registrations: Display a list of all registrations.\n";
	cout << "   - Delete Registration: Remove a registration by its ID.\n";
	cout << "   - Update Registration: Modify details of an existing registration.\n\n";
	cout << "2. Booking & Logistics:\n";
	cout << "   - One registration = One propose event booking\n";
	cout << "   - Each couple can only book one propose event\n";
	cout << "   - Choose your date, venue, and time carefully\n";
	cout << "   - Contact support if you need to change your booking\n\n";
	cout << "   - Booking & Logistics:\n";
	cout << "   - Make a Booking: Choose event date, venue, time slot, decoration theme, and vendor services.\n";
	cout << "   - View All Bookings: Display all bookings with details.\n\n";
	cout << "3. Payment Status Check:\n";
	cout << "   - Check the payment status of a registration using its ID.\n\n";
	cout << "4. Payment Methods:\n";
	cout << "   - Bank Card: Enter your 16-digit card number for payment.\n";
	cout << "   - FPX: Use online banking for payment.\n";
	cout << "   - Cash: Pay in cash on the event day (payment status will be pending until then).\n\n";

	cout << "5. Important Notes:\n";
	cout << "   - Always remember your Registration ID for bookings and payment status checks.\n";
	cout << "   - Ensure all information is accurate during registration to avoid issues later.\n";
	cout << "   - Contact support for any issues or questions regarding your registration or booking.\n\n";
	cout << "For further assistance, please contact our support team with number: +60123456789 ";
	cin.ignore();

}

string getDayOfWeek(int day, int month, int year) {
	// Using Zeller's congruence algorithm to find day of week_
	vector<string> days = { "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };

	if (month < 3) {
		month += 12;
		year--;
	}

	int k = year % 100;
	int j = year / 100;

	int h = (day + ((13 * (month + 1)) / 5) + k + (k / 4) + (j / 4) - 2 * j) % 7;

	return days[h];
}



DateInfo getCurrentDate() {
	time_t now = time(0);
	tm currentTime;
	localtime_s(&currentTime, &now);

	DateInfo current;
	current.year = currentTime.tm_year + 1900;  // tm_year is years since 1900
	current.month = currentTime.tm_mon + 1;     // tm_mon is 0-11, so add 1
	current.maxDays = getDaysInMonth(current.month, current.year);

	return current;
}

bool isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year) {
	int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (month == 2 && isLeapYear(year)) {
		return 29;
	}
	return daysInMonth[month - 1];
}

bool isDateAfterCurrent(int month, int year, int day) {
	DateInfo current = getCurrentDate();

	// If year is greater than current year, it's valid
	if (year > current.year) return true;

	// If year is less than current year, it's invalid
	if (year < current.year) return false;

	// Same year - check month
	if (month > current.month) return true;
	if (month < current.month) return false;

	// Same year and month - check day
	return day > current.day; // FIX: use current.day instead of currentTime->tm_mday
}

DateInfo validateMonthYear() {
	DateInfo current = getCurrentDate();
	DateInfo selected;
	const int MAX_YEAR = 2030; // Add year limit

	cout << "\n=== Select Event Date ===\n";
	cout << "Current date: " << current.month << "/" << current.year << "\n";
	cout << "Please select a future date for your event.\n";
	cout << "Available years: " << current.year << " - " << MAX_YEAR << "\n\n";

	while (true) {
		// Get year with upper limit
		cout << "Enter year (" << current.year << " - " << MAX_YEAR << "): ";
		cin >> selected.year;

		if (cin.fail() || selected.year < current.year || selected.year > MAX_YEAR) {
			cout << "Invalid year! Must be between " << current.year << " and " << MAX_YEAR << ".\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		// Get month
		cout << "Enter month (1-12): ";
		cin >> selected.month;

		if (cin.fail() || selected.month < 1 || selected.month > 12) {
			cout << "Invalid month! Must be between 1 and 12.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		// Validate that the selected month/year is not in the past
		if (selected.year == current.year && selected.month < current.month) {
			cout << "Cannot select a past month! Please choose " << current.month
				<< " or later for year " << current.year << ".\n";
			continue;
		}

		selected.maxDays = getDaysInMonth(selected.month, selected.year);

		cout << "\nSelected: " << selected.month << "/" << selected.year
			<< " (up to " << selected.maxDays << " days available)\n";

		char confirm;
		cout << "Confirm this selection? (y/n): ";
		cin >> confirm;

		if (tolower(confirm) == 'y') {
			break;
		}
	}

	return selected;
}

// Update the existing inputDateWithValidation function
int inputDateWithValidation(int MAX_DATES, const DateInfo& selectedDate) {
	DateInfo current = getCurrentDate();
	int date;
	int actualMaxDays = min(MAX_DATES, selectedDate.maxDays);

	string monthNames[] = { "", "January", "February", "March", "April", "May", "June",
						  "July", "August", "September", "October", "November", "December" };

	cout << "\nAvailable dates for " << monthNames[selectedDate.month] << " " << selectedDate.year << ":\n";
	cout << "========================================\n";

	// Show first 10 dates as examples
	for (int i = 1; i <= min(10, actualMaxDays); i++) {
		// Skip dates that are in the past
		if (selectedDate.year == current.year &&
			selectedDate.month == current.month &&
			i <= current.day) {
			continue;
		}
		cout << i << ". " << formatDateWithDay(i, selectedDate.month, selectedDate.year) << "\n";
	}

	if (actualMaxDays > 10) {
		cout << "... (and " << (actualMaxDays - 10) << " more dates available)\n";
	}
	cout << "========================================\n";

	do {
		cout << "Enter booking date (1-" << actualMaxDays << "): ";
		cin >> date;

		if (cin.fail() || date < 1 || date > actualMaxDays) {
			cout << "Invalid date! Please enter 1-" << actualMaxDays << ".\n";
			clearInput();
			continue;
		}

		// Check if the selected date is in the past
		if (!isDateAfterCurrent(selectedDate.month, selectedDate.year, date)) {
			cout << "Cannot select a past date! Please choose a future date.\n";
			continue;
		}

		break;
	} while (true);

	return date;
}

// Update the existing formatDateWithDay function
string formatDateWithDay(int dateNumber, int month, int year) {
	string dayName = getDayOfWeek(dateNumber, month, year);
	string monthNames[] = { "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
						  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	return "Day " + to_string(dateNumber) + " (" + dayName + ", " +
		to_string(dateNumber) + " " + monthNames[month] + " " + to_string(year) + ")";
}

// Update the makeBooking function to use the new date selection
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
		cout << "\nIf you forgot your Registration ID:\n";
		cout << "1. Go to Registration Management\n";
		cout << "2. Choose 'Find Registration by Details'\n";
		cout << "3. Search using your name, phone, or email\n";
		continuefunc();
		return;
	}

	// NEW: Check if this registration already has a booking
	if (hasExistingBooking(bookings, regID)) {
		cout << "\n*** BOOKING ALREADY EXISTS ***\n";
		cout << "Registration ID " << regID << " already has a propose event booking.\n";
		cout << "Each couple can only book ONE propose event.\n\n";

		// Show existing booking details
		for (const auto& booking : bookings) {
			if (booking.registrationID == regID) {
				cout << "Your existing booking:\n";
				cout << "Date: " << formatDateWithDay(booking.date, 12, 2024) << "\n";
				cout << "Venue: " << venueNames[booking.venue] << "\n";
				cout << "Time: " << timeSlots[booking.slot] << "\n";
				cout << "Decoration: " << booking.decoTheme << "\n";
				cout << "Vendor: " << booking.vendorOption << "\n";
				break;
			}
		}

		cout << "\nIf you need to change your booking, please contact support.\n";
		continuefunc();
		return;
	}
	clearScreen();
	cout << "\n=== Registration Details ===\n";
	displayRegistrationSummary(*currentReg);
	cout << "\nCreating propose event booking for: " << currentReg->manName << " & " << currentReg->womanName << "\n\n";

	// Rest of the booking process remains the same...
	DateInfo selectedDate = validateMonthYear();

	int date, venue, slot;
	int themeChoice, vendorChoice;
	Booking newBooking;

	newBooking.registrationID = regID;
	date = inputDateWithValidation(MAX_DATES, selectedDate);

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

		int venueIndex = venue - 1;
		int slotIndex = slot - 1;

		if (bookingStatus[date][venueIndex][slotIndex] == 1) {
			cout << "\n*** BOOKING CONFLICT ***\n";
			cout << "Sorry! " << venueNames[venueIndex]
				<< " is already booked for date " << date
				<< " at " << timeSlots[slotIndex] << ".\n";
			cout << "Please choose a different venue or time slot.\n\n";

			showAvailableSlots(bookingStatus, venueNames, timeSlots, date, MAX_DATES);
			continue;
		}

		break;
	} while (true);

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

	bookingStatus[date][venue][slot] = 1;
	bookings.push_back(newBooking);
	saveBookingsToFile(bookings);

	// Show booking confirmation
	cout << "\n*** PROPOSE EVENT BOOKING CONFIRMED ***\n";
	cout << "Registration ID: " << regID << "\n";
	cout << "Date: " << formatDateWithDay(date, 12, 2024)
		<< "\nVenue: " << venueNames[venue]
		<< "\nTime Slot: " << timeSlots[slot]
		<< "\nDecoration: " << newBooking.decoTheme << " (RM " << newBooking.decoCost << ")"
		<< "\nVendor: " << newBooking.vendorOption << " (RM " << newBooking.vendorCost << ")"
		<< "\nTotal Booking Cost: RM " << (newBooking.decoCost + newBooking.vendorCost) << "\n";

	cout << "\nCongratulations on your upcoming propose! \n";

	continuefunc();
	static int cashCounter = 0;
	processPayment(reg, *currentReg, newBooking.decoCost + newBooking.vendorCost, cashCounter);
}
void displayRegistrationIdReminder(const string& regID) {
	const int WIDTH = 60;

	cout << "\n" << string(WIDTH, '=') << "\n";
	cout << "    *** IMPORTANT: SAVE YOUR REGISTRATION ID ***\n";
	cout << string(WIDTH, '=') << "\n";
	cout << "    YOUR REGISTRATION ID IS: " << regID << "\n";
	cout << "    WRITE THIS DOWN! YOU NEED IT FOR BOOKING!\n";
	cout << string(WIDTH, '=') << "\n";
	cout << "    This ID is required for:\n";
	cout << "    - Making bookings\n";
	cout << "    - Checking payment status\n";
	cout << "    - Updating registration details\n";
	cout << string(WIDTH, '=') << "\n\n";

	// Force user to acknowledge they've seen the ID
	cout << "Type your Registration ID to confirm you've noted it: ";
	string confirmID;
	cin >> confirmID;
	cin.ignore();

	while (confirmID != regID) {
		cout << "ID doesn't match! Please type exactly: " << regID << "\n";
		cout << "Enter Registration ID: ";
		cin >> confirmID;
		cin.ignore();
	}

	cout << "\n[SUCCESS] Registration ID confirmed!\n";
}
void showAllRegistrations(const vector<EventRegistration>& registrations) {
	clearScreen();
	cout << "=== All Registrations ===\n\n";

	if (registrations.empty()) {
		cout << "No registrations found.\n";
		cin.ignore();
		continuefunc();
		return;
	}

	cout << "Registration ID | Couple Names\n";
	cout << "----------------------------------------\n";
	for (const auto& reg : registrations) {
		cout << left << setw(15) << reg.registrationID
			<< " | " << reg.manName << " & " << reg.womanName << "\n";
	}
	cout << "----------------------------------------\n";
	cout << "Total registrations: " << registrations.size() << "\n\n";
	cin.ignore();
	continuefunc();
}
bool hasExistingBooking(const vector<Booking>& bookings, const string& registrationID) {
	for (const auto& booking : bookings) {
		if (booking.registrationID == registrationID) {
			return true;
		}
	}
	return false;
}
