// Cash Register cpp

#include "CashRegister.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

int length = 20;

void clearScreenRUN()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayDollarBill()
{
    cout << endl;
    string asciiArt[] = {
        "╔══════════════════════ FEDERAL RESERVE NOTE ═════════════════════╗",
        "║ ┌─────┐  ╭──────────────────────────────────────────╮  ┌─────┐  ║",
        "║ │  1  │  │    ███╗   ██╗ ██████╗ ████████╗███████╗  │  │  1  │  ║",
        "║ └─────┘  │    ████╗  ██║██╔═══██╗╚══██╔══╝██╔════╝  │  └─────┘  ║",
        "║  SERIAL  │    ██╔██╗ ██║██║   ██║   ██║   █████╗    │   SERIAL  ║",
        "║ AC0123D  │    ██║╚██╗██║██║   ██║   ██║   ██╔══╝    │  DC210AB  ║",
        "║          │    ██║ ╚████║╚██████╔╝   ██║   ███████╗  │           ║",
        "║   ╭$╮    │    ╚═╝  ╚═══╝ ╚═════╝    ╚═╝   ╚══════╝  │    ╭$╮    ║",
        "║   │1│    ╰──────────────────────────────────────────╯    │1│    ║",
        "║   ╰$╯         THIS NOTE IS LEGAL TENDER FOR ALL          ╰$╯    ║",
        "║   ONE             DEBTS PUBLIC AND PRIVATE               ONE    ║",
        "╚═════════════════════════════════════════════════════════════════╝"};

    // Paper money themed gradient (grayish-green)
    for (size_t i = 0; i < 12; ++i)
    {
        float ratio = static_cast<float>(i) / 11.0f;
        // Paper money style coloring
        int green = 160 - static_cast<int>(20 * ratio); // Subtle green
        int red = 150 - static_cast<int>(20 * ratio);   // Close to green for gray
        int blue = 140 - static_cast<int>(20 * ratio);  // Slightly less for aged look

        cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m"
             << asciiArt[i] << "\033[0m" << endl;
    }
    cout << endl;
}

void displayCashRegisterShelf()
{
    cout << endl;
    string asciiArt[] = {
        "┏━━━━━━━━━━━━━━━  CASH REGISTER SYSTEM  ━━━━━━━━━━━━━━━━┓",
        "┃ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ ┃",
        "┃ │ $$$$ │ │ $$$$ │ │ $$$$ │ │ $$$$ │ │ $$$$ │ │ $$$$ │ ┃",
        "┃ │ $$$$ │ │ $$$$ │ │ $$$$ │ │ $$$$ │ │ $$$$ │ │ $$$$ │ ┃",
        "┃ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘ └──────┘ ┃",
        "┃  [OPEN]   [CASH]   [SALE]   [TOTAL]  [VOID]   [CLOSE] ┃",
        "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛"};

    // Money-themed gradient (golden-green)
    for (size_t i = 0; i < 7; ++i)
    {
        float ratio = static_cast<float>(i) / 6.0f;
        // Rich golden-green gradient
        int green = 220 - static_cast<int>(70 * ratio); // Dominant green
        int red = 180 - static_cast<int>(50 * ratio);   // Golden tint
        int blue = 60 + static_cast<int>(20 * ratio);   // Depth accent

        cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m"
             << asciiArt[i] << "\033[0m" << endl;
    }
    cout << endl;
}

CashRegister::CashRegister() : totalAmount(0.0), discountRate(0.0)
{
    loadInventory();
    if (inventory.empty())
    {
        initializeDefaultItems();
        saveInventory();
    }
}

void CashRegister::loadInventory()
{
    ifstream file("inventory.txt");
    if (!file.is_open())
    {
        cout << "Creating new inventory file..." << endl;
        return;
    }

    inventory.clear();
    string id, name;
    double price;
    int quantity;

    while (file >> id >> name >> price >> quantity)
    {
        inventory.push_back(Item(id, name, price, quantity));
    }
    file.close();
    cout << "Inventory loaded successfully!" << endl;
}

