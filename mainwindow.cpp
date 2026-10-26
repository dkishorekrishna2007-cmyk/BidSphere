#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QString>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <QCoreApplication>

// ─────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("BidSphere — Auction Platform");
    resize(820, 600);

    // Central container
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    buildAuthPage();
    buildListPage();
    buildCreatePage();
    buildBidPage();

    m_stack->addWidget(m_authPage);    // index 0
    m_stack->addWidget(m_listPage);    // index 1
    m_stack->addWidget(m_createPage);  // index 2
    m_stack->addWidget(m_bidPage);     // index 3

    // Resolve absolute paths beside the executable so persistence is stable
    // regardless of the working directory when BidSphere.exe is launched.
    QString appDir = QCoreApplication::applicationDirPath();
    USERS_FILE    = (appDir + "/bidsphere_users.dat").toStdString();
    AUCTIONS_FILE = (appDir + "/bidsphere_auctions.dat").toStdString();

    // Load persisted data (failures are silently ignored on first run)
    m_userManager.loadUsers(USERS_FILE);
    m_auctionManager.loadAuctions(AUCTIONS_FILE);

    showAuthPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ─────────────────────────────────────────────
// Page builders
// ─────────────────────────────────────────────

void MainWindow::buildAuthPage()
{
    m_authPage = new QWidget;
    auto* outer = new QVBoxLayout(m_authPage);
    outer->setAlignment(Qt::AlignCenter);

    // Title
    auto* title = new QLabel("BidSphere");
    QFont f = title->font();
    f.setPointSize(22);
    f.setBold(true);
    title->setFont(f);
    title->setAlignment(Qt::AlignCenter);
    outer->addWidget(title);

    auto* sub = new QLabel("Online Auction Platform");
    sub->setAlignment(Qt::AlignCenter);
    outer->addWidget(sub);
    outer->addSpacing(16);

    // Form
    auto* box = new QGroupBox("Account");
    auto* form = new QFormLayout(box);

    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setPlaceholderText("Enter username");
    form->addRow("Username:", m_usernameEdit);

    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setPlaceholderText("Enter password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    form->addRow("Password:", m_passwordEdit);

    m_roleCombo = new QComboBox;
    m_roleCombo->addItem("Buyer",  static_cast<int>(Role::BUYER));
    m_roleCombo->addItem("Seller", static_cast<int>(Role::SELLER));
    form->addRow("Role:", m_roleCombo);

    outer->addWidget(box);
    outer->addSpacing(8);

    // Buttons
    auto* btnRow = new QHBoxLayout;
    auto* regBtn = new QPushButton("Register");
    auto* loginBtn = new QPushButton("Login");
    regBtn->setMinimumHeight(34);
    loginBtn->setMinimumHeight(34);
    btnRow->addWidget(regBtn);
    btnRow->addWidget(loginBtn);
    outer->addLayout(btnRow);

    // Status label
    m_authStatus = new QLabel;
    m_authStatus->setAlignment(Qt::AlignCenter);
    m_authStatus->setWordWrap(true);
    outer->addWidget(m_authStatus);

    connect(regBtn,   &QPushButton::clicked, this, &MainWindow::onRegister);
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLogin);
}

void MainWindow::buildListPage()
{
    m_listPage = new QWidget;
    auto* outer = new QVBoxLayout(m_listPage);

    // Header row
    auto* headerRow = new QHBoxLayout;
    m_listUserLabel = new QLabel("Logged in as: —");
    QFont f = m_listUserLabel->font();
    f.setBold(true);
    m_listUserLabel->setFont(f);
    headerRow->addWidget(m_listUserLabel);
    headerRow->addStretch();

    auto* logoutBtn = new QPushButton("Logout");
    headerRow->addWidget(logoutBtn);
    outer->addLayout(headerRow);

    outer->addWidget(new QLabel("Available Auctions:"));

    m_auctionList = new QListWidget;
    m_auctionList->setAlternatingRowColors(true);
    outer->addWidget(m_auctionList, 1);

    // Action row
    auto* actionRow = new QHBoxLayout;
    auto* refreshBtn = new QPushButton("Refresh");

    m_createBtn = new QPushButton("Create Auction");
    m_bidBtn    = new QPushButton("Place Bid");

    actionRow->addWidget(refreshBtn);
    actionRow->addStretch();
    actionRow->addWidget(m_createBtn);
    actionRow->addWidget(m_bidBtn);
    outer->addLayout(actionRow);

    connect(refreshBtn,  &QPushButton::clicked, this, &MainWindow::onRefreshAuctions);
    connect(m_createBtn, &QPushButton::clicked, this, &MainWindow::onGoCreateAuction);
    connect(m_bidBtn,    &QPushButton::clicked, this, &MainWindow::onGoPlaceBid);
    connect(logoutBtn,   &QPushButton::clicked, this, &MainWindow::onLogout);
}

