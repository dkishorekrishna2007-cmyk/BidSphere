# BidSphere — Online Auction and Bidding System

## 1. Project Overview

BidSphere is a database-driven Online Auction and Bidding System developed in C++17 for an Object-Oriented Programming (OOP) course project.

The system is intended to allow users to participate in auctions, create auction listings, place bids, and manage auction outcomes through a graphical user interface.

**Project status:** Initial setup and Qt GUI starter project.

## 2. Technology Stack

- **Programming language:** C++17
- **GUI framework:** Qt 6 Widgets
- **Build system:** CMake
- **Version control:** Git and GitHub
- **IDE:** Qt Creator or Visual Studio Code
- **Database:** To be decided by the team

## 3. Project Structure

```text
BidSphere/
├── .gitignore
├── CMakeLists.txt
├── README.md
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── include/       # Shared header files
├── src/           # C++ implementation files
├── tests/         # Tests
└── docs/          # Design diagrams and documentation
```

Build directories and IDE-specific configuration files are generated locally and should not be committed.

## 4. Prerequisites

To build the current Qt application, install:

- Git
- Qt 6 with Qt Widgets
- Qt Creator
- A compatible C++ compiler and CMake

On Windows, use the Qt kit and compiler selected during the Qt installation. Team members should not assume that every compiler is binary-compatible with every Qt build.

## 5. Getting Started

Clone the repository:

```bash
git clone https://github.com/dkishorekrishna2007-cmyk/BidSphere.git
cd BidSphere
```

Open `CMakeLists.txt` in Qt Creator.

Select the compatible Qt 6 MinGW 64-bit kit, configure the project, and then build and run it.

## 6. Team Responsibilities

- **Member 1 — User Management:** User, Buyer, Seller, and UserManager.
- **Member 2 — Auction Management:** Auction and AuctionManager.
- **Member 3 — Bidding Engine:** Bid and BiddingEngine.
- **D Kishore Krishna 4 — GUI, Storage, and Integration:** Qt interface, persistence, integration, testing, documentation, and packaging.

The exact interfaces and database design must be agreed upon by the team before implementation begins.

## 7. Collaboration Guidelines

1. Pull the latest changes before starting work.
2. Create a separate branch for each task or feature.
3. Avoid committing generated build files or personal IDE settings.
4. Do not edit another member's module without coordinating first.
5. Agree on class interfaces and shared data structures before integrating modules.
6. Test changes locally before pushing.
7. Open a pull request and request a teammate's review before merging into `main`.

## 8. Current Status

- [x] C++ compiler tested
- [x] Qt starter application builds and runs
- [x] Git repository initialized and connected to GitHub
- [x] Initial repository structure created
- [ ] Database and schema finalized
- [ ] Module interfaces finalized
- [ ] Application features implemented
- [ ] Integration and testing completed
