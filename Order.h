#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include "MenuItem.h"
using namespace std;

// ─────────────────────────────────────────────
//  Order Status Enum
// ─────────────────────────────────────────────
enum class OrderStatus {
    PLACED,
    CONFIRMED,
    PREPARING,
    OUT_FOR_DELIVERY,
    DELIVERED,
    CANCELLED
};

string statusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::PLACED:            return "PLACED";
        case OrderStatus::CONFIRMED:         return "CONFIRMED";
        case OrderStatus::PREPARING:         return "PREPARING";
        case OrderStatus::OUT_FOR_DELIVERY:  return "OUT FOR DELIVERY";
        case OrderStatus::DELIVERED:         return "DELIVERED";
        case OrderStatus::CANCELLED:         return "CANCELLED";
        default:                             return "UNKNOWN";
    }
}

// ─────────────────────────────────────────────
//  Order  – represents a customer order
// ─────────────────────────────────────────────
struct Order {
    static int nextId;             // auto-increment order ID

    int              orderId;
    string           customerName;
    string           address;
    string           phone;
    vector<MenuItem> items;        // cart items (uses Vector DS)
    double           totalAmount;
    OrderStatus      status;
    string           timestamp;
    int              priority;     // 1 = VIP/Express, 2 = Normal (for heap)

    Order() : orderId(0), totalAmount(0.0),
              status(OrderStatus::PLACED), priority(2) {
        orderId = ++nextId;
        setTimestamp();
    }

    Order(const string& cName, const string& addr, const string& ph, int prio = 2)
        : customerName(cName), address(addr), phone(ph),
          totalAmount(0.0), status(OrderStatus::PLACED), priority(prio) {
        orderId = ++nextId;
        setTimestamp();
    }

    void setTimestamp() {
        time_t now = time(nullptr);
        char buf[25];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        timestamp = string(buf);
    }

    void addItem(const MenuItem& item) {
        items.push_back(item);
        totalAmount += item.price;
    }

    void display(bool brief = false) const {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║  Order ID  : #" << orderId << "\n";
        cout << "  ║  Customer  : " << customerName << "\n";
        if (!brief) {
            cout << "  ║  Phone     : " << phone << "\n";
            cout << "  ║  Address   : " << address << "\n";
            cout << "  ║  Time      : " << timestamp << "\n";
        }
        cout << "  ║  Priority  : " << (priority == 1 ? "⚡ EXPRESS/VIP" : "Normal") << "\n";
        cout << "  ║  Status    : " << statusToString(status) << "\n";
        if (!brief) {
            cout << "  ║  Items Ordered:\n";
            for (const auto& it : items)
                cout << "  ║    - " << it.name << "  Rs." << it.price << "\n";
        }
        cout << "  ║  Total     : Rs." << fixed << setprecision(2) << totalAmount << "\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
    }

    // Comparator for priority queue (min-heap: lower priority value = higher urgency)
    bool operator>(const Order& other) const {
        return priority > other.priority;
    }
};

int Order::nextId = 1000; // Orders start from #1001

#endif // ORDER_H