void CashRegister::saveInventory()
{
    ofstream file("inventory.txt");
    if (!file.is_open())
    {
        cout << "Error: Could not save inventory!" << endl;
        return;
    }

    for (const auto &item : inventory)
    {
        file << item.getItemID() << " " << item.getName() << " "
             << item.getPrice() << " " << item.getQuantity() << endl;
    }
    file.close();
}

void CashRegister::addNewItem(const Item &newItem)
{
    // Check if item already exists
    for (const auto &item : inventory)
    {
        if (item.getItemID() == newItem.getItemID())
        {
            cout << "Error: Item with ID " << newItem.getItemID() << " already exists!" << endl;
            return;
        }
    }
    inventory.push_back(newItem);
    saveInventory();
    cout << "Item added successfully!" << endl;
}

void CashRegister::editItem(const string &itemID, const string &newName,
                            double newPrice, int newQty)
{
    Item *item = searchItem(itemID);
    if (item != nullptr)
    {
        item->setName(newName);
        item->setPrice(newPrice);
        item->setQuantity(newQty);
        saveInventory();
        cout << "Item updated successfully!" << endl;
    }
    else
    {
        cout << "Error: Item not found!" << endl;
    }
}

void CashRegister::deleteItem(const string &itemID)
{
    auto it = find_if(inventory.begin(), inventory.end(),
                      [&itemID](const Item &item)
                      {
                          return item.getItemID() == itemID;
                      });

    if (it != inventory.end())
    {
        inventory.erase(it);
        saveInventory();
        cout << "Item deleted successfully!" << endl;
    }
    else
    {
        cout << "Error: Item not found!" << endl;
    }
}

Item *CashRegister::searchItem(const string &itemID)
{
    for (auto &item : inventory)
    {
        if (item.getItemID() == itemID)
        {
            return &item;
        }
    }
    return nullptr;
}

void CashRegister::displayInventory() const
{
    clearScreenRUN();
    displayCashRegisterShelf();
    cout << string(length - 5, ' ') << "=== CURRENT INVENTORY ===\n"
         << endl;
    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(10) << "Price"
         << "Stock" << endl;
    cout << string(50, '-') << endl;

    if (inventory.empty())
    {
        cout << "No items in inventory." << endl;
    }
    else
    {
        for (const auto &item : inventory)
        {
            item.displayItem();
        }
    }
    cout << endl;
}

void CashRegister::addItemToCart(const string &itemID, int quantity)
{
    Item *item = searchItem(itemID);
    if (item == nullptr)
    {
        cout << "Error: Item not found!" << endl;
        return;
    }

    if (quantity <= 0)
    {
        cout << "Error: Invalid quantity!" << endl;
        return;
    }

    if (item->getQuantity() < quantity)
    {
        cout << "Error: Not enough stock! Available: " << item->getQuantity() << endl;
        return;
    }

    cart[itemID] += quantity;
    item->reduceQuantity(quantity);
    cout << "Added " << quantity << " " << item->getName() << "(s) to cart!" << endl;
}

void CashRegister::removeItemFromCart(const string &itemID)
{
    auto it = cart.find(itemID);
    if (it != cart.end())
    {
        // Return items to inventory
        Item *item = searchItem(itemID);
        if (item != nullptr)
        {
            item->increaseQuantity(it->second);
        }
        cart.erase(it);
        cout << "Item removed from cart!" << endl;
    }
    else
    {
        cout << "Item not in cart!" << endl;
    }
}

