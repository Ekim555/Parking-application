#include <iostream>
#include <limits>
#include "ParkingLot.hpp"
#include "Utilities.hpp"


// Terminal launch command: g++ -std=c++17 -Wall -o parking_app main.cpp
// (Windows): .\ParkingApp
// (Linux/Mac): ./ParkingApp

// Print main menu options for UI
void printMainMenu() {
    std::cout << "\n------ Parking Application ------\n";
    std::cout << "1) View available spaces\n";
    std::cout << "2) Buy hourly permit ($4/hr)\n";
    std::cout << "3) Buy daily permit ($12/day)\n";
    std::cout << "4) Show active permits\n";
    std::cout << "5) Release permit\n";
    std::cout << "6) Show parking status\n";
    std::cout << "0) Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    ParkingLot lot(3, 6);
    bool done = false;

    while (!done) {
        printMainMenu();
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            auto avail = lot.listAvailable();
            if (avail.empty())
                std::cout << "No available spaces.\n";
            else {
                std::cout << "Available (" << avail.size() << "): ";
                for (auto& s : avail) std::cout << s.id << " ";
                std::cout << "\n";
            }
            break;
        }
        case 2: {
            std::string dummy, name;
            std::getline(std::cin, dummy);
            std::cout << "Enter name: ";
            std::getline(std::cin, name);
            std::cout << "Hours (1-24): ";
            int hours; std::cin >> hours;
            if (hours < 1) hours = 1;
            else if (hours > 24) hours = 24;
            double cost = PRICE_HOURLY * hours;
            std::cout << "Total: $" << cost << " Confirm (y/n): ";
            char ok; std::cin >> ok;
            if (ok == 'y' || ok == 'Y') {
                Permit p{generateCode(), name, "", Clock::now(),
                         Clock::now() + std::chrono::hours(hours), cost, true};
                std::string assigned = lot.assignAny(p);
                if (!assigned.empty())
                    std::cout << "Assigned " << assigned << " | Code: " << p.code << "\n";
                else std::cout << "No space available.\n";
            }
            break;
        }
        case 3: {
            std::string dummy, name;
            std::getline(std::cin, dummy);
            std::cout << "Enter name: ";
            std::getline(std::cin, name);
            std::cout << "Days (1-7): ";
            int days; std::cin >> days;
            if (days < 1) days = 1;
            else if (days > 7) days = 7;
            double cost = PRICE_DAILY * days;
            std::cout << "Total: $" << cost << " Confirm (y/n): ";
            char ok; std::cin >> ok;
            if (ok == 'y' || ok == 'Y') {
                Permit p{generateCode(), name, "", Clock::now(),
                         Clock::now() + std::chrono::hours(24 * days), cost, true};
                std::string assigned = lot.assignAny(p);
                if (!assigned.empty())
                    std::cout << "Assigned " << assigned << " | Code: " << p.code << "\n";
                else std::cout << "No space available.\n";
            }
            break;
        }
        case 4: {
            auto act = lot.activePermits();
            if (act.empty())
                std::cout << "No active permits.\n";
            else
                for (auto& p : act)
                    std::cout << "Code: " << p.code
                              << " | Owner: " << (p.ownerName.empty() ? "N/A" : p.ownerName)
                              << " | Space: " << p.spaceId
                              << " | Expires: " << timePointToString(p.expires)
                              << " | Paid: $" << p.amountPaid << "\n";
            break;
        }
        case 5: {
            std::cout << "Enter permit code: ";
            std::string code; std::cin >> code;
            if (lot.releasePermit(code))
                std::cout << "Permit released.\n";
            else
                std::cout << "Invalid code.\n";
            break;
        }
        case 6:
            lot.printStatus();
            break;
        case 0:
            done = true;
            break;
        default:
            std::cout << "Invalid option.\n";
        }
    }

    std::cout << "\nThank you for using the Parking App!\n";
    return 0;
}

