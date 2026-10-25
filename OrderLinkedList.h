#ifndef ORDERLINKEDLIST_H
#define ORDERLINKEDLIST_H

// ─────────────────────────────────────────────
//  DATA STRUCTURE: SINGLY LINKED LIST
//  Used for: Persistent Order History log.
//  New orders are appended; can traverse all.
// ─────────────────────────────────────────────

#include "Order.h"
using namespace std;

// Node of linked list
struct ListNode {
    Order     data;
    ListNode* next;

    ListNode(const Order& o) : data(o), next(nullptr) {}
};

class OrderLinkedList {
private:
    ListNode* head;
    ListNode* tail;
    int       count;

public:
    OrderLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Destructor – free memory
    ~OrderLinkedList() {
        ListNode* cur = head;
        while (cur) {
            ListNode* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    // Append order to end of history list
    void append(const Order& order) {
        ListNode* node = new ListNode(order);
        if (!tail) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        count++;
        cout << "  📜 Order #" << order.orderId << " added to history.\n";
    }

    // Search by order ID – O(n)
    Order* findById(int id) {
        ListNode* cur = head;
        while (cur) {
            if (cur->data.orderId == id) return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    // Update status of an order (found by ID)
    bool updateStatus(int id, OrderStatus newStatus) {
        Order* o = findById(id);
        if (!o) return false;
        o->status = newStatus;
        return true;
    }

    // Display all orders in history
    void displayAll() const {
        if (!head) {
            cout << "\n  ℹ️  Order history is empty.\n";
            return;
        }
        cout << "\n  📖 Complete Order History (" << count << " orders):\n";
        ListNode* cur = head;
        while (cur) {
            cur->data.display(true);
            cur = cur->next;
        }
    }

    // Display orders filtered by customer name
    void displayByCustomer(const string& name) const {
        cout << "\n  🔍 Orders for customer: " << name << "\n";
        ListNode* cur = head;
        bool found = false;
        while (cur) {
            if (cur->data.customerName == name) {
                cur->data.display(false);
                found = true;
            }
            cur = cur->next;
        }
        if (!found) cout << "  ℹ️  No orders found for this customer.\n";
    }

    // Display delivered orders only
    void displayDelivered() const {
        cout << "\n  ✅ Delivered Orders:\n";
        ListNode* cur = head;
        bool found = false;
        while (cur) {
            if (cur->data.status == OrderStatus::DELIVERED) {
                cur->data.display(true);
                found = true;
            }
            cur = cur->next;
        }
        if (!found) cout << "  ℹ️  No delivered orders yet.\n";
    }

    int size() const { return count; }
};

#endif // ORDERLINKEDLIST_H