void MainWindow::buildCreatePage()
{
    m_createPage = new QWidget;
    auto* outer = new QVBoxLayout(m_createPage);
    outer->setAlignment(Qt::AlignTop);

    auto* title = new QLabel("Create New Auction");
    QFont f = title->font();
    f.setPointSize(14);
    f.setBold(true);
    title->setFont(f);
    outer->addWidget(title);
    outer->addSpacing(8);

    auto* box  = new QGroupBox("Auction Details");
    auto* form = new QFormLayout(box);

    m_itemEdit = new QLineEdit;
    m_itemEdit->setPlaceholderText("e.g. Vintage Watch");
    form->addRow("Item Name:", m_itemEdit);

    m_priceBox = new QDoubleSpinBox;
    m_priceBox->setPrefix("$ ");
    m_priceBox->setRange(0.01, 1000000.00);
    m_priceBox->setDecimals(2);
    m_priceBox->setValue(1.00);
    form->addRow("Starting Price:", m_priceBox);

    outer->addWidget(box);
    outer->addSpacing(8);

    auto* btnRow = new QHBoxLayout;
    auto* createBtn = new QPushButton("Create Auction");
    auto* cancelBtn = new QPushButton("Back");
    createBtn->setMinimumHeight(32);
    cancelBtn->setMinimumHeight(32);
    btnRow->addWidget(createBtn);
    btnRow->addWidget(cancelBtn);
    outer->addLayout(btnRow);

    m_createStatus = new QLabel;
    m_createStatus->setWordWrap(true);
    outer->addWidget(m_createStatus);

    connect(createBtn, &QPushButton::clicked, this, &MainWindow::onCreateAuction);
    connect(cancelBtn, &QPushButton::clicked, this, &MainWindow::onCancelCreate);
}

void MainWindow::buildBidPage()
{
    m_bidPage = new QWidget;
    auto* outer = new QVBoxLayout(m_bidPage);
    outer->setAlignment(Qt::AlignTop);

    auto* title = new QLabel("Place a Bid");
    QFont f = title->font();
    f.setPointSize(14);
    f.setBold(true);
    title->setFont(f);
    outer->addWidget(title);
    outer->addSpacing(8);

    auto* box  = new QGroupBox("Bid Details");
    auto* form = new QFormLayout(box);

    m_auctionIdBox = new QSpinBox;
    m_auctionIdBox->setRange(101, 99999);
    m_auctionIdBox->setValue(101);
    form->addRow("Auction ID:", m_auctionIdBox);

    m_currentHighestLabel = new QLabel("—");
    form->addRow("Current Highest Bid:", m_currentHighestLabel);

    m_bidAmountBox = new QDoubleSpinBox;
    m_bidAmountBox->setPrefix("$ ");
    m_bidAmountBox->setRange(0.01, 1000000.00);
    m_bidAmountBox->setDecimals(2);
    m_bidAmountBox->setValue(1.00);
    form->addRow("Your Bid:", m_bidAmountBox);

    outer->addWidget(box);
    outer->addSpacing(8);

    auto* btnRow = new QHBoxLayout;
    auto* bidBtn    = new QPushButton("Place Bid");
    auto* cancelBtn = new QPushButton("Back");
    bidBtn->setMinimumHeight(32);
    cancelBtn->setMinimumHeight(32);
    btnRow->addWidget(bidBtn);
    btnRow->addWidget(cancelBtn);
    outer->addLayout(btnRow);

    m_bidStatus = new QLabel;
    m_bidStatus->setWordWrap(true);
    outer->addWidget(m_bidStatus);

    connect(bidBtn,    &QPushButton::clicked, this, &MainWindow::onPlaceBid);
    connect(cancelBtn, &QPushButton::clicked, this, &MainWindow::onCancelBid);
    connect(m_auctionIdBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onAuctionIdChanged);
}

// ─────────────────────────────────────────────
// Navigation helpers
// ─────────────────────────────────────────────

void MainWindow::showAuthPage()
{
    m_authStatus->clear();
    m_stack->setCurrentIndex(0);
}

