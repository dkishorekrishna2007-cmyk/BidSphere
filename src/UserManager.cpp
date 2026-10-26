#include "UserManager.h"

#include <fstream>
#include <sstream>

using namespace std;


UserManager::UserManager()
{
    nextUserId = 1001;
}


// --------------------------------------
// Check if username already exists
// --------------------------------------

bool UserManager::usernameExists(
    const string& username) const
{
    for (const auto& user : users)
    {
        if (user->getUsername() == username)
        {
            return true;
        }
    }

    return false;
}


// --------------------------------------
// Register User
// --------------------------------------

bool UserManager::registerUser(
    const string& username,
    const string& password,
    Role role)
{
    // Empty username/password
    if (username.empty() || password.empty())
    {
        return false;
    }

    // Duplicate username
    if (usernameExists(username))
    {
        return false;
    }

    // Create Buyer
    if (role == Role::BUYER)
    {
        users.push_back(
            make_unique<Buyer>(
                nextUserId,
                username,
                password
            )
        );
    }

    // Create Seller
    else if (role == Role::SELLER)
    {
        users.push_back(
            make_unique<Seller>(
                nextUserId,
                username,
                password
            )
        );
    }

    else
    {
        return false;
    }

    nextUserId++;

    return true;
}


// --------------------------------------
// Login
// --------------------------------------

User* UserManager::login(
    const string& username,
    const string& password)
{
    for (const auto& user : users)
    {
        if (user->getUsername() == username &&
            user->checkPassword(password))
        {
            return user.get();
        }
    }

    return nullptr;
}


// --------------------------------------
// Find user by ID
// --------------------------------------

User* UserManager::findUserById(
    int userId) const
{
    for (const auto& user : users)
    {
        if (user->getUserId() == userId)
        {
            return user.get();
        }
    }

    return nullptr;
}


// --------------------------------------
// Find user by username
// --------------------------------------

User* UserManager::findUserByUsername(
    const string& username) const
{
    for (const auto& user : users)
    {
        if (user->getUsername() == username)
        {
            return user.get();
        }
    }

    return nullptr;
}


// --------------------------------------
// Check Buyer
// --------------------------------------

bool UserManager::isBuyer(int userId) const
{
    User* user = findUserById(userId);

    if (user == nullptr)
    {
        return false;
    }

    return user->getRole() == Role::BUYER;
}


// --------------------------------------
// Check Seller
// --------------------------------------

bool UserManager::isSeller(int userId) const
{
    User* user = findUserById(userId);

    if (user == nullptr)
    {
        return false;
    }

    return user->getRole() == Role::SELLER;
}


// --------------------------------------
// Save users
// --------------------------------------

void UserManager::saveUsers(
    const string& filename) const
{
    ofstream file(filename);

    if (!file.is_open())
    {
        return;
    }

    for (const auto& user : users)
    {
        file << user->serialize() << endl;
    }

    file.close();
}


// --------------------------------------
// Load users
// --------------------------------------

bool UserManager::loadUsers(
    const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    users.clear();

    string line;

    int highestId = 1000;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);

        string idString;
        string username;
        string password;
        string roleString;

        getline(ss, idString, '|');
        getline(ss, username, '|');
        getline(ss, password, '|');
        getline(ss, roleString, '|');

        if (idString.empty() ||
            username.empty() ||
            password.empty())
        {
            continue;
        }

        int id = stoi(idString);

        if (roleString == "BUYER")
        {
            users.push_back(
                make_unique<Buyer>(
                    id,
                    username,
                    password
                )
            );
        }
        else if (roleString == "SELLER")
        {
            users.push_back(
                make_unique<Seller>(
                    id,
                    username,
                    password
                )
            );
        }

        if (id > highestId)
        {
            highestId = id;
        }
    }

    nextUserId = highestId + 1;

    file.close();

    return true;
}