void CashRegister::viewCart() const
{
    cout << string(length, ' ') << "=== YOUR CART ===\n"
         << endl;
    if (cart.empty())
    {
        cout << "Cart is empty." << endl;
        return;
    }

    cout << left << setw(8) << "ID"
         << setw(20) << "Name"
         << setw(10) << "Price"
         << setw(8) << "Qty"
         << "Subtotal" << endl;
    cout << string(60, '-') << endl;

    double total = 0.0;
    for (const auto &cartItem : cart)
    {
        Item *item = const_cast<CashRegister *>(this)->searchItem(cartItem.first);
        if (item != nullptr)
        {
            double subtotal = item->getPrice() * cartItem.second;
            total += subtotal;

            cout << left << setw(8) << item->getItemID()
                 << setw(20) << item->getName()
                 << "$" << setw(9) << fixed << setprecision(2) << item->getPrice()
                 << setw(8) << cartItem.second
                 << "$" << fixed << setprecision(2) << subtotal << endl;
        }
    }
    cout << string(60, '-') << endl;
    cout << "Total: $" << fixed << setprecision(2) << total << endl;
    cout << endl;
}

double CashRegister::calculateTotal()
{
    totalAmount = 0.0;
    for (const auto &cartItem : cart)
    {
        Item *item = searchItem(cartItem.first);
        if (item != nullptr)
        {
            totalAmount += item->getPrice() * cartItem.second;
        }
    }
    return totalAmount * (1.0 - discountRate);
}

double CashRegister::calculateTotal() const
{
    double tempTotal = 0.0;
    for (const auto &cartItem : cart)
    {
        Item *item = const_cast<CashRegister *>(this)->searchItem(cartItem.first);
        if (item != nullptr)
        {
            tempTotal += item->getPrice() * cartItem.second;
        }
    }
    return tempTotal * (1.0 - discountRate);
}

void CashRegister::applyDiscount(double discount)
{
    if (discount >= 0 && discount <= 1.0)
    {
        discountRate = discount;
        cout << "Discount of " << (discount * 100) << "% applied!" << endl;
    }
    else
    {
        cout << "Invalid discount rate!" << endl;
    }
}

void CashRegister::processPayment(double payment)
{
    double finalTotal = calculateTotal();
    if (payment >= finalTotal)
    {
        double change = payment - finalTotal;
        clearScreenRUN();
        displayDollarBill();
        cout << string(length - 9, ' ') << "=== PAYMENT PROCESSED ===\n"
             << endl;
        cout << "Payment received: $" << fixed << setprecision(2) << payment << endl;
        cout << "Change: $" << fixed << setprecision(2) << change << endl;
        generateReceipt();
        clearCart();
        saveInventory(); // Save updated inventory
    }
    else
    {
        cout << "Insufficient payment! Total: $" << fixed << setprecision(2) << finalTotal << endl;
        // Return items to inventory if payment fails
        for (const auto &cartItem : cart)
        {
            Item *item = searchItem(cartItem.first);
            if (item != nullptr)
            {
                item->increaseQuantity(cartItem.second);
            }
        }
    }
}

void CashRegister::generateReceipt() const
{
    cout << "\n"
         << string(50, '=') << endl;
    cout << "             CASH REGISTER RECEIPT" << endl;
    cout << string(50, '=') << endl;

    for (const auto &cartItem : cart)
    {
        Item *item = const_cast<CashRegister *>(this)->searchItem(cartItem.first);
        if (item != nullptr)
        {
            double subtotal = item->getPrice() * cartItem.second;
            cout << left << setw(20) << item->getName()
                 << setw(5) << cartItem.second << " x $"
                 << setw(8) << fixed << setprecision(2) << item->getPrice()
                 << " = $" << fixed << setprecision(2) << subtotal << endl;
        }
    }

    cout << string(50, '-') << endl;
    cout << left << setw(35) << "Subtotal:"
         << "$" << fixed << setprecision(2) << totalAmount << endl;

    if (discountRate > 0)
    {
        cout << left << setw(35) << "Discount:"
             << "-$" << fixed << setprecision(2) << (totalAmount * discountRate) << endl;
    }

    cout << left << setw(35) << "TOTAL:"
         << "$" << fixed << setprecision(2) << calculateTotal() << endl;
    cout << string(50, '=') << endl;
    cout << "            Thank you for your purchase!" << endl;
    cout << string(50, '=') << endl;
}