void MainWindow::showListPage()
{
    QString roleStr = (m_currentUser && m_currentUser->getRole() == Role::SELLER)
                      ? "Seller" : "Buyer";
    m_listUserLabel->setText(
        QString("Logged in as: %1 (%2)")
            .arg(QString::fromStdString(m_currentUser->getUsername()))
            .arg(roleStr));

    bool isSeller = (m_currentUser && m_currentUser->getRole() == Role::SELLER);
    m_createBtn->setVisible(isSeller);
    m_bidBtn->setVisible(!isSeller);

    populateAuctionList();
    m_stack->setCurrentIndex(1);
}

void MainWindow::showCreatePage()
{
    m_itemEdit->clear();
    m_priceBox->setValue(1.00);
    m_createStatus->clear();
    m_stack->setCurrentIndex(2);
}

void MainWindow::showBidPage()
{
    m_bidStatus->clear();
    m_auctionIdBox->setValue(101);
    m_bidAmountBox->setValue(1.00);
    refreshCurrentHighest(101);
    m_stack->setCurrentIndex(3);
}

// ─────────────────────────────────────────────
// Auth slots
// ─────────────────────────────────────────────

void MainWindow::onRegister()
{
    std::string username = m_usernameEdit->text().toStdString();
    std::string password = m_passwordEdit->text().toStdString();
    int roleIndex = m_roleCombo->currentIndex();
    Role role = (roleIndex == 1) ? Role::SELLER : Role::BUYER;

    if (username.empty() || password.empty()) {
        m_authStatus->setStyleSheet("color: red;");
        m_authStatus->setText("Username and password are required.");
        return;
    }

    bool ok = m_userManager.registerUser(username, password, role);
    if (ok) {
        m_userManager.saveUsers(USERS_FILE);
        m_authStatus->setStyleSheet("color: green;");
        m_authStatus->setText("Registration successful. You can now log in.");
    } else {
        m_authStatus->setStyleSheet("color: red;");
        m_authStatus->setText("Registration failed: username already taken or invalid input.");
    }
}

void MainWindow::onLogin()
{
    std::string username = m_usernameEdit->text().toStdString();
    std::string password = m_passwordEdit->text().toStdString();

    if (username.empty() || password.empty()) {
        m_authStatus->setStyleSheet("color: red;");
        m_authStatus->setText("Username and password are required.");
        return;
    }

    User* user = m_userManager.login(username, password);
    if (!user) {
        m_authStatus->setStyleSheet("color: red;");
        m_authStatus->setText("Login failed: incorrect username or password.");
        return;
    }

    m_currentUser = user;  // pointer is owned by m_userManager — safe as long as the manager lives

    // Construct a Bidder only for buyers
    m_bidder.reset();
    if (m_currentUser->getRole() == Role::BUYER) {
        m_bidder = std::make_unique<Bidder>(
            m_currentUser->getUserId(),
            m_currentUser->getUsername());
    }

    showListPage();
}

// ─────────────────────────────────────────────
// Auction list slots
// ─────────────────────────────────────────────

void MainWindow::populateAuctionList()
{
    m_auctionList->clear();

    const auto& auctions = m_auctionManager.getAuctions();
    if (auctions.empty()) {
        m_auctionList->addItem("No auctions available.");
        return;
    }

    for (const Auction& a : auctions) {
        // Cast away const to call non-const getters — getters do not mutate
        Auction& mut = const_cast<Auction&>(a);

        QString status = mut.isActive() ? "Active" : "Closed";
        int highestBidder = mut.getHighestBidderID();
        QString bidderStr = (highestBidder == -1)
                            ? "none"
                            : QString::number(highestBidder);

        QString line = QString("[ID %1] %2 | Start: $%3 | Highest: $%4 | Bidder: %5 | %6")
                           .arg(mut.getAuctionID())
                           .arg(QString::fromStdString(mut.getItemName()))
                           .arg(mut.getStartingPrice(), 0, 'f', 2)
                           .arg(mut.getHighestBid(),    0, 'f', 2)
                           .arg(bidderStr)
                           .arg(status);
        m_auctionList->addItem(line);
    }
}

void MainWindow::onRefreshAuctions()
{
    populateAuctionList();
}

void MainWindow::onGoCreateAuction()
{
    if (!m_currentUser || m_currentUser->getRole() != Role::SELLER) {
        QMessageBox::warning(this, "Access Denied", "Only sellers can create auctions.");
        return;
    }
    showCreatePage();
}

