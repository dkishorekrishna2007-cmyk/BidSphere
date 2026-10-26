#ifndef BIDDER_H
#define BIDDER_H

#include "Auction.hpp"
#include <ctime>
#include <string>
#include <vector>

using namespace std;
class Bidder;
class Bid_rec{
    int auctionID;
    double amount;
    time_t bid_time;
    friend class Bidder;
};

class Bidder{

    int bidder_ID;
    string bidder_name;
    vector<Bid_rec> history;

public:
    
    Bidder(int id, string name);
    int getbidder_ID();
    string getbidder_name();
    int getTotalBids();
    void addBid_rec(int auctionID, double amount);
    bool placeBid(AuctionManager &manager,int auctionID,double amount);
    bool valid_bid(AuctionManager &manager,int auctionID,double amount);
    void displayhistory();
    void display_all_bids(int auctionID);
    bool hasbid(int auctionID);
};

#endif