void CashRegister::clearCart()
{
    cart.clear();
    totalAmount = 0.0;
    discountRate = 0.0;
}

void CashRegister::initializeDefaultItems()
{
    inventory.push_back(Item("001", "Coffee", 2.50, 20));
    inventory.push_back(Item("002", "Sandwich", 5.99, 15));
    inventory.push_back(Item("003", "Chips", 1.25, 30));
    inventory.push_back(Item("004", "Soda", 1.75, 25));
    inventory.push_back(Item("005", "Candy", 0.99, 40));
    cout << "Default inventory initialized!" << endl;
}

void CashRegister::displayMainMenu() const
{
    clearScreenRUN();
    displayCashRegisterShelf();
    cout << string(length - 5, ' ') << "=== CASH REGISTER SYSTEM ===\n"
         << endl;
    cout << string(length - 1, ' ') << "1. Customer Mode" << endl;
    cout << string(length - 1, ' ') << "2. Admin Mode" << endl;
    cout << string(length - 1, ' ') << "3. Quit\n"
         << endl;
    cout << string(length - 1, ' ') << "Choose your role: ";
}

void CashRegister::displayCustomerMenu() const
{
    clearScreenRUN();
    displayCashRegisterShelf();
    length = 20;
    int deduct = 2;
    cout << string(length - 5, ' ') << "=== CUSTOMER MODE ===\n\n";
    cout << string(length - deduct, ' ') << "(A)dd item to cart" << endl;
    cout << string(length - deduct, ' ') << "(R)emove item from cart" << endl;
    cout << string(length - deduct, ' ') << "(V)iew cart" << endl;
    cout << string(length - deduct, ' ') << "(C)alculate total" << endl;
    cout << string(length - deduct, ' ') << "(D)iscount" << endl;
    cout << string(length - deduct, ' ') << "(P)ay" << endl;
    cout << string(length - deduct, ' ') << "(S)how inventory" << endl;
    cout << string(length - deduct, ' ') << "(Q)uit to main menu\n"
         << endl;
    cout << string(length - deduct, ' ') << "Enter choice: ";
}

void CashRegister::displayAdminMenu() const
{
    clearScreenRUN();
    displayCashRegisterShelf();
    cout << string(length - 1, ' ') << "=== ADMIN MODE ===" << endl;
    cout << string(length, ' ') << "1. Add new item" << endl;
    cout << string(length, ' ') << "2. Edit item" << endl;
    cout << string(length, ' ') << "3. Delete item" << endl;
    cout << string(length, ' ') << "4. Search item" << endl;
    cout << string(length, ' ') << "5. View inventory" << endl;
    cout << string(length, ' ') << "6. Back to main menu\n"
         << endl;
    cout << string(length, ' ') << "Enter choice: ";
}

