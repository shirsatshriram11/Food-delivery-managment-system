#ifndef ORDERSTACK_H
#define ORDERSTACK_H

// ─────────────────────────────────────────────
//  DATA STRUCTURE: STACK  (LIFO)
//  Used for: Tracking recently placed orders.
//  Allows "undo last order" / cancel recent.
// ─────────────────────────────────────────────

#include <stack>
#include "Order.h"
using namespace std;

class OrderStack {
private:
    stack<Order> st;
    static const int MAX_SIZE = 10; // keep last 10 orders

public:
    // Push order onto stack
    void push(const Order& order) {
        if ((int)st.size() >= MAX_SIZE) {
            // We don't discard — just a soft warning
            cout << "  ℹ️  Recent-order stack is full (last " << MAX_SIZE << " shown).\n";
        }
        st.push(order);
        cout << "  📌 Order #" << order.orderId << " saved to recent orders.\n";
    }

    // Pop (undo / cancel last order)
    Order pop() {
        if (st.empty()) {
            cout << "\n  ⚠️  No recent orders to undo.\n";
            return Order();
        }
        Order top = st.top();
        st.pop();
        cout << "\n  ↩️  Removed Order #" << top.orderId
             << " from recent orders (Undo).\n";
        return top;
    }

    // Peek top without removing
    Order peek() const {
        if (st.empty()) {
            cout << "\n  ⚠️  No recent orders.\n";
            return Order();
        }
        return st.top();
    }

    bool isEmpty() const { return st.empty(); }
    int  size()    const { return (int)st.size(); }

    // Display all recent orders (top = most recent)
    void displayAll() const {
        if (st.empty()) {
            cout << "\n  ℹ️  No recent orders in stack.\n";
            return;
        }
        cout << "\n  🕐 Recent Orders (most recent first):\n";
        stack<Order> temp = st;
        int rank = 1;
        while (!temp.empty()) {
            const Order& o = temp.top();
            cout << "  [" << rank++ << "] Order #" << o.orderId
                 << " — " << o.customerName
                 << " — Rs." << o.totalAmount
                 << " — " << statusToString(o.status) << "\n";
            temp.pop();
        }
    }
};

#endif // ORDERSTACK_H
