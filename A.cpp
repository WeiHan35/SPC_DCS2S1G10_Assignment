// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>
using namespace std;

int main() {
	vector <string> appointment = { "10:30", "14:15", "16:00", "18:30" };


	appointment.erase(appointment.begin() + 2);
	appointment.insert(appointment.begin() + 1, "16:20");
	appointment[2] = "16:20";


	for (int i = 0; i < appointment.size(); i++) {
		if (appointment[i] == "16:20") {
			appointment.insert(appointment.begin() + i + 1, "17.10");
			break;
		}
	}

// I try edit la //

	for (int i = 0; i < appointment.size(); i++) {
		if (appointment[i] == "10:30") {
			appointment.erase(appointment.begin() + i);
			break;
		}
	}
	cout << "aaa";
	cout << "aaa";

	for (string app : appointment) {
		cout << app << endl;
		break;
	}
	return 0;

}