void CashRegister::customerMode()
{
    char choice;
    string buffer;

    while (true)
    {
        displayCustomerMenu();

        // Get single character input
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        choice = toupper(choice);

        clearScreenRUN();
        displayCashRegisterShelf();

        switch (choice)
        {
        case 'A':
        {
            displayInventory();
            if (inventory.empty())
            {
                cout << "No items available in inventory!" << endl;
                break;
            }
            string itemID;
            int quantity;

            cout << "Enter item ID (or 'B' to go back): ";
            getline(cin, itemID);
            if (itemID == "B" || itemID == "b")
                break;

            cout << "Enter quantity (or 0 to go back): ";
            while (!(cin >> quantity) || quantity < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid quantity! Please enter a positive number (or 0 to go back): ";
            }
            if (quantity == 0)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            addItemToCart(itemID, quantity);
            break;
        }
        case 'R':
        {
            if (cart.empty())
            {
                cout << "Cart is empty!" << endl;
                break;
            }
            viewCart();
            string itemID;
            cout << "Enter item ID to remove (or 'B' to go back): ";
            getline(cin, itemID);
            if (itemID == "B" || itemID == "b")
                break;
            removeItemFromCart(itemID);
            break;
        }
        case 'V':
            viewCart();
            break;
        case 'C':
        {
            if (cart.empty())
            {
                cout << "Cart is empty!" << endl;
                break;
            }
            viewCart();
            double total = calculateTotal();
            cout << "Current total: $" << fixed << setprecision(2) << total << endl;
            break;
        }

        case 'D':
        {
            if (cart.empty())
            {
                cout << "Cart is empty!" << endl;
                break;
            }

            viewCart();

            string input;
            double discount = 0.0;

            cout << "Enter discount code (or type 'back' to cancel): ";
            getline(cin, input);

            if (input == "back")
                break;

            // Convert input to uppercase for consistency (optional)
            transform(input.begin(), input.end(), input.begin(), ::toupper);

            // Sample discount codes
            if (input == "SAVE10")
            {
                discount = 10.0;
            }
            else if (input == "WELCOME20")
            {
                discount = 20.0;
            }
            else if (input == "VIP30")
            {
                discount = 30.0;
            }
            else
            {
                cout << "Invalid discount code." << endl;
                break;
            }

            applyDiscount(discount / 100.0);
            cout << "Discount of " << discount << "% applied successfully!" << endl;
            break;
        }

        case 'P':
        {
            if (cart.empty())
            {
                cout << "Cart is empty!" << endl;
                break;
            }
            viewCart();
            double total = calculateTotal();
            cout << "Total amount: $" << fixed << setprecision(2) << total << endl;
            double payment;
            cout << "Enter payment amount (or -1 to go back): $";
            while (!(cin >> payment) || (payment != -1 && payment < total))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid payment! Amount must be at least $"
                     << fixed << setprecision(2) << total
                     << " (or -1 to go back): $";
            }
            if (payment == -1)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            processPayment(payment);
            break;
        }
        case 'S':
            displayInventory();
            break;
        case 'Q':
            return;
        default:
            cout << "Invalid choice! Please enter A, R, V, C, D, P, S, or Q" << endl;
        }

        cout << "\nPress Enter to continue...";
        getline(cin, buffer);
        // clearScreenRUN();
    }
}

/*
void CashRegister::adminMode()

{
    int choice;

    while (true)
    {
        displayAdminMenu();

        // Input validation for menu choice
        while (!(cin >> choice) || choice < 1 || choice > 6)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 6: ";
        }

        switch (choice)
        {
        case 1:
        {
            string id, name;
            double price;
            int quantity;

            cout << "Enter item ID: ";
            cin >> id;

            cout << "Enter item name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter price: $";
            while (!(cin >> price) || price < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid price! Please enter a positive number: $";
            }

            cout << "Enter quantity: ";
            while (!(cin >> quantity) || quantity < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid quantity! Please enter a positive number: ";
            }

            addNewItem(Item(id, name, price, quantity));
            break;
        }
        case 2:
        {
            string id, name;
            double price;
            int quantity;

            cout << "Enter item ID to edit: ";
            cin >> id;

            cout << "Enter new name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter new price: $";
            while (!(cin >> price) || price < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid price! Please enter a positive number: $";
            }

            cout << "Enter new quantity: ";
            while (!(cin >> quantity) || quantity < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid quantity! Please enter a positive number: ";
            }

            editItem(id, name, price, quantity);
            break;
        }
        case 3:
        {
            string id;
            cout << "Enter item ID to delete: ";
            cin >> id;
            deleteItem(id);
            break;
        }
        case 4:
        {
            string id;
            cout << "Enter item ID to search: ";
            cin >> id;
            Item *item = searchItem(id);
            if (item != nullptr)
            {
                cout << "Item found:" << endl;
                item->displayItem();
            }
            else
            {
                cout << "Item not found!" << endl;
            }
            break;
        }
        case 5:
            displayInventory();
            break;
        case 6:
            return;
        }
    }
}
*/