void MainWindow::onGoPlaceBid()
{
    if (!m_currentUser || m_currentUser->getRole() != Role::BUYER) {
        QMessageBox::warning(this, "Access Denied", "Only buyers can place bids.");
        return;
    }
    showBidPage();
}

void MainWindow::onLogout()
{
    m_currentUser = nullptr;
    m_bidder.reset();
    m_usernameEdit->clear();
    m_passwordEdit->clear();
    showAuthPage();
}

// ─────────────────────────────────────────────
// Create auction slots
// ─────────────────────────────────────────────

void MainWindow::onCreateAuction()
{
    if (!m_currentUser || m_currentUser->getRole() != Role::SELLER) {
        m_createStatus->setStyleSheet("color: red;");
        m_createStatus->setText("Only sellers can create auctions.");
        return;
    }

    std::string item = m_itemEdit->text().trimmed().remove('\r').remove('\n').toStdString();
    double price     = m_priceBox->value();

    if (item.empty()) {
        m_createStatus->setStyleSheet("color: red;");
        m_createStatus->setText("Item name cannot be empty.");
        return;
    }

    // Count auctions before so we can find the new ID
    int sizeBefore = static_cast<int>(m_auctionManager.getAuctions().size());

    m_auctionManager.createAuction(m_currentUser->getUserId(), item, price);

    int sizeAfter = static_cast<int>(m_auctionManager.getAuctions().size());
    if (sizeAfter <= sizeBefore) {
        m_createStatus->setStyleSheet("color: red;");
        m_createStatus->setText("Auction creation failed (check seller ID and item name).");
        return;
    }

    // The new auction is the last element
    const auto& auctions = m_auctionManager.getAuctions();
    Auction& newAuction = const_cast<Auction&>(auctions.back());
    int newId = newAuction.getAuctionID();

    m_auctionManager.saveAuctions(AUCTIONS_FILE);

    m_createStatus->setStyleSheet("color: green;");
    m_createStatus->setText(
        QString("Auction created! ID: %1").arg(newId));
}

void MainWindow::onCancelCreate()
{
    showListPage();
}

// ─────────────────────────────────────────────
// Place bid slots
// ─────────────────────────────────────────────

void MainWindow::refreshCurrentHighest(int auctionId)
{
    Auction* a = m_auctionManager.getAuctionByID(auctionId);
    if (!a) {
        m_currentHighestLabel->setText("Auction not found");
        return;
    }
    QString status = a->isActive() ? "Active" : "Closed";
    m_currentHighestLabel->setText(
        QString("$%1 (%2)")
            .arg(a->getHighestBid(), 0, 'f', 2)
            .arg(status));
}

void MainWindow::onAuctionIdChanged(int id)
{
    refreshCurrentHighest(id);
}

void MainWindow::onPlaceBid()
{
    if (!m_currentUser || m_currentUser->getRole() != Role::BUYER || !m_bidder) {
        m_bidStatus->setStyleSheet("color: red;");
        m_bidStatus->setText("Only buyers can place bids.");
        return;
    }

    int    auctionId = m_auctionIdBox->value();
    double amount    = m_bidAmountBox->value();

    Auction* auction = m_auctionManager.getAuctionByID(auctionId);
    if (!auction) {
        m_bidStatus->setStyleSheet("color: red;");
        m_bidStatus->setText(QString("Auction ID %1 not found.").arg(auctionId));
        return;
    }

    // Prevent a seller from bidding on their own auction
    if (auction->getSellerID() == m_currentUser->getUserId()) {
        m_bidStatus->setStyleSheet("color: red;");
        m_bidStatus->setText("You cannot bid on your own auction.");
        return;
    }

    bool accepted = m_bidder->placeBid(m_auctionManager, auctionId, amount);

    if (accepted) {
        m_auctionManager.saveAuctions(AUCTIONS_FILE);
        refreshCurrentHighest(auctionId);
        m_bidStatus->setStyleSheet("color: green;");
        m_bidStatus->setText(
            QString("Bid accepted! New highest bid: $%1").arg(amount, 0, 'f', 2));
    } else {
        refreshCurrentHighest(auctionId);
        m_bidStatus->setStyleSheet("color: red;");
        m_bidStatus->setText(
            "Bid rejected. The auction may be closed, or your bid is not higher than the current highest.");
    }
}

void MainWindow::onCancelBid()
{
    showListPage();
}
