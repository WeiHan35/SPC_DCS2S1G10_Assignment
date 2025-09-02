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


string checkPaid(bool paid);
void saveRegistrationsToFile(const vector<EventRegistration>& reg);
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
    int mainChoice;
    rewriteToVector(reg); // Load existing registrations from file
	loadBookingsFromFile(bookings, bookingStatus); // Load existing bookings from file

    while (true) {
        clearScreen();
        displayMenu();
        cin >> mainChoice;
        
        if (cin.fail()) {
            cout << "Invalid input! Please enter a number.\n";
            clearInput();
            continue;
        }
        
        // Move the switch statement INSIDE the loop
        switch (mainChoice) {
        case 1:
            clearScreen();
            displayRegistrationMenu();
            registrationChoice(reg);
            break;
        case 2:
            booking(reg, bookings, bookingStatus, venueNames, timeSlots, MAX_DATES);
            break;
        case 0:
            cout << "Exiting program." << endl;
            return 0;
        default:
            cout << "Invalid choice. Try again." << endl;
            continuefunc();
            break;
        }
    }

    return 0;
}
void displayMenu() {
	cout << "=============================\n";
	cout << " Event Management System\n";
	cout << "=============================\n";
	cout << "1. Event Registration\n";
	cout << "2. Event booking on dates and logistic\n";
	cout << "0. Exit\n";
	cout << "Enter your choice: ";
}
void displayRegistrationMenu() {
	cout << "Event Registration System" << endl;
	cout << "1. Add New Registration" << endl;
	cout << "2. Search Registration" << endl;
	cout << "3. Delete Registration" << endl;
	cout << "4. Update Registration" << endl;
	cout << "0. Back to Main Menu" << endl;
	cout << "5. Exit" << endl;
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
		searchRegistrationByID(reg);
		break;
	case 3:
		deleteRegistrationByID(reg);
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


void addRegistration(vector<EventRegistration>& reg) {
	clearScreen();
	EventRegistration registration = inputRegistration();
	int newID = reg.size() + 1001;
	registration.registrationID = to_string(newID);
	reg.push_back(registration);
	string paid = checkPaid(registration.paymentInfo.paid);

	ofstream outputFile("registrations.txt");
	for (const auto& r : reg) {
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
	clearScreen();
	cout << "Registration added successfully with ID: " << registration.registrationID << endl;
	//output the added registration details
	outputRegistration(registration);
	cout << "Press enter key to continue....";
	string enter;
	getline(cin, enter);
}


void searchRegistrationByID(vector<EventRegistration>& reg) {
	clearScreen();
	string id, enter;
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
	string id, enter;
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
			saveRegistrationsToFile(reg); // Use the helper function
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
		return;
	}

	if (inputFile.peek() == ifstream::traits_type::eof()) {

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
			ofstream outputFile("registrations.txt");
			for (const auto& x : reg) {
				outputFile << x.registrationID << ","
					<< x.manName << ","
					<< x.womanName << ","
					<< x.phone << ","
					<< x.email << ","
					<< x.numberOfGuests << ","
					<< x.packageType << ","
					<< x.specialRequests << ","
					<< x.paymentInfo.method << ","
					<< paid << ","
					<< x.paymentInfo.amount << ","
					<< endl;
			}
			cout << "Registration updated successfully.\n";
			return;
		}
	}

	cout << "No Registration found with this ID.\n";
	cout << "Press enter key to continue....";
	getline(cin, enter);
}
string checkPaid(bool paid) {
	if (paid) {
		return "Paid";
	}
	return "Pending Payment";
}

// ===== Validation Functions =====
string inputName(const string& prompt) {
	string name;

	regex pattern("^[A-Za-z ]+$");
	while (true) {

		cout << prompt;
		cin.ignore();

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
	int pkg;
	while (true) {
		cout << "Choose a Package Type\n1. Surprise Proposal (Base: RM1200)\n2. Romantic Dinner(Base:RM1500)\n3. Family Gathering(Base:RM1500)\nEnter package type : ";

		cin >> pkg;
		if (cin.fail() || pkg < 1 || pkg > 3) {
			cout << "Invalid package choice. Enter 1, 2, or 3.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}


		if
			(pkg == 1) return "Surprise Proposal";
		else if (pkg == 2) return "Romantic Dinner";
		else if (pkg == 3) return "Family Gathering";

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
	cin.clear();
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