#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
using namespace std;

struct ProposalPlan{
    string theme;
    string coupleNames;
    string date;
    string time;
    string venue;
    string secretName;
    string inviterName;
    string inviterPhoneNumber;
};

void displayEventMarketingMenu();

void displayProposalManagementMenu();
void createNewProposalPlan(vector<ProposalPlan> &plans);
bool isValidPhoneNumber(const string &inviterPhoneNumber);
void viewProposalPlan(const vector<ProposalPlan> &plans);
void editProposalDetails(vector<ProposalPlan> &plans);
void deleteProposalPlan(vector<ProposalPlan> &plans);
void saveProposalPlans(const vector<ProposalPlan> &plans, const string &filename);

void displayInvitationTemplatesMenu();
void viewAvailableTemplates(const vector<string> &templates);
void customizeTemplate(string templateStr);
void saveCustomizedTemplate(const string &content, const string &filename);

bool isValidPhoneNumber(const string &inviterPhoneNumber){
    if (inviterPhoneNumber.empty()) return false;

    for (size_t i = 0; i < inviterPhoneNumber.length(); i++){
        char c = inviterPhoneNumber[i];
        // Allow digits, +, spaces, and hyphens连字符号
        if (!isdigit(c) && c != '+' && c != ' ' && c != '-'){
            return false;
        }
        if (c == '+' && i != 0){
            return false; // '+' only allowed at the start
        }
    }
    return true;
}

// ====================== Event Marketing Menu ======================
void displayEventMarketingMenu(){
    cout << "\n\n======================================================\n";
    cout << "           Marriage Proposal Event Marketing\n";
    cout << "======================================================\n";
    cout << "1. Proposal Management\n";
    cout << "2. Invitation Templates\n";
    cout << "3. Back\n";
    cout << "Enter your choice: ";
}

// ===================== Proposal Management Menu =====================
void displayProposalManagementMenu(){
    cout << "\n\n======================================================\n";
    cout << "                  Proposal Management\n";
    cout << "======================================================\n";
    cout << "1. Create New Proposal Plan\n";
    cout << "2. View Proposal Plans\n";
    cout << "3. Edit Proposal Details\n";
    cout << "4. Delete Proposal Plan\n";
    cout << "5. Save Proposal Plan\n";
    cout << "6. Back\n";
    cout << "Enter your choice: ";
}

// ===================== Invitation Templates Menu =====================
void displayInvitationTemplatesMenu(){
    cout << "\n\n======================================================\n";
    cout << "                Invitation Templates\n";
    cout << "======================================================\n";
    cout << "1. View Available Templates\n";
    cout << "2. Select & Customize Template\n";
    cout << "3. Back\n";
    cout << "Enter your choice: ";
}

// ===================== Proposal Management Functions =====================
void createNewProposalPlan(vector<ProposalPlan> &plans){
    ProposalPlan proposalPlan;
    cout << "\nEnter Proposal Theme Name/Title: ";
    getline(cin, proposalPlan.theme);
    while (proposalPlan.theme.empty()){
        cout << "Theme cannot be empty. Enter proposal theme name/title: ";
        getline(cin, proposalPlan.theme);
    }
    cout << "Enter couple names: ";
    getline(cin, proposalPlan.coupleNames);
    cout << "Enter date (DD/MM/YYYY): ";
    getline(cin, proposalPlan.date);
    cout << "Enter time (HH:MM): ";
    getline(cin, proposalPlan.time);
    cout << "Enter venue/location: ";
    getline(cin, proposalPlan.venue);
    cout << "Enter name of the person to keep it secret from: ";
    getline(cin, proposalPlan.secretName);
    cout << "Enter inviter name: ";
    getline(cin, proposalPlan.inviterName);
    cout << "Enter inviter phone number: ";
    getline(cin, proposalPlan.inviterPhoneNumber);
    while (proposalPlan.inviterPhoneNumber.empty() || !isValidPhoneNumber(proposalPlan.inviterPhoneNumber)){
        cout << "Please enter a valid phone number (can include +, digits, spaces, hyphens): ";
        getline(cin, proposalPlan.inviterPhoneNumber);
    }
    plans.push_back(proposalPlan);
    cout << "Proposal plan added successfully!\n";
}

