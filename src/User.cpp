#include "User.h"

User::User(int id, const string& username,
           const string& password, Role role)
{
    this->userId = id;
    this->username = username;
    this->password = password;
    this->role = role;
}

int User::getUserId() const
{
    return userId;
}

string User::getUsername() const
{
    return username;
}

Role User::getRole() const
{
    return role;
}

bool User::checkPassword(const string& password) const
{
    return this->password == password;
}

string User::serialize() const
{
    string roleName;

    if (role == Role::BUYER)
    {
        roleName = "BUYER";
    }
    else
    {
        roleName = "SELLER";
    }

    return to_string(userId) + "|" +
           username + "|" +
           password + "|" +
           roleName;
}


Buyer::Buyer(int id, const string& username,
             const string& password)
    : User(id, username, password, Role::BUYER)
{
}


Seller::Seller(int id, const string& username,
               const string& password)
    : User(id, username, password, Role::SELLER)
{
}