
#ifndef AUCTION_HPP
#define AUCTION_HPP

#include <string>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

class Auction
{
private:
    int auctionID;
    int sellerID;
    string itemName;
    double startingPrice;
    double highestBid;
    int highestBidderID;
    bool active;
    time_t endTime;

public:
    Auction(int id, int seller, string name, double price);

    int getAuctionID();
    int getSellerID();
    string getItemName();
    double getStartingPrice();
    double getHighestBid();
    int getHighestBidderID();
    bool isActive();
    time_t getEndTime();

    void checkDeadline();
    bool tryUpdateHighestBid(double amount, int bidderID);
    void updateHighestBid(double amount, int bidderID);
    void closeAuction();

    void restoreState(double highest, int bidder,
                      bool status, time_t end);

    void saveToFile(ofstream &file);
};

class AuctionManager
{
private:
    vector<Auction> auctions;
    int nextAuctionID;

public:
    AuctionManager();

    void createAuction(int sellerID, string itemName,
                       double startingPrice);

    Auction *getAuctionByID(int auctionID);

    void displayAuctions();
    void searchAuction(int auctionID);
    void closeAuction(int auctionID);

    const vector<Auction>& getAuctions() const;

    void saveAuctions(string filename);
    void loadAuctions(string filename);
};

#endif
