# BidSphere — Online Auction and Bidding System

BidSphere is a **C++17 desktop application** for creating auction listings and placing bids. Developed as an Object-Oriented Programming (OOP) course project, it uses Qt 6 Widgets for its graphical user interface.

**Project status:** Core GUI workflow implemented and manually tested, including auction and highest-bid persistence across application restarts.

## Features

- **User registration and login** with buyer/seller role selection.
- **Seller workflow:** Create auction listings with an item name and starting price.
- **Buyer workflow:** View available auctions and place bids.
- **Bid validation:** Accepted bids update the highest bid; bids that do not exceed the current highest bid are rejected.
- **File-based persistence:** User and auction data are saved locally and loaded at startup.
- **GUI:** Screens for authentication, auction listing, auction creation, and bidding.

## Technology Stack

- **Language:** C++17
- **GUI:** Qt 6 Widgets
- **Build system:** CMake
- **IDE:** Qt Creator or another CMake-compatible IDE
- **Version control:** Git and GitHub
- **Persistence:** Local delimited text files; no external database is configured.

## Project Structure

```text
BidSphere/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── include/
│   ├── Auction.hpp
│   ├── User.h
│   ├── UserManager.h
│   └── bidding.hpp
├── src/
│   ├── Auction.cpp
│   ├── User.cpp
│   ├── UserManager.cpp
│   └── bidding.cpp
└── tests/
    ├── smoke_test.cpp
    └── test_user.cpp
```

## Prerequisites

Install the following:

- Git
- Qt 6 with Qt Widgets
- A C++17-compatible compiler matching your Qt kit
- CMake

The project has been built using **Qt 6.12.0 MinGW 64-bit** and its matching MinGW compiler on Windows.

## Getting Started

### 1. Clone the repository

```cmd
git clone https://github.com/dkishorekrishna2007-cmyk/BidSphere.git
cd BidSphere
```

### 2. Build using Qt Creator

1. Open `CMakeLists.txt` in Qt Creator.
2. Select a compatible Qt 6 kit.
3. Configure the project.
4. Build the `BidSphere` target.
5. Run the application.

### 3. Build using Windows Command Prompt

The following commands match the tested Qt installation paths. Adjust them if Qt is installed elsewhere.

```cmd
set "PATH=C:\Qt\6.12.0\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\CMake_64\bin;%PATH%"
cmake -S . -B build\Desktop_Qt_6_12_0_MinGW_64_bit_Debug -DCMAKE_PREFIX_PATH=C:\Qt\6.12.0\mingw_64
cmake --build build\Desktop_Qt_6_12_0_MinGW_64_bit_Debug --config Debug
```

Run the executable from CMD so Qt can find its runtime DLLs:

```cmd
set "PATH=C:\Qt\6.12.0\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%"
build\Desktop_Qt_6_12_0_MinGW_64_bit_Debug\BidSphere.exe
```

## Data Persistence

The application stores local data in:

- `bidsphere_users.dat`
- `bidsphere_auctions.dat`

These files are placed beside the running executable. Keep them to retain local user and auction data between launches, and back them up before manually editing or deleting them.

### Current limitations

- Passwords are stored in plaintext. This implementation is intended for coursework, not production use with real credentials.
- Bidder history is session-only and does not persist across restarts.
- Auction duration is fixed at 24 hours.
- The GUI does not currently provide a close-auction control.
- Persistence is file-based; no database server or schema is configured.

## Verification

The following core workflow has been manually tested:

- Seller registration and login
- Auction creation
- Buyer registration and login
- Valid bid accepted
- Lower bid rejected
- Auction remains visible after restarting
- Highest bid remains saved after restarting

Test source files are included in `tests/`. They are separate from the GUI application's CMake target.

## Object-Oriented Design

- **`User` and `UserManager`:** User data, registration, login, and role lookup.
- **`Auction` and `AuctionManager`:** Auction details and auction management.
- **`Bidder`:** Bid submission and validation against the current highest bid.
- **Qt main window:** User-facing screens and coordination between GUI actions and application logic.

## Collaboration Guidelines

1. Pull the latest changes before starting work.
2. Coordinate changes to shared class interfaces.
3. Build and test locally before committing.
4. Avoid committing build output, executable files, local data files, or personal IDE settings.
5. Use descriptive commit messages and push tested changes.

## Repository

[GitHub — BidSphere](https://github.com/dkishorekrishna2007-cmyk/BidSphere)