void viewProposalPlan(const vector<ProposalPlan> &plans){
    cout << "\n----- Proposal Plans List -----\n";
    cout << "Total Plans: " << plans.size() << "\n";
    if (plans.empty()){
        cout << "No proposal plans found.\n";
        return;
    }
    
    for (size_t i = 0; i < plans.size(); ++i){
        cout << "\n" << i + 1 << ". Theme: " << plans[i].theme 
             << "\n   Couple Names: " << plans[i].coupleNames
             << "\n   Date: " << plans[i].date 
             << "\n   Time: " << plans[i].time 
             << "\n   Venue: " << plans[i].venue
             << "\n   Secret From: " << plans[i].secretName
             << "\n   Inviter: " << plans[i].inviterName
             << "\n   Phone: " << plans[i].inviterPhoneNumber << "\n";
    }
}

void editProposalDetails(vector<ProposalPlan> &plans){
    if (plans.empty()){
        cout << "No plans to edit.\n";
        return;
    }
    
    viewProposalPlan(plans);
    cout << "Enter proposal number to edit: ";
    size_t idx;
    cin >> idx;
    cin.ignore();
    
    if (idx < 1 || idx > plans.size()){
        cout << "Invalid proposal number.\n";
        return;
    }
    
    ProposalPlan &plan = plans[idx - 1];
    string input;
    
    cout << "Edit proposal theme name (current: " << plan.theme << ", press enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) plan.theme = input;
    
    cout << "Edit couple names (current: " << plan.coupleNames << ", press enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) plan.coupleNames = input;
    
    cout << "Edit date (current: " << plan.date << ", press enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) plan.date = input;
    
    cout << "Edit time (current: " << plan.time << ", press enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) plan.time = input;
    
    cout << "Edit venue/location (current: " << plan.venue << ", press enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) plan.venue = input;
    
    cout << "Edit secret name (current: " << plan.secretName << ", press enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) plan.secretName = input;
    
    cout << "Edit inviter name (current: " << plan.inviterName << ", press enter to keep current): ";
    getline(cin, input);
    if (!input.empty()) plan.inviterName = input;
    
    cout << "Edit phone number (current: " << plan.inviterPhoneNumber << ", press enter to keep current): ";
    getline(cin, input);
    if (!input.empty() && isValidPhoneNumber(input)){
    plan.inviterPhoneNumber = input;
    }else if (!input.empty()){
        cout << "Invalid phone number format. Keeping current value.\n";
    }
    cout << "Proposal plan updated!\n";
}

void deleteProposalPlan(vector<ProposalPlan> &plans){
    if (plans.empty()){
        cout << "No plans to delete.\n";
        return;
    }
    
    viewProposalPlan(plans);
    cout << "Enter proposal number to delete: ";
    size_t idx;
    cin >> idx;
    cin.ignore();
    
    if (idx < 1 || idx > plans.size()){
        cout << "Invalid proposal number.\n";
        return;
    }
    
    plans.erase(plans.begin() + (idx - 1));
    cout << "Proposal plan deleted successfully!\n";
}

