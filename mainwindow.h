#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <memory>

#include "UserManager.h"
#include "Auction.hpp"
#include "bidding.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Auth screen
    void onRegister();
    void onLogin();

    // Auction list screen
    void onRefreshAuctions();
    void onGoCreateAuction();
    void onGoPlaceBid();
    void onLogout();

    // Create auction screen (seller)
    void onCreateAuction();
    void onCancelCreate();

    // Place bid screen (buyer)
    void onPlaceBid();
    void onAuctionIdChanged(int id);
    void onCancelBid();

private:
    Ui::MainWindow *ui;

    // --- Application state ---
    UserManager    m_userManager;
    AuctionManager m_auctionManager;
    User*          m_currentUser   = nullptr;  // owned by m_userManager
    std::unique_ptr<Bidder> m_bidder;           // constructed after buyer login

    // --- Pages ---
    QStackedWidget* m_stack = nullptr;

    // Page 0: Login / Register
    QWidget*    m_authPage       = nullptr;
    QLineEdit*  m_usernameEdit   = nullptr;
    QLineEdit*  m_passwordEdit   = nullptr;
    QComboBox*  m_roleCombo      = nullptr;
    QLabel*     m_authStatus     = nullptr;

    // Page 1: Auction list
    QWidget*     m_listPage      = nullptr;
    QLabel*      m_listUserLabel = nullptr;
    QListWidget* m_auctionList   = nullptr;
    QPushButton* m_createBtn     = nullptr;
    QPushButton* m_bidBtn        = nullptr;

    // Page 2: Create auction (seller)
    QWidget*       m_createPage   = nullptr;
    QLineEdit*     m_itemEdit      = nullptr;
    QDoubleSpinBox* m_priceBox    = nullptr;
    QLabel*        m_createStatus = nullptr;

    // Page 3: Place bid (buyer)
    QWidget*       m_bidPage      = nullptr;
    QSpinBox*      m_auctionIdBox = nullptr;
    QDoubleSpinBox* m_bidAmountBox = nullptr;
    QLabel*        m_bidStatus    = nullptr;
    QLabel*        m_currentHighestLabel = nullptr;

    // --- Helpers ---
    void buildAuthPage();
    void buildListPage();
    void buildCreatePage();
    void buildBidPage();

    void showAuthPage();
    void showListPage();
    void showCreatePage();
    void showBidPage();

    void populateAuctionList();
    void refreshCurrentHighest(int auctionId);

    std::string USERS_FILE;
    std::string AUCTIONS_FILE;
};

#endif // MAINWINDOW_H