void CashRegister::adminMode()
{
    int choice;
    string buffer;

    while (true)
    {
        displayAdminMenu();

        // Input validation for menu choice
        while (!(cin >> choice) || choice < 1 || choice > 6)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 6: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        clearScreenRUN();
        displayCashRegisterShelf();

        switch (choice)
        {
        case 1:
        {
            string id, name;
            double price;
            int quantity;

            cout << "Enter item ID (or 'B' to go back): ";
            getline(cin, id);
            if (id == "B" || id == "b")
                break;

            cout << "Enter item name (or 'B' to go back): ";
            getline(cin, name);
            if (name == "B" || name == "b")
                break;

            cout << "Enter price (or -1 to go back): $";
            while (!(cin >> price) || (price != -1 && price < 0))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid price! Please enter a positive number (or -1 to go back): $";
            }
            if (price == -1)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            cout << "Enter quantity (or -1 to go back): ";
            while (!(cin >> quantity) || (quantity != -1 && quantity < 0))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid quantity! Please enter a positive number (or -1 to go back): ";
            }
            if (quantity == -1)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            addNewItem(Item(id, name, price, quantity));
            break;
        }
        case 2:
        {
            displayInventory();
            if (inventory.empty())
            {
                cout << "No items available in inventory!" << endl;
                break;
            }

            string id, name;
            double price;
            int quantity;

            cout << "Enter item ID to edit (or 'B' to go back): ";
            getline(cin, id);
            if (id == "B" || id == "b")
                break;

            // Check if item exists before proceeding
            Item *existingItem = searchItem(id);
            if (existingItem == nullptr)
            {
                cout << "Error: Item with ID '" << id << "' not found!" << endl;
                break;
            }

            cout << "Current item details:" << endl;
            existingItem->displayItem();
            cout << endl;

            cout << "Enter new name (or 'B' to go back): ";
            getline(cin, name);
            if (name == "B" || name == "b")
                break;

            cout << "Enter new price (or -1 to go back): $";
            while (!(cin >> price) || (price != -1 && price < 0))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid price! Please enter a positive number (or -1 to go back): $";
            }
            if (price == -1)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            cout << "Enter new quantity (or -1 to go back): ";
            while (!(cin >> quantity) || (quantity != -1 && quantity < 0))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid quantity! Please enter a positive number (or -1 to go back): ";
            }
            if (quantity == -1)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            editItem(id, name, price, quantity);
            break;
        }
        case 3:
        {
            displayInventory();
            if (inventory.empty())
            {
                cout << "No items available in inventory!" << endl;
                break;
            }

            string id;
            cout << "Enter item ID to delete (or 'B' to go back): ";
            getline(cin, id);
            if (id == "B" || id == "b")
                break;

            // Confirmation before deletion
            Item *itemToDelete = searchItem(id);
            if (itemToDelete != nullptr)
            {
                cout << "Are you sure you want to delete this item?" << endl;
                itemToDelete->displayItem();
                cout << "Type 'YES' to confirm or anything else to cancel: ";
                string confirmation;
                getline(cin, confirmation);
                if (confirmation == "YES")
                {
                    deleteItem(id);
                }
                else
                {
                    cout << "Deletion cancelled." << endl;
                }
            }
            else
            {
                cout << "Error: Item with ID '" << id << "' not found!" << endl;
            }
            break;
        }
        case 4:
        {
            string id;
            cout << "Enter item ID to search (or 'B' to go back): ";
            getline(cin, id);
            if (id == "B" || id == "b")
                break;

            Item *item = searchItem(id);
            if (item != nullptr)
            {
                cout << "\nItem found:" << endl;
                cout << string(50, '-') << endl;
                item->displayItem();
                cout << string(50, '-') << endl;
            }
            else
            {
                cout << "Item with ID '" << id << "' not found!" << endl;
            }
            break;
        }
        case 5:
            displayInventory();
            break;
        case 6:
            return;
        }

        cout << "\nPress Enter to continue...";
        getline(cin, buffer);
    }
}

// END