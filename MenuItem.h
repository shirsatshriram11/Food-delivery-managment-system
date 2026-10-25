#ifndef MENUITEM_H
#define MENUITEM_H

#include <iostream>
#include <string>
using namespace std;

// ─────────────────────────────────────────────
//  MenuItem  – represents one food item on the menu
// ─────────────────────────────────────────────
struct MenuItem {
    int    id;
    string name;
    string category;   // e.g. "Pizza", "Burger", "Drink"
    double price;
    bool   available;

    MenuItem() : id(0), price(0.0), available(true) {}

    MenuItem(int id, const string& name, const string& category,
             double price, bool available = true)
        : id(id), name(name), category(category),
          price(price), available(available) {}

    void display() const {
        cout << "  [" << id << "] "
             << left  << setw(25) << name
             << "  Category: " << left << setw(10) << category
             << "  Price: Rs." << fixed << setprecision(2) << price
             << (available ? "" : "  [UNAVAILABLE]") << "\n";
    }
};

#endif // MENUITEM_H
