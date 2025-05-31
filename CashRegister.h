// Header file for CashRegister class
#ifndef CASHREGISTER_H
#define CASHREGISTER_H

#include "Item.h"
#include <vector>
#include <map>
using namespace std;

class CashRegister
{
private:
    vector<Item> inventory;
    map<string, int> cart; // itemID -> quantity in cart
    double totalAmount;
    double discountRate;

public:
    // Constructor
    CashRegister();

    // Inventory Management
    void loadInventory();
    void saveInventory();
    void addNewItem(const Item &newItem);
    void editItem(const string &itemID, const string &newName,
                  double newPrice, int newQty);
    void deleteItem(const string &itemID);
    Item *searchItem(const string &itemID);
    void displayInventory() const;

    // Customer Operations
    void addItemToCart(const string &itemID, int quantity);
    void removeItemFromCart(const string &itemID);
    void viewCart() const;
    double calculateTotal();
    double calculateTotal() const; // const version for receipt generation
    void applyDiscount(double discount);
    void processPayment(double payment);
    void generateReceipt() const;
    void clearCart();

    // Admin Mode
    void adminMode();
    void customerMode();

    // Utility
    void initializeDefaultItems();
    void displayMainMenu() const;
    void displayCustomerMenu() const;
    void displayAdminMenu() const;
};

#endif