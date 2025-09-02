#include "EventMarketing.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
using namespace std;

// Function to display the marketing menu
void displayMarketingMenu() {
    cout << "\n\n================================================================\n";
    cout << "                     Event Marketing Module     ";
    cout << "\n================================================================\n";
    cout << "1. Add New Campaign\n";
    cout << "2. View Campaigns\n";
    cout << "3. Update Campaign\n";
    cout << "4. Track Campaign Performance\n";
    cout << "5. Save Campaigns\n";
    cout << "6. Upload Existing Campaigns\n";
    cout << "7. View & Customised Invitation Templates\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

// Function to add a new campaign with input validation
void addNewCampaign(vector<MarketingCampaign>& campaigns) {
    MarketingCampaign campaign;
    cout << "Enter campaign name: ";
    getline(cin, campaign.name);
    while (campaign.name.empty()) {
        cout << "Name cannot be empty. Enter campaign name: ";
        getline(cin, campaign.name);
    }
    cout << "Enter campaign marketing type (Social Media/Email/Flyer): ";
    getline(cin, campaign.type);
    cout << "Enter start date (YYYY-MM-DD): ";
    getline(cin, campaign.startDate);
    cout << "Enter end date (YYYY-MM-DD): ";
    getline(cin, campaign.endDate);
    cout << "Enter budget: ";
    while (!(cin >> campaign.budget) || campaign.budget <= 0) {
        cout << "Budget must be a positive number. Enter budget: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(); // Clear newline
    campaign.status.reach = 0;
    campaign.status.engagement = 0;
    campaigns.push_back(campaign);
    cout << "Campaign added successfully!\n";
}

// Function to view all campaigns
void viewCampaigns(const vector<MarketingCampaign>& campaigns) {
    cout << "\n----- Campaigns List -----\n";
    for (size_t i = 0; i < campaigns.size(); ++i) {
        cout << i+1 << ". " << campaigns[i].name << " | Type: " << campaigns[i].type
                  << " | Dates: " << campaigns[i].startDate << " to " << campaigns[i].endDate
                  << " | Budget: RM" << fixed << setprecision(2) << campaigns[i].budget << "\n";
        displayStatus(campaigns[i].status);
    }
    if (campaigns.empty()) cout << "No campaigns found.\n";
}

// Function to update a campaign
void updateCampaign(vector<MarketingCampaign>& campaigns) {
    if (campaigns.empty()) {
        cout << "No campaigns to update.\n";
        return;
    }
    viewCampaigns(campaigns);
    cout << "Enter campaign number to update: ";
    size_t idx;
    cin >> idx;
    cin.ignore();
    if (idx < 1 || idx > campaigns.size()) {
        cout << "Invalid campaign number.\n";
        return;
    }
    MarketingCampaign& campaign = campaigns[idx-1];
    cout << "Update campaign name (current: " << campaign.name << "): ";
    getline(cin, campaign.name);
    cout << "Update campaign marketing type (current: " << campaign.type << "): ";
    getline(cin, campaign.type);
    cout << "Update start date (current: " << campaign.startDate << "): ";
    getline(cin, campaign.startDate);
    cout << "Update end date (current: " << campaign.endDate << "): ";
    getline(cin, campaign.endDate);
    cout << "Update budget (current: RM" << campaign.budget << "): ";
    while (!(cin >> campaign.budget) || campaign.budget <= 0) {
        cout << "Budget must be a positive number. Enter budget: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();
    cout << "Campaign updated!\n";
}

// Function to track campaign performance
void trackCampaignPerformance(vector<MarketingCampaign>& campaigns) {
    if (campaigns.empty()) {
        cout << "No campaigns to track.\n";
        return;
    }
    viewCampaigns(campaigns);
    cout << "Enter campaign number to track campaign performance: ";
    size_t idx;
    cin >> idx;
    cin.ignore();
    if (idx < 1 || idx > campaigns.size()) {
        cout << "Invalid campaign number.\n";
        return;
    }
    MarketingCampaign& campaign = campaigns[idx-1];
    cout << "Enter reach (number of people who receive the invitation): ";
    while (!(cin >> campaign.status.reach) || campaign.status.reach < 0) {
        cout << "Reach must be non-negative. Enter reach: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Enter engagement (number of people who interacted with the campaign): ";
    while (!(cin >> campaign.status.engagement) || campaign.status.engagement < 0) {
        cout << "Engagement must be non-negative. Enter engagement: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();
    cout << "Status updated!\n";
}

// Function to save campaigns to a file
void saveCampaigns(const vector<MarketingCampaign>& campaigns, const string& filename) {
    // Check if file is empty to write header only once
    ifstream checkFile(filename);
    bool hasHeader = false;
    string firstLine;
    if (getline(checkFile, firstLine)) {
        if (firstLine.find("Campaign Name") != string::npos) hasHeader = true;
    }
    checkFile.close();

    ofstream outFile(filename, ios::trunc);
    if (!outFile) {
        cout << "Error opening file for saving.\n";
        return;
    }
    if (!hasHeader) {
        outFile << left << setw(20) << "CAMPAIGN NAME"
                << "|" << setw(22) << "CAMPAIGN MARKETING TYPE"
                << "|" << setw(13) << "START DATE"
                << "|" << setw(12) << "END DATE"
                << "|" << setw(12) << "BUDGET"
                << "|" << setw(8) << "REACH"
                << "|" << setw(10) << "ENGAGEMENT"
                << "\n";
    }
    for (const auto& c : campaigns) {
        outFile << left << setw(20) << c.name
                << "|" << setw(23) << c.type
                << "|" << setw(13) << c.startDate
                << "|" << setw(12) << c.endDate
                << "|" << setw(12) << fixed << setprecision(0) << c.budget
                << "|" << setw(8) << c.status.reach
                << "|" << setw(10) << c.status.engagement
                << "\n";
    }
    outFile.close();
    cout << "Campaigns saved to " << filename << "\n";
}

// Invitation template samples
const vector<string> defaultTemplates = {
    // Template 1: Wedding Ceremony
    "Template 1: Wedding Ceremony\n\n**********************************************\n              You're Invited!\n         Come celebrate love with us!\n\nDate: [Enter Date]\nVenue: [Enter Venue]\nTime: [Enter Time]\nSlogan: <Slogan>\n\nLet's make unforgettable memories together!\n**********************************************",
    // Template 2: Fun & Festive
    "Template 2: Fun & Festive\n\n===============================================\n      You're invited to join the celebration!\n                    <Slogan>\n\nDate: [Enter Date]\nVenue: [Enter Venue]\nTime: [Enter Time]\nTheme: <Theme>\n\nWe can't wait to have fun with you!\n===============================================",
    // Template 3: Party
    "Template 3: Party\n\n--------------------------------------------------\n               A Special Invitation\n              You are warmly invited!\n\nDate: [Enter Date]\nVenue: [Enter Venue]\nTime: [Enter Time]\nMessage: <Message>\n\nYour presence will make this day complete!\n--------------------------------------------------"
};

// Save templates to file
void saveTemplates(const vector<string>& templates, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error saving templates.\n";
        return;
    }
    for (size_t i = 0; i < templates.size(); ++i) {
        outFile << "Template " << (i+1) << ":\n" << templates[i] << "\n---\n";
    }
    outFile.close();
}

// Upload templates from file
vector<string> UploadTemplates(const string& filename) {
    ifstream inFile(filename);
    vector<string> templates;
    if (!inFile) {
        return defaultTemplates;
    }
    string line, temp;
    while (getline(inFile, line)) {
        if (line.find("Template ") == 0) {
            temp.clear();
        } else if (line == "---") {
            templates.push_back(temp);
        } else {
            temp += line + "\n";
        }
    }
    inFile.close();
    if (templates.empty()) return defaultTemplates;
    return templates;
}


// View & Customize Templates Function
void viewAndCustomizeTemplates(vector<string>& templates) {
    // Side-by-side display
    vector<vector<string>> lines(3);
    size_t maxLines = 0;
    for (size_t i = 0; i < templates.size(); ++i) {
        istringstream iss(templates[i]);
        string line;
        while (getline(iss, line)) {
            lines[i].push_back(line);
        }
        if (lines[i].size() > maxLines) maxLines = lines[i].size();
    }
    cout << "\n===========================================================================================================================================================\n";
    cout << "                                                                    Invitation Templates\n";
    cout << "===========================================================================================================================================================\n\n";
    for (size_t row = 0; row < maxLines; row++) {
        cout << left << setw(50) << (row < lines[0].size() ? lines[0][row] : "")
             << " | " << left << setw(50) << (row < lines[1].size() ? lines[1][row] : "")
             << " | " << left << setw(50) << (row < lines[2].size() ? lines[2][row] : "")
             << endl;
    }
    // Prompt user to pick a template
    int pick = 0;
    do {
        cout << "\nChoose a template (1, 2, or 3): ";
        cin >> pick;
        cin.ignore();
        if (pick < 1 || pick > 3) cout << "Invalid choice. Try again.\n";
    } while (pick < 1 || pick > 3);
    // Prompt for customization
    string date, venue, time, slogan, theme, message;
    cout << "Enter Date (YYYY-MM-DD): "; getline(cin, date);
    cout << "Enter Venue: "; getline(cin, venue);
    cout << "Enter Time: "; getline(cin, time);
    vector<string> customized;
    if (pick == 1) {
        cout << "Enter Slogan: "; getline(cin, slogan);
        for (const auto& line : lines[0]) {
            string l = line;
            size_t pos;
            if ((pos = l.find("[Enter Date]")) != string::npos) l.replace(pos, 12, date);
            if ((pos = l.find("[Enter Venue]")) != string::npos) l.replace(pos, 13, venue);
            if ((pos = l.find("[Enter Time]")) != string::npos) l.replace(pos, 12, time);
            if ((pos = l.find("<Slogan>")) != string::npos) l.replace(pos, 8, slogan);
            customized.push_back(l);
        }
    } else if (pick == 2) {
        cout << "Enter Slogan: "; getline(cin, slogan);
        cout << "Enter Theme: "; getline(cin, theme);
        for (const auto& line : lines[1]) {
            string l = line;
            size_t pos;
            if ((pos = l.find("[Enter Date]")) != string::npos) l.replace(pos, 12, date);
            if ((pos = l.find("[Enter Venue]")) != string::npos) l.replace(pos, 13, venue);
            if ((pos = l.find("[Enter Time]")) != string::npos) l.replace(pos, 12, time);
            if ((pos = l.find("<Slogan>")) != string::npos) l.replace(pos, 8, slogan);
            if ((pos = l.find("<Theme>")) != string::npos) l.replace(pos, 7, theme);
            customized.push_back(l);
        }
    } else if (pick == 3) {
        cout << "Enter Message: "; getline(cin, message);
        for (const auto& line : lines[2]) {
            string l = line;
            size_t pos;
            if ((pos = l.find("[Enter Date]")) != string::npos) l.replace(pos, 12, date);
            if ((pos = l.find("[Enter Venue]")) != string::npos) l.replace(pos, 13, venue);
            if ((pos = l.find("[Enter Time]")) != string::npos) l.replace(pos, 12, time);
            if ((pos = l.find("<Message>")) != string::npos) l.replace(pos, 9, message);
            customized.push_back(l);
        }
    }
    // Show customized invitation
    cout << "\nYour Customized Invitation:\n\n";
    for (const auto& line : customized) cout << line << endl;
    // Ask to save to file
    char save;
    cout << "\nDo you want to save this invitation as a .txt file? (y/n): ";
    cin >> save;
    cin.ignore();
    if (save == 'y' || save == 'Y') {
        string filename;
        cout << "Enter filename (e.g., invitation.txt): ";
        getline(cin, filename);
        ofstream outFile(filename);
        if (!outFile) {
            cout << "Error saving file.\n";
        } else {
            for (const auto& line : customized) outFile << line << endl;
            outFile.close();
            cout << "Invitation saved to " << filename << "\n";
        }
    }
}
void UploadCampaigns(vector<MarketingCampaign>& campaigns, const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "Error opening file for Uploading.\n";
        return;
    }
    campaigns.clear();
    string line;
    bool firstLine = true;
    while (getline(inFile, line)) {
        if (firstLine) { firstLine = false; continue; } // Skip header
        if (line.empty()) continue; // Skip empty lines
        MarketingCampaign c;
        size_t pos = 0, prev = 0;
        int field = 0;
        while ((pos = line.find('|', prev)) != string::npos) {
            string value = line.substr(prev, pos - prev);
            switch (field) {
                case 0: c.name = value; break;
                case 1: c.type = value; break;
                case 2: c.startDate = value; break;
                case 3: c.endDate = value; break;
                case 4: c.budget = stod(value); break;
                case 5: c.status.reach = stoi(value); break;
            }
            prev = pos + 1;
            ++field;
        }
        // Last field: engagement
        c.status.engagement = stoi(line.substr(prev));
        campaigns.push_back(c);
    }
    inFile.close();
    cout << "Campaigns Uploaded from " << filename << "\n";
}

// Template function to display campaign status
template <typename T>
void displayStatus(const T& status) {
    cout << "   Reach: " << status.reach << ", Engagement: " << status.engagement << "\n\n";
}

// Main function for testing (can be removed if integrating into main system)
int main() {
    vector<MarketingCampaign> campaigns;
    int choice;
    string filename = "marketing_campaigns.txt";
        vector<string> templates = UploadTemplates("invitation_templates.txt");
    do {
        displayMarketingMenu();
        cin >> choice;
        cin.ignore();
        switch (choice) {
            case 1: addNewCampaign(campaigns); break;
            case 2: viewCampaigns(campaigns); break;
            case 3: updateCampaign(campaigns); break;
            case 4: trackCampaignPerformance(campaigns); break;
            case 5: saveCampaigns(campaigns, filename); break;
            case 6: UploadCampaigns(campaigns, filename); break;
            case 7: viewAndCustomizeTemplates(templates); break;
            case 8: cout << "Exiting Event Marketing Module.\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);
    return 0;
}
