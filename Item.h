// Header file for Item class

#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>
using namespace std;

class Item
{
private:
    string itemID;
    string name;
    double price;
    int quantity;

public:
    // Constructors
    Item();
    Item(string id, string itemName, double itemPrice, int itemQty);

    // Getters
    string getItemID() const;
    string getName() const;
    double getPrice() const;
    int getQuantity() const;

    // Setters
    void setItemID(const string &id);
    void setName(const string &itemName);
    void setPrice(double itemPrice);
    void setQuantity(int itemQty);

    // Display method
    void displayItem() const;

    // Utility methods
    bool isInStock() const;
    void reduceQuantity(int amount);
    void increaseQuantity(int amount);
};

#endif