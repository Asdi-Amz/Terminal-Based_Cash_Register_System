/* CASH REGISTER*/
// Asdi V. Amamence BSCPE 1-7 

#include "CashRegister.h"
#include <iostream>
#include <limits>
using namespace std;

void clearScreenIDLE()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayCashRegisterTitle()
{
    cout << endl;
    // Top border
    cout << "╔══════════════════════════════════════════════════════════════════╗\n";

    string asciiArt[] = {
        "   ██████╗ █████╗ ███████╗██╗  ██╗    ██████╗ ███████╗ ██████╗  ",
        "  ██╔════╝██╔══██╗██╔════╝██║  ██║    ██╔══██╗██╔════╝██╔════╝  ",
        "  ██║     ███████║███████╗███████║    ██████╔╝█████╗  ██║  ███╗ ",
        "  ██║     ██╔══██║╚════██║██╔══██║    ██╔══██╗██╔══╝  ██║   ██║ ",
        "  ╚██████╗██║  ██║███████║██║  ██║    ██║  ██║███████╗╚██████╔╝ ",
        "   ╚═════╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝    ╚═╝  ╚═╝╚══════╝ ╚═════╝  "};

    // Enhanced money-themed green gradient from emerald to dark green
    for (size_t i = 0; i < 6; ++i)
    {
        float ratio = static_cast<float>(i) / 5.0f;
        int green = 255 - static_cast<int>(100 * ratio); // Gradient from 255 to 155
        int red = 50 + static_cast<int>(50 * ratio);     // Subtle red variation
        int blue = 50;                                   // Fixed blue component for richness

        cout << "║ "; // Left border
        cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m"
             << asciiArt[i] << "\033[0m";
        cout << " ║" << endl; // Right border
    }

    // Bottom border
    cout << "╚══════════════════════════════════════════════════════════════════╝" << endl;
    cout << endl;
}

void displayThankYouMessage()
{
    cout << endl;
    cout << "╔════════════════════════════════════════════════════════════════════════════════╗\n";

    string asciiArt[] = {
        "  ████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗    ██╗   ██╗ ██████╗ ██╗   ██╗  ",
        "  ╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║ ██╔╝    ╚██╗ ██╔╝██╔═══██╗██║   ██║  ",
        "     ██║   ███████║███████║██╔██╗ ██║█████╔╝      ╚████╔╝ ██║   ██║██║   ██║  ",
        "     ██║   ██╔══██║██╔══██║██║╚██╗██║██╔═██╗       ╚██╔╝  ██║   ██║██║   ██║  ",
        "     ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗       ██║   ╚██████╔╝╚██████╔╝  ",
        "     ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝       ╚═╝    ╚═════╝  ╚═════╝   "};

    // Warm gradient from gold to orange
    for (size_t i = 0; i < 6; ++i)
    {
        float ratio = static_cast<float>(i) / 5.0f;
        int red = 255 - static_cast<int>(30 * ratio);   // Golden to orange red
        int green = 215 - static_cast<int>(50 * ratio); // Golden to orange green
        int blue = 50 + static_cast<int>(20 * ratio);   // Subtle blue accent

        cout << "║ ";
        cout << "\033[38;2;" << red << ";" << green << ";" << blue << "m"
             << asciiArt[i] << "\033[0m";
        cout << " ║" << endl;
    }

    cout << "║                                                                                ║\n";
    cout << "║                      🌟 For using Cash Register System! 🌟                     ║\n";
    cout << "║                                                                                ║\n";
    cout << "║                           💰 Have a profitable day! 💰                         ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════════════╝\n";
    cout << endl;

    // Money falling animation effect
    // cout << "                               💵   💴   💶   💷   💸" << endl;
    // cout << "                          💰 Thank you for your business! 💰" << endl;
    // cout << "                               💵   💴   💶   💷   💸" << endl;
    // cout << endl;
}

int main()
{
    CashRegister register_system;
    clearScreenIDLE();
    displayCashRegisterTitle();
    int choice;
    string buffer;
    string AdminPassword = "asdi1523";
    string InputPassword = " ";

    cout << string(14, ' ') << "=== Welcome to Cash Register System ===" << endl;

    cout << string(19, ' ') << "Press Enter to continue...";
    cin.get();

    while (true)
    {
        register_system.displayMainMenu();

        // Input validation for menu choice
        while (!(cin >> choice) || choice < 1 || choice > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 3: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {

        case 1:
            register_system.customerMode();
            break;
        case 2:
        {
            int attempts = 3;

            while (attempts > 0)
            {
                cout << "Enter password: ";
                cin >> InputPassword;

                if (InputPassword == AdminPassword)
                {
                    register_system.adminMode();
                    break;
                }
                else
                {
                    attempts--;
                    cout << "Wrong password. Attempts left: " << attempts << endl;
                    if (attempts == 0)
                    {
                        cout << "Access denied." << endl;
                    }
                }
            }
            break;
        }

        case 3:
            clearScreenIDLE();
            displayThankYouMessage();
            return 0;
        }

        // Optional: Add a "Press Enter to continue..." after returning from modes
        cout << "\nReturning to main menu...\nPress Enter to continue...";
        getline(cin, buffer);
    }

    return 0;
}