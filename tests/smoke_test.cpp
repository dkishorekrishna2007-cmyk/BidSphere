// smoke_test.cpp — CLI smoke test for BidSphere backend logic
// Exercises UserManager, AuctionManager, and Bidder directly.
// Compile with the same flags as the main project.
// No Qt required.

#include "UserManager.h"
#include "Auction.hpp"
#include "bidding.hpp"

#include <iostream>
#include <string>
#include <cassert>

static int passed = 0;
static int failed = 0;

static void check(bool condition, const char* label) {
    if (condition) {
        std::cout << "  PASS  " << label << "\n";
        ++passed;
    } else {
        std::cout << "  FAIL  " << label << "\n";
        ++failed;
    }
}

int main() {
    std::cout << "=== BidSphere Backend Smoke Test ===\n\n";

    // ─────────────────────────────────────────────
    // 1. UserManager — registration
    // ─────────────────────────────────────────────
    std::cout << "-- Registration --\n";
    UserManager um;
    check(um.registerUser("audit_seller1", "pass123", Role::SELLER),
          "Register seller succeeds");
    check(!um.registerUser("audit_seller1", "other", Role::SELLER),
          "Duplicate username rejected");
    check(!um.registerUser("", "pass", Role::BUYER),
          "Empty username rejected");
    check(!um.registerUser("user2", "", Role::BUYER),
          "Empty password rejected");
    check(um.registerUser("audit_buyer1", "buypass", Role::BUYER),
          "Register buyer succeeds");

    // ─────────────────────────────────────────────
    // 2. UserManager — login
    // ─────────────────────────────────────────────
    std::cout << "\n-- Login --\n";
    User* seller = um.login("audit_seller1", "pass123");
    check(seller != nullptr,               "Seller login succeeds");
    check(seller && seller->getRole() == Role::SELLER, "Seller role correct");

    User* badLogin = um.login("audit_seller1", "wrongpass");
    check(badLogin == nullptr,             "Wrong password rejected");

    User* buyer = um.login("audit_buyer1", "buypass");
    check(buyer != nullptr,                "Buyer login succeeds");
    check(buyer && buyer->getRole() == Role::BUYER, "Buyer role correct");

    // ─────────────────────────────────────────────
    // 3. isBuyer / isSeller role guards
    // ─────────────────────────────────────────────
    std::cout << "\n-- Role guards --\n";
    check(um.isSeller(seller->getUserId()), "isSeller correct for seller");
    check(!um.isBuyer(seller->getUserId()), "isBuyer false for seller");
    check(um.isBuyer(buyer->getUserId()),   "isBuyer correct for buyer");
    check(!um.isSeller(buyer->getUserId()), "isSeller false for buyer");

    // ─────────────────────────────────────────────
    // 4. AuctionManager — create auction
    // ─────────────────────────────────────────────
    std::cout << "\n-- Create auction --\n";
    AuctionManager am;
    int countBefore = (int)am.getAuctions().size();
    am.createAuction(seller->getUserId(), "Antique Clock", 50.0);
    int countAfter  = (int)am.getAuctions().size();
    check(countAfter == countBefore + 1,   "Auction count incremented");

    Auction* a = am.getAuctionByID(101);
    check(a != nullptr,                    "Auction 101 found by ID");
    check(a && a->getItemName() == "Antique Clock", "Item name correct");
    check(a && a->getStartingPrice() == 50.0,       "Starting price correct");
    check(a && a->getHighestBid()    == 50.0,       "Highest bid initialised to starting price");
    check(a && a->getHighestBidderID() == -1,       "No bidder initially (-1)");
    check(a && a->isActive(),                       "Auction is active");

    // Invalid creates
    int c0 = (int)am.getAuctions().size();
    am.createAuction(0, "Bad Auction", 10.0);   // invalid seller ID
    check((int)am.getAuctions().size() == c0,   "Invalid sellerID create ignored");
    am.createAuction(seller->getUserId(), "", 10.0); // empty name
    check((int)am.getAuctions().size() == c0,   "Empty item name create ignored");
    am.createAuction(seller->getUserId(), "X", 0.0); // zero price
    check((int)am.getAuctions().size() == c0,   "Zero price create ignored");

    // ─────────────────────────────────────────────
    // 5. Bidder — placeBid (valid)
    // ─────────────────────────────────────────────
    std::cout << "\n-- Bidding --\n";
    Bidder bidder(buyer->getUserId(), buyer->getUsername());
    bool accepted = bidder.placeBid(am, 101, 75.0);
    check(accepted,                         "Valid bid ($75 > $50) accepted");

    Auction* a2 = am.getAuctionByID(101);
    check(a2 && a2->getHighestBid() == 75.0,        "Highest bid updated to $75");
    check(a2 && a2->getHighestBidderID() == buyer->getUserId(),
          "Highest bidder ID updated to buyer");
    check(bidder.getTotalBids() == 1,               "Bid recorded in history");
    check(bidder.hasbid(101),                       "hasbid(101) returns true");

    // ─────────────────────────────────────────────
    // 6. Bidder — placeBid (invalid: too low)
    // ─────────────────────────────────────────────
    bool rejected = bidder.placeBid(am, 101, 50.0);   // equal to starting — below current
    check(!rejected,                        "Bid equal to start ($50 <= $75) rejected");
    check(a2 && a2->getHighestBid() == 75.0, "Highest bid unchanged after rejected bid");
    check(bidder.getTotalBids() == 1,        "History unchanged after rejected bid");

    bool rejectedExact = bidder.placeBid(am, 101, 75.0); // exactly equal to current
    check(!rejectedExact,                   "Bid exactly equal to current ($75) rejected");
    check(a2 && a2->getHighestBid() == 75.0, "Highest bid still $75");

    // ─────────────────────────────────────────────
    // 7. Seller cannot bid on their own auction
    // ─────────────────────────────────────────────
    std::cout << "\n-- Seller self-bid guard --\n";
    // tryUpdateHighestBid checks sellerID == bidderID
    Bidder sellerAsBidder(seller->getUserId(), seller->getUsername());
    bool selfBid = sellerAsBidder.placeBid(am, 101, 200.0);
    check(!selfBid,                         "Seller blocked from bidding on own auction");
    check(a2 && a2->getHighestBid() == 75.0, "Highest bid unchanged after seller self-bid attempt");

    // ─────────────────────────────────────────────
    // 8. Bid on non-existent auction
    // ─────────────────────────────────────────────
    std::cout << "\n-- Edge cases --\n";
    bool noAuction = bidder.placeBid(am, 999, 100.0);
    check(!noAuction,                       "Bid on non-existent auction ID rejected");

    // ─────────────────────────────────────────────
    // 9. Persistence — save then reload
    // ─────────────────────────────────────────────
    std::cout << "\n-- Persistence --\n";
    um.saveUsers("test_users.dat");
    am.saveAuctions("test_auctions.dat");

    UserManager um2;
    bool loaded = um2.loadUsers("test_users.dat");
    check(loaded,                                  "Users loaded from file");
    User* s2 = um2.login("audit_seller1", "pass123");
    check(s2 != nullptr,                           "Seller survives save/load");
    check(s2 && s2->getRole() == Role::SELLER,     "Seller role correct after reload");
    User* b2 = um2.login("audit_buyer1", "buypass");
    check(b2 != nullptr,                           "Buyer survives save/load");

    AuctionManager am2;
    am2.loadAuctions("test_auctions.dat");
    check(!am2.getAuctions().empty(),              "Auctions loaded from file");
    Auction* a3 = am2.getAuctionByID(101);
    check(a3 != nullptr,                           "Auction 101 survives save/load");
    check(a3 && a3->getHighestBid() == 75.0,       "Highest bid ($75) survives save/load");
    check(a3 && a3->getHighestBidderID() == buyer->getUserId(),
          "Highest bidder ID survives save/load");
    check(a3 && a3->getItemName() == "Antique Clock",
          "Item name survives save/load");

    // ─────────────────────────────────────────────
    // 10. getAuctions() accessor
    // ─────────────────────────────────────────────
    std::cout << "\n-- getAuctions() accessor --\n";
    const auto& auctions = am2.getAuctions();
    check(!auctions.empty(),                       "getAuctions() returns non-empty vector");

    // ─────────────────────────────────────────────
    // Summary
    // ─────────────────────────────────────────────
    std::cout << "\n========================================\n";
    std::cout << "  PASSED: " << passed << "\n";
    std::cout << "  FAILED: " << failed << "\n";
    std::cout << "========================================\n";
    return (failed == 0) ? 0 : 1;
}
