#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "User.h"
#include <vector>
#include <memory>
#include <string>

using namespace std;

class UserManager
{
private:

    vector<unique_ptr<User>> users;

    int nextUserId;

public:

    UserManager();

    // Registration
    bool registerUser(const string& username,
                      const string& password,
                      Role role);

    // Login
    User* login(const string& username,
                const string& password);

    // User search
    User* findUserById(int userId) const;

    User* findUserByUsername(
        const string& username) const;

    // Username validation
    bool usernameExists(
        const string& username) const;

    // Role checking
    bool isBuyer(int userId) const;

    bool isSeller(int userId) const;

    // File storage
    void saveUsers(const string& filename) const;

    bool loadUsers(const string& filename);
};

#endif