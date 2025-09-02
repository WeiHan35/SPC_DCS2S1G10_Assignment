/* h.(header) file is used to declare structures, functions, and templates that are used in the .cpp(source) file.*/

#ifndef EVENTMARKETING_H
#define EVENTMARKETING_H
#include <string>
#include <vector>
using namespace std;

// Structure for campaign statistics
struct CampaignStatus {
    int reach;
    int engagement;
};

// Structure for a marketing campaign
struct MarketingCampaign {
    string name;
    string type; // e.g., Social Media, Email, Flyer
    string startDate;
    string endDate;
    double budget;
    CampaignStatus status; // Nested structure
};

// Function declarations
void displayMarketingMenu();
void addNewCampaign(vector<MarketingCampaign>& campaigns);
void viewCampaigns(const vector<MarketingCampaign>& campaigns);
void updateCampaign(vector<MarketingCampaign>& campaigns);
void trackCampaignPerformance(vector<MarketingCampaign>& campaigns);
void saveCampaigns(const vector<MarketingCampaign>& campaigns, const string& filename);
void loadCampaigns(vector<MarketingCampaign>& campaigns, const string& filename);
template <typename T>
void displayStatus(const T& status);

#endif // EVENTMARKETING_H
