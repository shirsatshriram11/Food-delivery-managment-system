#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

// ─────────────────────────────────────────────
//  DATA STRUCTURE: PRIORITY QUEUE (MIN-HEAP)
//  Used for: Express / VIP order dispatch.
//  Priority 1 = Express (processed first)
//  Priority 2 = Normal
// ─────────────────────────────────────────────

#include <vector>
#include "Order.h"
using namespace std;

// Custom comparator: lower priority value = higher urgency
struct OrderComparator {
    bool operator()(const Order& a, const Order& b) {
        // Min-heap by priority; tie-break by orderId (earlier = higher)
        if (a.priority != b.priority) return a.priority > b.priority;
        return a.orderId > b.orderId;
    }
};

class PriorityOrderQueue {
private:
    // Manual min-heap using vector
    vector<Order> heap;

    int parent(int i) { return (i - 1) / 2; }
    int left  (int i) { return 2 * i + 1;   }
    int right (int i) { return 2 * i + 2;   }

    // Should a come before b? (a has more urgency)
    bool hasHigherPriority(const Order& a, const Order& b) {
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.orderId < b.orderId;
    }

    void heapifyUp(int i) {
        while (i > 0 && hasHigherPriority(heap[i], heap[parent(i)])) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i) {
        int smallest = i;
        int l = left(i), r = right(i);
        int n = (int)heap.size();
        if (l < n && hasHigherPriority(heap[l], heap[smallest])) smallest = l;
        if (r < n && hasHigherPriority(heap[r], heap[smallest])) smallest = r;
        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    // Insert order
    void insert(const Order& order) {
        heap.push_back(order);
        heapifyUp((int)heap.size() - 1);
        cout << "\n  ⚡ Order #" << order.orderId
             << " added to priority queue ("
             << (order.priority == 1 ? "EXPRESS" : "Normal") << ").\n";
    }

    // Remove & return highest priority order
    Order extractMin() {
        if (heap.empty()) {
            cout << "\n  ⚠️  Priority queue is empty.\n";
            return Order();
        }
        Order top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
        cout << "\n  🚀 Dispatching Priority Order #" << top.orderId
             << " [" << (top.priority == 1 ? "EXPRESS" : "Normal") << "]\n";
        return top;
    }

    // Peek top without removing
    Order peek() const {
        if (heap.empty()) return Order();
        return heap[0];
    }

    bool isEmpty() const { return heap.empty(); }
    int  size()    const { return (int)heap.size(); }

    // Display all (not sorted — raw heap array shown with rank info)
    void displayAll() const {
        if (heap.empty()) {
            cout << "\n  ℹ️  Priority queue is empty.\n";
            return;
        }
        // Show a sorted copy
        vector<Order> temp = heap;
        // Simple selection sort for display
        for (int i = 0; i < (int)temp.size()-1; i++)
            for (int j = i+1; j < (int)temp.size(); j++)
                if (temp[j].priority < temp[i].priority ||
                   (temp[j].priority == temp[i].priority && temp[j].orderId < temp[i].orderId))
                    swap(temp[i], temp[j]);

        cout << "\n  ⚡ Priority Queue (" << heap.size() << " orders, by urgency):\n";
        for (int i = 0; i < (int)temp.size(); i++) {
            cout << "  [" << i+1 << "] Order #" << temp[i].orderId
                 << " — " << temp[i].customerName
                 << " — " << (temp[i].priority == 1 ? "⚡ EXPRESS" : "Normal")
                 << " — Rs." << temp[i].totalAmount << "\n";
        }
    }
};

#endif // PRIORITYQUEUE_H
