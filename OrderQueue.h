#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

// ─────────────────────────────────────────────
//  DATA STRUCTURE: QUEUE  (FIFO)
//  Used for: Normal order processing pipeline.
//  Orders placed first are dispatched first.
// ─────────────────────────────────────────────

#include <queue>
#include "Order.h"
using namespace std;

class OrderQueue {
private:
    queue<Order> q;   // STL queue internally uses deque

public:
    // Enqueue – add a new order to the back
    void enqueue(const Order& order) {
        q.push(order);
        cout << "\n  ✅ Order #" << order.orderId
             << " added to processing queue.\n";
    }

    // Dequeue – process and remove the front order
    Order dequeue() {
        if (q.empty()) {
            cout << "\n  ⚠️  Queue is empty! No orders to process.\n";
            return Order();
        }
        Order front = q.front();
        q.pop();
        cout << "\n  🚀 Processing Order #" << front.orderId
             << " for " << front.customerName << "\n";
        return front;
    }

    // Peek front without removing
    Order peek() const {
        if (q.empty()) {
            cout << "\n  ⚠️  Queue is empty.\n";
            return Order();
        }
        return q.front();
    }

    bool isEmpty() const { return q.empty(); }
    int  size()    const { return (int)q.size(); }

    // Display all orders waiting in queue
    void displayAll() const {
        if (q.empty()) {
            cout << "\n  ℹ️  No orders in queue.\n";
            return;
        }
        cout << "\n  📋 Orders in Processing Queue (" << q.size() << "):\n";
        queue<Order> temp = q;   // copy to iterate
        int pos = 1;
        while (!temp.empty()) {
            cout << "  [" << pos++ << "] Order #" << temp.front().orderId
                 << " — " << temp.front().customerName
                 << " — Rs." << temp.front().totalAmount
                 << " — " << statusToString(temp.front().status) << "\n";
            temp.pop();
        }
    }
};

#endif // ORDERQUEUE_H
