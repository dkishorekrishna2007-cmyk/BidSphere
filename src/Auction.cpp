
#include "Auction.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

Auction::Auction(int id, int seller, string name, double price)
{
    auctionID = id;
    sellerID = seller;
    itemName = name;
    startingPrice = price;
    highestBid = price;
    highestBidderID = -1;
    active = true;

    endTime = time(nullptr) + 24 * 60 * 60;
}

int Auction::getAuctionID()
{
    return auctionID;
}

int Auction::getSellerID()
{
    return sellerID;
}

string Auction::getItemName()
{
    return itemName;
}

double Auction::getStartingPrice()
{
    return startingPrice;
}

double Auction::getHighestBid()
{
    return highestBid;
}

int Auction::getHighestBidderID()
{
    return highestBidderID;
}

bool Auction::isActive()
{
    checkDeadline();
    return active;
}

time_t Auction::getEndTime()
{
    return endTime;
}

void Auction::checkDeadline()
{
    if (active && time(nullptr) >= endTime)
    {
        active = false;
    }
}

bool Auction::tryUpdateHighestBid(double amount, int bidderID)
{
    checkDeadline();

    if (!active)
    {
        cout << "Auction is closed or has expired.\n";
        return false;
    }

    if (bidderID <= 0)
    {
        cout << "Invalid bidder ID.\n";
        return false;
    }

    if (bidderID == sellerID)
    {
        cout << "Seller cannot bid on their own item.\n";
        return false;
    }

    if (amount <= highestBid)
    {
        cout << "Bid must be higher than the current highest bid.\n";
        return false;
    }

    highestBid = amount;
    highestBidderID = bidderID;

    cout << "Bid placed successfully!\n";
    return true;
}

void Auction::updateHighestBid(double amount, int bidderID)
{
    tryUpdateHighestBid(amount, bidderID);
}

void Auction::closeAuction()
{
    active = false;
    cout << "Auction " << auctionID << " is now closed.\n";
}

void Auction::restoreState(double highest, int bidder,
                           bool status, time_t end)
{
    highestBid = highest;
    highestBidderID = bidder;
    active = status;
    endTime = end;

    checkDeadline();
}

void Auction::saveToFile(ofstream &file)
{
    file << auctionID << '\t'
         << sellerID << '\t'
         << itemName << '\t'
         << startingPrice << '\t'
         << highestBid << '\t'
         << highestBidderID << '\t'
         << active << '\t'
         << endTime << '\n';
}

AuctionManager::AuctionManager()
{
    nextAuctionID = 101;
}

void AuctionManager::createAuction(int sellerID, string itemName,
                                   double startingPrice)
{
    if (sellerID <= 0)
    {
        cout << "Invalid seller ID.\n";
        return;
    }

    if (itemName.empty())
    {
        cout << "Item name cannot be empty.\n";
        return;
    }

    if (startingPrice <= 0)
    {
        cout << "Starting price must be greater than zero.\n";
        return;
    }

    Auction auction(nextAuctionID, sellerID,
                    itemName, startingPrice);

    auctions.push_back(auction);

    cout << "Auction created successfully!\n";
    cout << "Auction ID: " << nextAuctionID << '\n';

    nextAuctionID++;
}

Auction *AuctionManager::getAuctionByID(int auctionID)
{
    for (size_t i = 0; i < auctions.size(); i++)
    {
        if (auctions[i].getAuctionID() == auctionID)
        {
            return &auctions[i];
        }
    }

    return nullptr;
}

void AuctionManager::displayAuctions()
{
    if (auctions.empty())
    {
        cout << "No auctions available.\n";
        return;
    }

    for (size_t i = 0; i < auctions.size(); i++)
    {
        Auction &auction = auctions[i];

        cout << "\n-------------------------\n";
        cout << "Auction ID: "
             << auction.getAuctionID() << '\n';

        cout << "Seller ID: "
             << auction.getSellerID() << '\n';

        cout << "Item: "
             << auction.getItemName() << '\n';

        cout << fixed << setprecision(2);

        cout << "Starting Price: "
             << auction.getStartingPrice() << '\n';

        cout << "Highest Bid: "
             << auction.getHighestBid() << '\n';

        cout << "Highest Bidder ID: "
             << auction.getHighestBidderID() << '\n';

        cout << "Status: "
             << (auction.isActive() ? "Active" : "Closed")
             << '\n';

        cout << "-------------------------\n";
    }
}

void AuctionManager::searchAuction(int auctionID)
{
    Auction *auction = getAuctionByID(auctionID);

    if (auction == nullptr)
    {
        cout << "Auction not found.\n";
        return;
    }

    cout << "\nAuction ID: "
         << auction->getAuctionID() << '\n';

    cout << "Item: "
         << auction->getItemName() << '\n';

    cout << "Seller ID: "
         << auction->getSellerID() << '\n';

    cout << fixed << setprecision(2);

    cout << "Starting Price: "
         << auction->getStartingPrice() << '\n';

    cout << "Highest Bid: "
         << auction->getHighestBid() << '\n';

    cout << "Highest Bidder ID: "
         << auction->getHighestBidderID() << '\n';

    cout << "Status: "
         << (auction->isActive() ? "Active" : "Closed")
         << '\n';
}

void AuctionManager::closeAuction(int auctionID)
{
    Auction *auction = getAuctionByID(auctionID);

    if (auction == nullptr)
    {
        cout << "Auction not found.\n";
        return;
    }

    auction->closeAuction();
}

const vector<Auction>& AuctionManager::getAuctions() const
{
    return auctions;
}

void AuctionManager::saveAuctions(string filename)
{
    ofstream file(filename);

    if (!file)
    {
        cout << "Error opening file for saving.\n";
        return;
    }

    for (size_t i = 0; i < auctions.size(); i++)
    {
        auctions[i].saveToFile(file);
    }

    cout << "Auction data saved successfully.\n";
}

void AuctionManager::loadAuctions(string filename)
{
    ifstream file(filename);

    if (!file)
    {
        cout << "No saved auction data found.\n";
        return;
    }

    auctions.clear();
    nextAuctionID = 101;

    string line;

    while (getline(file, line))
    {
        istringstream input(line);

        int id, seller, bidder;
        double price, highest;
        bool status;
        time_t end;
        string name;

        if (!(input >> id >> seller))
        {
            continue;
        }

        if (input.peek() == '\t')
        {
            input.get();
        }

        if (!getline(input, name, '\t'))
        {
            continue;
        }

        if (!(input >> price))
        {
            continue;
        }

        if (input.peek() == '\t')
        {
            input.get();
        }

        if (!(input >> highest))
        {
            continue;
        }

        if (input.peek() == '\t')
        {
            input.get();
        }

        if (!(input >> bidder))
        {
            continue;
        }

        if (input.peek() == '\t')
        {
            input.get();
        }

        if (!(input >> status))
        {
            continue;
        }

        if (input.peek() == '\t')
        {
            input.get();
        }

        if (!(input >> end))
        {
            continue;
        }

        Auction auction(id, seller, name, price);

        auction.restoreState(highest, bidder, status, end);

        auctions.push_back(auction);

        if (id >= nextAuctionID)
        {
            nextAuctionID = id + 1;
        }
    }

    cout << "Auction data loaded successfully.\n";
}
