// Item cpp
#include "Item.h"
#include <iostream>
#include <iomanip>
using namespace std;

Item::Item() : itemID(""), name(""), price(0.0), quantity(0) {}

Item::Item(string id, string itemName, double itemPrice, int itemQty)
    : itemID(id), name(itemName), price(itemPrice), quantity(itemQty) {}

string Item::getItemID() const { return itemID; }
string Item::getName() const { return name; }
double Item::getPrice() const { return price; }
int Item::getQuantity() const { return quantity; }

void Item::setItemID(const string &id) { itemID = id; }
void Item::setName(const string &itemName) { name = itemName; }
void Item::setPrice(double itemPrice) { price = itemPrice; }
void Item::setQuantity(int itemQty) { quantity = itemQty; }

void Item::displayItem() const
{
    cout << left << setw(8) << itemID
         << setw(20) << name
         << "$" << setw(8) << fixed << setprecision(2) << price
         << "Stock: " << quantity << endl;
}

bool Item::isInStock() const
{
    return quantity > 0;
}

void Item::reduceQuantity(int amount)
{
    if (amount <= quantity)
    {
        quantity -= amount;
    }
}

void Item::increaseQuantity(int amount)
{
    quantity += amount;
}