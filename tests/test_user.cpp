#include "UserManager.h"

#include <iostream>

using namespace std;


int main()
{
    UserManager manager;

    cout << "====================================\n";
    cout << "   BIDSPHERE USER MANAGEMENT TEST\n";
    cout << "====================================\n\n";


    // ----------------------------------
    // REGISTER BUYER
    // ----------------------------------

    cout << "Registering Buyer...\n";

    bool result1 = manager.registerUser(
        "dhanush",
        "1234",
        Role::BUYER
    );

    cout << "Buyer registration: ";

    if (result1)
        cout << "SUCCESS\n";
    else
        cout << "FAILED\n";


    // ----------------------------------
    // REGISTER SELLER
    // ----------------------------------

    cout << "\nRegistering Seller...\n";

    bool result2 = manager.registerUser(
        "seller1",
        "abcd",
        Role::SELLER
    );

    cout << "Seller registration: ";

    if (result2)
        cout << "SUCCESS\n";
    else
        cout << "FAILED\n";


    // ----------------------------------
    // REGISTER SECOND BUYER
    // ----------------------------------

    bool result3 = manager.registerUser(
        "rahul",
        "5678",
        Role::BUYER
    );

    cout << "Second Buyer registration: ";

    if (result3)
        cout << "SUCCESS\n";
    else
        cout << "FAILED\n";


    // ----------------------------------
    // DUPLICATE USERNAME
    // ----------------------------------

    cout << "\nTesting duplicate username...\n";

    bool duplicate = manager.registerUser(
        "dhanush",
        "9999",
        Role::BUYER
    );

    if (!duplicate)
        cout << "Duplicate username rejected: SUCCESS\n";
    else
        cout << "Duplicate username rejected: FAILED\n";


    // ----------------------------------
    // LOGIN
    // ----------------------------------

    cout << "\nTesting login...\n";

    User* user = manager.login(
        "dhanush",
        "1234"
    );

    if (user != nullptr)
    {
        cout << "Login: SUCCESS\n";

        cout << "Username: "
             << user->getUsername()
             << endl;

        cout << "User ID: "
             << user->getUserId()
             << endl;
    }
    else
    {
        cout << "Login: FAILED\n";
    }


    // ----------------------------------
    // WRONG PASSWORD
    // ----------------------------------

    cout << "\nTesting wrong password...\n";

    User* wrongUser = manager.login(
        "dhanush",
        "wrong"
    );

    if (wrongUser == nullptr)
        cout << "Wrong password rejected: SUCCESS\n";
    else
        cout << "Wrong password rejected: FAILED\n";


    // ----------------------------------
    // FIND USER BY ID
    // ----------------------------------

    cout << "\nTesting findUserById...\n";

    User* found = manager.findUserById(1001);

    if (found != nullptr)
    {
        cout << "User found: SUCCESS\n";
        cout << "Username: "
             << found->getUsername()
             << endl;
    }
    else
    {
        cout << "User found: FAILED\n";
    }


    // ----------------------------------
    // FIND USER BY USERNAME
    // ----------------------------------

    cout << "\nTesting findUserByUsername...\n";

    User* foundName =
        manager.findUserByUsername("seller1");

    if (foundName != nullptr)
    {
        cout << "User found: SUCCESS\n";
        cout << "User ID: "
             << foundName->getUserId()
             << endl;
    }
    else
    {
        cout << "User found: FAILED\n";
    }


    // ----------------------------------
    // BUYER CHECK
    // ----------------------------------

    cout << "\nTesting Buyer role...\n";

    if (manager.isBuyer(1001))
        cout << "Buyer role: SUCCESS\n";
    else
        cout << "Buyer role: FAILED\n";


    // ----------------------------------
    // SELLER CHECK
    // ----------------------------------

    cout << "\nTesting Seller role...\n";

    if (manager.isSeller(1002))
        cout << "Seller role: SUCCESS\n";
    else
        cout << "Seller role: FAILED\n";


    // ----------------------------------
    // SAVE USERS
    // ----------------------------------

    cout << "\nSaving users...\n";

    manager.saveUsers("users.txt");

    cout << "Users saved successfully.\n";


    // ----------------------------------
    // LOAD USERS
    // ----------------------------------

    cout << "\nLoading users...\n";

    UserManager newManager;

    bool loaded =
        newManager.loadUsers("users.txt");

    if (loaded)
        cout << "Users loaded: SUCCESS\n";
    else
        cout << "Users loaded: FAILED\n";


    // ----------------------------------
    // LOGIN AFTER LOADING
    // ----------------------------------

    cout << "\nTesting login after loading...\n";

    User* loadedUser =
        newManager.login(
            "dhanush",
            "1234"
        );

    if (loadedUser != nullptr)
    {
        cout << "Login after loading: SUCCESS\n";

        cout << "Username: "
             << loadedUser->getUsername()
             << endl;

        cout << "User ID: "
             << loadedUser->getUserId()
             << endl;
    }
    else
    {
        cout << "Login after loading: FAILED\n";
    }


    // ----------------------------------
    // TEST NEXT USER ID
    // ----------------------------------

    cout << "\nTesting next user ID...\n";

    newManager.registerUser(
        "newuser",
        "9999",
        Role::BUYER
    );

    User* newUser =
        newManager.findUserByUsername("newuser");

    if (newUser != nullptr)
    {
        cout << "New user ID: "
             << newUser->getUserId()
             << endl;

        cout << "Expected: 1004\n";
    }


    cout << "\n====================================\n";
    cout << "          TEST COMPLETE\n";
    cout << "====================================\n";

    return 0;
}