void saveProposalPlans(const vector<ProposalPlan> &plans, const string &filename){
    ofstream outFile(filename, ios::trunc);
    if (!outFile){
        cout << "Error opening file for saving.\n";
        return;
    }
    
    // Write header
    outFile << left << setw(25) << "PROPOSAL THEME"
            << "|" << setw(20) << "COUPLE NAMES"
            << "|" << setw(15) << "DATE"
            << "|" << setw(10) << "TIME"
            << "|" << setw(30) << "VENUE"
            << "|" << setw(20) << "SECRET FROM"
            << "|" << setw(15) << "INVITER"
            << "|" << setw(15) << "PHONE NUMBER"
            << "\n";
    
    // Add separator line
    outFile << string(156, '-') << "\n";
    
    // Write data
    for (const auto &plan : plans){
        outFile << left << setw(25) << plan.theme
                << "|" << setw(20) << plan.coupleNames
                << "|" << setw(15) << plan.date
                << "|" << setw(10) << plan.time
                << "|" << setw(30) << plan.venue
                << "|" << setw(20) << plan.secretName
                << "|" << setw(15) << plan.inviterName
                << "|" << setw(15) << plan.inviterPhoneNumber
                << "\n";
    }
    
    outFile.close();
    cout << "Proposal plans saved to " << filename << "\n";
}

// ===================== Invitation Templates =====================
const vector<string> proposalTemplates = {
    // Template 1: Classic Elegance
    "Template 1: Classic Elegance\n"
    "=============================================\n"
    "        MARRIAGE PROPOSAL INVITATION\n"
    "=============================================\n"
    "    You are cordially invited to witness\n"    
    "  a moment that will change my life forever.\n\n"
    "Theme: [Theme]\n"
    "Date: [Date]\n"
    "Time: [Time]\n"
    "Venue: [Venue]\n\n"
    "Your presence will make this moment complete.\n"
    "Please keep this a secret from [Secret]!\n"
    "RSVP to [Inviter] ([Phone])\n"
    "=============================================",
    
    // Template 2: Romantic Style
    "Template 2: Romantic Style\n"
    "**************************************************\n"
    "               A NIGHT TO REMEMBER\n"
    "**************************************************\n"
    "    Under the stars, a question will be asked,\n"
    "     and a new journey of love will begin...\n\n"
    "Theme: [Theme]\n"
    "Date: [Date]\n"
    "Time: [Time]\n"
    "Venue: [Venue]\n\n"
    "We wish for you to share in this intimate surprise.\n"
    "Shh... please keep it a secret from [Secret]!\n"
    "Contact: [Inviter] ([Phone])\n"
    "**************************************************",
    
    // Template 3: Modern Minimalist
    "Template 3: Modern Minimalist\n"
    "------------------------------------------------\n"
    "      YOU'RE INVITED TO A MARRIAGE PROPOSAL\n"
    "------------------------------------------------\n"
    "  Join us for an unforgettable moment of love\n"
    "as I ask the most important question of my life.\n\n"
    "Theme: [Theme]\n"
    "Date: [Date]\n"
    "Time: [Time]\n"
    "Venue: [Venue]\n\n"
    "Your presence means the world.\n"
    "Please do not tell [Secret] -> it's a surprise!\n"
    "Reach me: [Inviter] ([Phone])\n"
    "------------------------------------------------"
};

// ===================== Invitation Template Functions =====================
void viewAvailableTemplates(const vector<string> &templates){
    vector<vector<string>> lines(3);
    size_t maxLines = 0;
    
    for (size_t i = 0; i < templates.size(); ++i){
        istringstream iss(templates[i]);
        string line;
        while (getline(iss, line)){
            lines[i].push_back(line);
        }
        if (lines[i].size() > maxLines)
            maxLines = lines[i].size();
    }
    
    cout << "\n===============================================================================================================================================================\n";
    cout << "                                                                   INVITATION TEMPLATES\n";
    cout << "===============================================================================================================================================================\n\n";
    
    for (size_t row = 0; row < maxLines; row++){
        cout << left << setw(50) << (row < lines[0].size() ? lines[0][row] : "")
             << " | " << left << setw(55) << (row < lines[1].size() ? lines[1][row] : "")
             << " | " << left << setw(55) << (row < lines[2].size() ? lines[2][row] : "")
             << endl;
    }
}

