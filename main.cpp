/*
 * ╔══════════════════════════════════════════════════════════════════╗
 * ║          FOOD DELIVERY ORDER MANAGEMENT SYSTEM                  ║
 * ║                  DSA Mini Project — C++                         ║
 * ║                                                                 ║
 * ║  Data Structures Used:                                          ║
 * ║   1. Vector          — Cart & Menu item storage                 ║
 * ║   2. Queue           — FIFO order processing pipeline           ║
 * ║   3. Stack           — Recent orders / Undo last order          ║
 * ║   4. Linked List     — Persistent order history log             ║
 * ║   5. BST             — Menu search & sorted display             ║
 * ║   6. Priority Queue  — Express / VIP order dispatch (Min-Heap)  ║
 * ║   7. Hash Map        — O(1) order status tracking               ║
 * ╚══════════════════════════════════════════════════════════════════╝
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>
#include <climits>
#include <algorithm>

#include "structures/MenuItem.h"
#include "structures/Order.h"
#include "structures/OrderQueue.h"
#include "structures/OrderStack.h"
#include "structures/OrderLinkedList.h"
#include "structures/BST.h"
#include "structures/PriorityQueue.h"

using namespace std;

// ────────────────────────────────────────────────
//  GLOBAL DATA STRUCTURES
// ────────────────────────────────────────────────
MenuBST           menuTree;                    // BST: menu storage & search
OrderQueue        processingQueue;             // Queue: FIFO dispatch
OrderStack        recentOrders;                // Stack: recent/undo orders
OrderLinkedList   orderHistory;                // Linked List: full history
PriorityOrderQueue priorityQueue;             // Min-Heap: express orders
unordered_map<int, Order> orderTracker;       // HashMap: O(1) lookup by ID

// ────────────────────────────────────────────────
//  UTILITY HELPERS
// ────────────────────────────────────────────────
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnter() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int getIntInput(const string& prompt, int lo = INT_MIN, int hi = INT_MAX) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= lo && val <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  ❌ Invalid input. Try again.\n";
    }
}

string getStringInput(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

void printBanner() {
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════════════════╗\n";
    cout << "  ║   🍕  FOOD DELIVERY ORDER MANAGEMENT SYSTEM  🛵     ║\n";
    cout << "  ║            DSA Mini Project — C++                   ║\n";
    cout << "  ╚══════════════════════════════════════════════════════╝\n";
}

void printSeparator(const string& title = "") {
    cout << "\n  ══════════════════════════════════════════════════\n";
    if (!title.empty())
        cout << "  " << title << "\n"
             << "  ══════════════════════════════════════════════════\n";
}

// ────────────────────────────────────────────────
//  INITIALISE DEFAULT MENU  (loaded into BST)
// ────────────────────────────────────────────────
void initMenu() {
    // Pizzas
    menuTree.insert(MenuItem(1,  "Margherita Pizza",       "Pizza",   199.0));
    menuTree.insert(MenuItem(2,  "Pepperoni Pizza",        "Pizza",   299.0));
    menuTree.insert(MenuItem(3,  "BBQ Chicken Pizza",      "Pizza",   349.0));
    menuTree.insert(MenuItem(4,  "Veggie Supreme Pizza",   "Pizza",   249.0));

    // Burgers
    menuTree.insert(MenuItem(5,  "Classic Beef Burger",    "Burger",  149.0));
    menuTree.insert(MenuItem(6,  "Crispy Chicken Burger",  "Burger",  169.0));
    menuTree.insert(MenuItem(7,  "Double Cheese Burger",   "Burger",  199.0));
    menuTree.insert(MenuItem(8,  "Veg Aloo Burger",        "Burger",  109.0));

    // Sides
    menuTree.insert(MenuItem(9,  "French Fries (Large)",   "Sides",    79.0));
    menuTree.insert(MenuItem(10, "Onion Rings",            "Sides",    89.0));
    menuTree.insert(MenuItem(11, "Garlic Bread",           "Sides",    69.0));
    menuTree.insert(MenuItem(12, "Coleslaw",               "Sides",    49.0));

    // Drinks
    menuTree.insert(MenuItem(13, "Coca-Cola (500ml)",      "Drink",    49.0));
    menuTree.insert(MenuItem(14, "Mango Shake",            "Drink",    99.0));
    menuTree.insert(MenuItem(15, "Mineral Water",          "Drink",    30.0));
    menuTree.insert(MenuItem(16, "Fresh Lime Soda",        "Drink",    69.0));

    // Desserts
    menuTree.insert(MenuItem(17, "Chocolate Brownie",      "Dessert", 129.0));
    menuTree.insert(MenuItem(18, "Ice Cream Sundae",       "Dessert",  99.0));
    menuTree.insert(MenuItem(19, "Cheesecake Slice",       "Dessert", 149.0));

    cout << "  ✅ Menu loaded successfully (" << 19 << " items in BST).\n";
}

// ────────────────────────────────────────────────
//  REGISTER ORDER (common helper)
// ────────────────────────────────────────────────
void registerOrder(Order& order, bool express) {
    order.status = OrderStatus::PLACED;

    // HashMap — O(1) insert
    orderTracker[order.orderId] = order;

    // Linked List — history log
    orderHistory.append(order);

    // Stack — recent orders
    recentOrders.push(order);

    if (express) {
        // Priority Queue — express dispatch
        priorityQueue.insert(order);
    } else {
        // Queue — normal FIFO
        processingQueue.enqueue(order);
    }

    cout << "\n  🎉 Order placed successfully!\n";
    order.display();
}

// ────────────────────────────────────────────────
//  MODULE 1 — CUSTOMER: PLACE ORDER
// ────────────────────────────────────────────────
void placeOrder() {
    printSeparator("📝 PLACE NEW ORDER");

    string name    = getStringInput("  Enter your name    : ");
    string phone   = getStringInput("  Enter phone number : ");
    string address = getStringInput("  Enter address      : ");

    cout << "\n  Order type:\n"
         << "  [1] Normal Order\n"
         << "  [2] Express / VIP Order (Priority Delivery)\n";
    int type = getIntInput("  Choice: ", 1, 2);
    int prio = (type == 2) ? 1 : 2;

    Order order(name, address, phone, prio);
    vector<MenuItem> cart;

    while (true) {
        menuTree.displayMenu();
        cout << "\n  Enter item ID to add to cart (0 to finish): ";
        int id;
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (id == 0) break;

        MenuItem* item = menuTree.findById(id);
        if (!item) {
            cout << "  ❌ Item not found. Try again.\n";
            continue;
        }
        if (!item->available) {
            cout << "  ❌ Item unavailable.\n";
            continue;
        }
        order.addItem(*item);
        cout << "  ✅ Added: " << item->name << " (Rs." << item->price << ")\n";
        cout << "  🛒 Cart total: Rs." << order.totalAmount << "\n";
    }

    if (order.items.empty()) {
        cout << "\n  ⚠️  Cart is empty. Order not placed.\n";
        return;
    }

    registerOrder(order, prio == 1);
    pressEnter();
}

// ────────────────────────────────────────────────
//  MODULE 2 — TRACK ORDER (HashMap O(1) Lookup)
// ────────────────────────────────────────────────
void trackOrder() {
    printSeparator("🔍 TRACK ORDER STATUS");
    int id = getIntInput("  Enter Order ID: ");

    auto it = orderTracker.find(id);
    if (it == orderTracker.end()) {
        cout << "\n  ❌ Order #" << id << " not found.\n";
    } else {
        cout << "\n  Order found via HashMap (O(1) lookup):\n";
        it->second.display();
    }
    pressEnter();
}

// ────────────────────────────────────────────────
//  MODULE 3 — PROCESS NEXT NORMAL ORDER (Queue)
// ────────────────────────────────────────────────
void processNextOrder() {
    printSeparator("🚀 PROCESS NEXT ORDER (QUEUE)");
    if (processingQueue.isEmpty()) {
        cout << "\n  ℹ️  No orders in processing queue.\n";
        pressEnter();
        return;
    }
    Order processed = processingQueue.dequeue();
    processed.status = OrderStatus::PREPARING;

    // Sync status in HashMap and History
    if (orderTracker.count(processed.orderId))
        orderTracker[processed.orderId].status = OrderStatus::PREPARING;
    orderHistory.updateStatus(processed.orderId, OrderStatus::PREPARING);

    cout << "\n  ✅ Order #" << processed.orderId
         << " is now PREPARING. Remaining in queue: "
         << processingQueue.size() << "\n";
    processed.display();
    pressEnter();
}

// ────────────────────────────────────────────────
//  MODULE 4 — PROCESS NEXT PRIORITY ORDER (Heap)
// ────────────────────────────────────────────────
void processPriorityOrder() {
    printSeparator("⚡ DISPATCH PRIORITY ORDER (MIN-HEAP)");
    if (priorityQueue.isEmpty()) {
        cout << "\n  ℹ️  No priority orders.\n";
        pressEnter();
        return;
    }
    Order dispatched = priorityQueue.extractMin();
    dispatched.status = OrderStatus::OUT_FOR_DELIVERY;

    if (orderTracker.count(dispatched.orderId))
        orderTracker[dispatched.orderId].status = OrderStatus::OUT_FOR_DELIVERY;
    orderHistory.updateStatus(dispatched.orderId, OrderStatus::OUT_FOR_DELIVERY);

    cout << "\n  🛵 Order #" << dispatched.orderId
         << " is OUT FOR DELIVERY.\n";
    dispatched.display();
    pressEnter();
}

// ────────────────────────────────────────────────
//  MODULE 5 — UNDO LAST ORDER (Stack)
// ────────────────────────────────────────────────
void undoLastOrder() {
    printSeparator("↩️  UNDO / CANCEL LAST ORDER (STACK)");
    if (recentOrders.isEmpty()) {
        cout << "\n  ℹ️  No recent orders to undo.\n";
        pressEnter();
        return;
    }
    Order last = recentOrders.pop();
    last.status = OrderStatus::CANCELLED;

    if (orderTracker.count(last.orderId))
        orderTracker[last.orderId].status = OrderStatus::CANCELLED;
    orderHistory.updateStatus(last.orderId, OrderStatus::CANCELLED);

    cout << "\n  ❌ Order #" << last.orderId << " has been CANCELLED.\n";
    last.display();
    pressEnter();
}

// ────────────────────────────────────────────────
//  MODULE 6 — UPDATE ORDER STATUS (Admin)
// ────────────────────────────────────────────────
void updateOrderStatus() {
    printSeparator("✏️  UPDATE ORDER STATUS (Admin)");
    int id = getIntInput("  Enter Order ID: ");

    auto it = orderTracker.find(id);
    if (it == orderTracker.end()) {
        cout << "\n  ❌ Order not found.\n";
        pressEnter();
        return;
    }

    cout << "\n  Current status: " << statusToString(it->second.status) << "\n";
    cout << "  Select new status:\n"
         << "  [1] CONFIRMED\n"
         << "  [2] PREPARING\n"
         << "  [3] OUT FOR DELIVERY\n"
         << "  [4] DELIVERED\n"
         << "  [5] CANCELLED\n";
    int ch = getIntInput("  Choice: ", 1, 5);

    OrderStatus ns;
    switch (ch) {
        case 1: ns = OrderStatus::CONFIRMED;        break;
        case 2: ns = OrderStatus::PREPARING;        break;
        case 3: ns = OrderStatus::OUT_FOR_DELIVERY; break;
        case 4: ns = OrderStatus::DELIVERED;        break;
        default: ns = OrderStatus::CANCELLED;       break;
    }

    it->second.status = ns;
    orderHistory.updateStatus(id, ns);
    cout << "\n  ✅ Order #" << id << " updated to: " << statusToString(ns) << "\n";
    pressEnter();
}

// ────────────────────────────────────────────────
//  MODULE 7 — MENU SEARCH  (BST)
// ────────────────────────────────────────────────
void searchMenu() {
    printSeparator("🔎 SEARCH MENU (BST)");
    cout << "  [1] Search by keyword\n"
         << "  [2] Filter by category\n"
         << "  [3] Show full menu (sorted)\n";
    int ch = getIntInput("  Choice: ", 1, 3);

    if (ch == 1) {
        string kw = getStringInput("  Enter keyword: ");
        auto results = menuTree.searchByKeyword(kw);
        if (results.empty()) {
            cout << "  ❌ No items found for \"" << kw << "\".\n";
        } else {
            cout << "\n  Results for \"" << kw << "\" (" << results.size() << "):\n";
            for (auto& m : results) m.display();
        }
    } else if (ch == 2) {
        cout << "  Categories: Pizza | Burger | Sides | Drink | Dessert\n";
        string cat = getStringInput("  Enter category: ");
        auto results = menuTree.getByCategory(cat);
        if (results.empty()) {
            cout << "  ❌ No items in category \"" << cat << "\".\n";
        } else {
            cout << "\n  Items in \"" << cat << "\" (" << results.size() << "):\n";
            for (auto& m : results) m.display();
        }
    } else {
        menuTree.displayMenu();
    }
    pressEnter();
}

// ────────────────────────────────────────────────
//  MODULE 8 — VIEW QUEUES & HISTORY
// ────────────────────────────────────────────────
void viewQueues() {
    printSeparator("📋 VIEW ALL QUEUES & HISTORY");
    cout << "\n  [1] Processing Queue (FIFO)\n"
         << "  [2] Priority Queue (Min-Heap)\n"
         << "  [3] Recent Orders (Stack)\n"
         << "  [4] Full Order History (Linked List)\n"
         << "  [5] Delivered Orders Only\n"
         << "  [6] Search History by Customer\n";
    int ch = getIntInput("  Choice: ", 1, 6);

    switch (ch) {
        case 1: processingQueue.displayAll();  break;
        case 2: priorityQueue.displayAll();    break;
        case 3: recentOrders.displayAll();     break;
        case 4: orderHistory.displayAll();     break;
        case 5: orderHistory.displayDelivered(); break;
        case 6: {
            string cust = getStringInput("  Enter customer name: ");
            orderHistory.displayByCustomer(cust);
            break;
        }
    }
    pressEnter();
}

// ────────────────────────────────────────────────
//  MODULE 9 — SYSTEM STATS
// ────────────────────────────────────────────────
void systemStats() {
    printSeparator("📊 SYSTEM STATISTICS");
    cout << "\n  Total orders in history     : " << orderHistory.size()      << "\n";
    cout << "  Orders in processing queue  : " << processingQueue.size()    << "\n";
    cout << "  Priority (Express) queue    : " << priorityQueue.size()      << "\n";
    cout << "  Recent orders (stack depth) : " << recentOrders.size()       << "\n";
    cout << "  Total tracked (HashMap)     : " << orderTracker.size()       << "\n";
    pressEnter();
}

// ────────────────────────────────────────────────
//  DS REFERENCE MENU
// ────────────────────────────────────────────────
void dsReference() {
    printSeparator("📚 DATA STRUCTURES USED IN THIS PROJECT");
    cout << R"(
  ┌─────────────────────────────────────────────────────────────────┐
  │  DS               │ Where Used           │ Complexity          │
  ├─────────────────────────────────────────────────────────────────┤
  │  Vector           │ Cart / Menu storage  │ Add O(1) amort.     │
  │  Queue (FIFO)     │ Normal order pipe    │ Enq/Deq O(1)        │
  │  Stack (LIFO)     │ Recent orders / Undo │ Push/Pop O(1)       │
  │  Linked List      │ Order history log    │ Append O(1)         │
  │  BST              │ Menu search & sort   │ Search O(log n) avg │
  │  Priority Queue   │ Express order heap   │ Insert/Del O(log n) │
  │  Hash Map         │ Order ID → Status    │ Lookup O(1) avg     │
  └─────────────────────────────────────────────────────────────────┘
)";
    pressEnter();
}

// ────────────────────────────────────────────────
//  MAIN MENU
// ────────────────────────────────────────────────
void mainMenu() {
    while (true) {
        clearScreen();
        printBanner();
        cout << "\n  ─── MAIN MENU ──────────────────────────────────\n";
        cout << "  [1]  📝  Place New Order\n";
        cout << "  [2]  🔍  Track Order Status\n";
        cout << "  [3]  🚀  Process Next Normal Order (Queue)\n";
        cout << "  [4]  ⚡  Dispatch Next Express Order (Priority Queue)\n";
        cout << "  [5]  ↩️   Undo / Cancel Last Order (Stack)\n";
        cout << "  [6]  ✏️   Update Order Status (Admin)\n";
        cout << "  [7]  🔎  Search Menu (BST)\n";
        cout << "  [8]  📋  View Queues & Order History\n";
        cout << "  [9]  📊  System Statistics\n";
        cout << "  [10] 📚  Data Structures Reference\n";
        cout << "  [0]  ❌  Exit\n";
        cout << "  ────────────────────────────────────────────────\n";

        int choice = getIntInput("  Enter choice: ", 0, 10);

        switch (choice) {
            case 1:  placeOrder();           break;
            case 2:  trackOrder();           break;
            case 3:  processNextOrder();     break;
            case 4:  processPriorityOrder(); break;
            case 5:  undoLastOrder();        break;
            case 6:  updateOrderStatus();    break;
            case 7:  searchMenu();           break;
            case 8:  viewQueues();           break;
            case 9:  systemStats();          break;
            case 10: dsReference();          break;
            case 0:
                cout << "\n  👋 Thank you for using Food Delivery OMS!\n\n";
                return;
        }
    }
}

// ────────────────────────────────────────────────
//  ENTRY POINT
// ────────────────────────────────────────────────
int main() {
    clearScreen();
    printBanner();
    cout << "\n  Initialising menu...\n";
    initMenu();
    pressEnter();
    mainMenu();
    return 0;
}
