# 🍕 Food Delivery Order Management System
### DSA Mini Project — C++

---

## 📌 Overview
A console-based Food Delivery Order Management System that demonstrates **7 key Data Structures** working together in a real-world simulation.

---

## 🧠 Data Structures Used

| # | Data Structure | File | Role |
|---|---|---|---|
| 1 | **Vector** | `Order.h` | Cart — dynamic list of ordered items |
| 2 | **Queue (FIFO)** | `OrderQueue.h` | Normal order processing pipeline |
| 3 | **Stack (LIFO)** | `OrderStack.h` | Recent orders + Undo last order |
| 4 | **Singly Linked List** | `OrderLinkedList.h` | Persistent order history log |
| 5 | **Binary Search Tree** | `BST.h` | Menu storage — search & sorted display |
| 6 | **Min-Heap (Priority Queue)** | `PriorityQueue.h` | Express / VIP order dispatch |
| 7 | **Hash Map (unordered_map)** | `main.cpp` | O(1) order status lookup by Order ID |

---

## 📁 File Structure
```
DSA mini pro/
├── main.cpp                    ← Entry point, all modules
├── structures/
│   ├── MenuItem.h              ← Food item struct
│   ├── Order.h                 ← Order struct + status enum
│   ├── OrderQueue.h            ← Queue-based FIFO processing
│   ├── OrderStack.h            ← Stack for recent orders / undo
│   ├── OrderLinkedList.h       ← Linked List for order history
│   ├── BST.h                   ← BST for menu management
│   └── PriorityQueue.h         ← Manual Min-Heap implementation
└── README.md
```

---

## 🚀 How to Compile & Run

### Using g++ (MinGW / GCC):
```bash
g++ -std=c++17 -o FoodDelivery main.cpp
./FoodDelivery
```

### Using VS Code:
- Open the folder, press **F5** (with C++ extension installed)

### Using Code::Blocks:
- Create a new project → Add all `.h` files and `main.cpp`

---

## 🎮 Features / Modules

| Option | Feature | DS Used |
|---|---|---|
| 1 | Place New Order (Normal or Express) | Vector, Queue / Priority Queue |
| 2 | Track Order Status by ID | Hash Map (O(1)) |
| 3 | Process Next Normal Order | Queue (Dequeue) |
| 4 | Dispatch Next Express Order | Priority Queue (Extract-Min) |
| 5 | Undo / Cancel Last Order | Stack (Pop) |
| 6 | Update Order Status (Admin) | Hash Map + Linked List |
| 7 | Search Menu by keyword / category | BST traversal |
| 8 | View all Queues & Order History | Queue, Stack, Linked List |
| 9 | System Statistics | All structures |
| 10 | Data Structures Reference Table | — |

---

## ⚙️ Complexity Summary

| Operation | DS | Time Complexity |
|---|---|---|
| Add item to cart | Vector | O(1) amortized |
| Place normal order | Queue | O(1) |
| Place express order | Min-Heap | O(log n) |
| Track order by ID | Hash Map | O(1) average |
| Process next order | Queue | O(1) |
| Dispatch express order | Min-Heap | O(log n) |
| Undo last order | Stack | O(1) |
| Search menu (exact) | BST | O(log n) average |
| Display sorted menu | BST In-Order | O(n) |
| Append to history | Linked List | O(1) |

---

## 👨‍💻 Author
DSA Mini Project — C++ | Food Delivery Order Management System
