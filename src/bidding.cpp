
#include "bidding.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

Bidder :: Bidder(int id, string name){
    bidder_ID = id;
    bidder_name = name;
}

bool Bidder ::hasbid(int auctionID){
    for (const auto& bid : history) if (bid.auctionID == auctionID) return true;

    return false;
}

bool Bidder:: valid_bid(AuctionManager& manager, int auctionID, double amount){  

    Auction* auction = manager.getAuctionByID(auctionID);
    if (auction == nullptr || !auction->isActive()) return false;
    if (amount <= auction->getHighestBid()) return false;

    return true;
}

void Bidder :: addBid_rec(int auctionID, double amount){
    Bid_rec bid;
    bid.auctionID = auctionID;
    bid.amount = amount;
    bid.bid_time = time(nullptr);
    history.push_back(bid);
}

bool Bidder :: placeBid(AuctionManager& manager, int auctionID, double amount){
    Auction* auction = manager.getAuctionByID(auctionID);
    if (auction && auction->tryUpdateHighestBid(amount, bidder_ID)){
        addBid_rec(auctionID, amount);
        return true;
    }

    return false;
}

void Bidder :: display_all_bids(int auctionID){
    bool found = false;

    for (auto& bid : history)
    {
        if (bid.auctionID == auctionID)
        {
            found = true;
            cout << "Auction ID: " << bid.auctionID << endl << "Amount: " << fixed << setprecision(2) << bid.amount << endl;
            cout << "Time: ";
            tm* timeInfo = localtime(&bid.bid_time);

            if (timeInfo) cout << put_time(timeInfo, "%Y-%m-%d %H:%M:%S") << "\n\n";
        }
    }

    if (!found) cout << "No bids found for this auction " << auctionID << endl;
}

void Bidder :: displayhistory(){
    if (history.empty())
    {
        cout << "No bids placed .\n";
        return;
    }

    cout << "Bidder ID: " << bidder_ID << endl;
    cout << "Bidder Name: " << bidder_name << endl;

    for (size_t i = 0; i < history.size(); i++)
    {
        const Bid_rec& bid = history[i];
        cout << "\nBid SR NO.) " << i + 1 << endl;
        cout << "Auction ID : " << bid.auctionID << endl;
        cout << "Amount : " << fixed << setprecision(2) << bid.amount << endl;
        cout << "Time : ";

        tm* timeInfo = localtime(&bid.bid_time);

        if (timeInfo){
            cout << put_time(timeInfo, "%Y-%m-%d %H:%M:%S") << endl;
        }
    }
}

int Bidder::getTotalBids(){
    return history.size();
}

int Bidder::getbidder_ID(){
    return bidder_ID;
}

string Bidder::getbidder_name(){
    return bidder_name;
}