void customizeTemplate(string templateStr){
    string theme, date, time, venue, secretName, inviterName, inviterPhoneNumber;
    
    cout << "\n----- Customize Your Invitation -----\n";
    cout << "Theme: ";
    getline(cin, theme);
    cout << "Date: ";
    getline(cin, date);
    cout << "Time: ";
    getline(cin, time);
    cout << "Venue: ";
    getline(cin, venue);
    cout << "Keep this secret from: ";
    getline(cin, secretName);
    cout << "Inviter name: ";
    getline(cin, inviterName);
    cout << "Inviter Phone Number: ";
    getline(cin, inviterPhoneNumber);

    // Lambda function to replace all occurrences of a substring
    auto replaceAll = [](string &str, const string &from, const string &to){
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != string::npos){
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    };
    
    replaceAll(templateStr, "[Theme]", theme);
    replaceAll(templateStr, "[Date]", date);
    replaceAll(templateStr, "[Time]", time);
    replaceAll(templateStr, "[Venue]", venue);
    replaceAll(templateStr, "[Secret]", secretName);
    replaceAll(templateStr, "[Inviter]", inviterName);
    replaceAll(templateStr, "[Phone]", inviterPhoneNumber);

    cout << "\nYour Customized Invitation:\n\n"
         << templateStr << endl;

    char save;
    cout << "\nDo you want to save this invitation as a .txt file? (y/n): ";
    cin >> save;
    cin.ignore();
    
    if (save == 'y' || save == 'Y'){
        string filename;
        cout << "Enter filename (e.g., invitation.txt): ";
        getline(cin, filename);
        
        ofstream outFile(filename);
        if (!outFile){
            cout << "Error saving file.\n";
        }else{
            outFile << templateStr << endl;
            outFile.close();
            cout << "Invitation saved to " << filename << "\n";
        }
    }
}

void saveCustomizedTemplate(const string &content, const string &filename){
    ofstream outFile(filename);
    if (!outFile){
        cout << "Error saving file.\n";
        return;
    }
    
    outFile << content << endl;
    outFile.close();
    cout << "Template saved to " << filename << "\n";
}

// ===================== Main Function =====================
int main(){
    vector<ProposalPlan> plans;
    int mainChoice;
    
    cout << "Welcome to Marriage Proposal Event Management System!\n";
    
    do{
        displayEventMarketingMenu();
        cin >> mainChoice;
        cin.ignore();
        
        if (mainChoice == 1){
            int proposalChoice;
            do{
                displayProposalManagementMenu();
                cin >> proposalChoice;
                cin.ignore();
                
                switch (proposalChoice){
                case 1:
                    createNewProposalPlan(plans);
                    break;
                case 2:
                    viewProposalPlan(plans);
                    break;
                case 3:
                    editProposalDetails(plans);
                    break;
                case 4:
                    deleteProposalPlan(plans);
                    break;
                case 5:
                    saveProposalPlans(plans, "proposal_plans.txt");
                    break;
                case 6:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
                }
            }while (proposalChoice != 6);
        }else if (mainChoice == 2){
            int invChoice;
            do{
                displayInvitationTemplatesMenu();
                cin >> invChoice;
                cin.ignore();
                
                switch (invChoice){
                case 1:
                    viewAvailableTemplates(proposalTemplates);
                    break;
                case 2:{
                    int selected;
                    cout << "Select template (1-3): ";
                    cin >> selected;
                    cin.ignore();

                    if (selected >= 1 && selected <= 3){
                        customizeTemplate(proposalTemplates[selected - 1]);
                    }else{
                        cout << "Invalid template selection. Please choose 1-3.\n";
                    }
                    break;
                }
                case 3:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
                }
            }while (invChoice != 3);
        }else if (mainChoice == 3){
            cout << "Exiting Event Marketing Menu...\n";
        }else{
            cout << "Invalid choice. Please try again.\n";
        }
        
    }while (mainChoice != 3);
    
    cout << "Thank you for using Marriage Proposal Event Management System!\n";
    return 0;
}
