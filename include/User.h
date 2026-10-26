#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

enum class Role
{
    BUYER,
    SELLER
};

class User
{
protected:
    int userId;
    string username;
    string password;
    Role role;

public:
    User(int id, const string& username,
         const string& password, Role role);

    int getUserId() const;
    string getUsername() const;
    Role getRole() const;

    bool checkPassword(const string& password) const;

    string serialize() const;

    virtual ~User() = default;
};


class Buyer : public User
{
public:
    Buyer(int id, const string& username,
          const string& password);
};


class Seller : public User
{
public:
    Seller(int id, const string& username,
           const string& password);
};

#endif