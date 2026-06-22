# DSA
# Wealth Track(C++ / DSA Project)

## 1. Project Title

**Wealth Track** — a console-based C++ application that manages books, members, and checkouts using core data structures: Linked List, Stack, Queue, and Binary Search Tree (BST).

---

## 2. Problem Statement

A library needs a system to manage its book catalogue, track which books are issued to which members, handle waiting lists when a book is unavailable, and allow fast lookups of book information. The system must also persist data between runs, so that books, issue records, and statuses are not lost every time the program closes.

This project solves that problem by representing each part of the library's workflow with the data structure best suited to it:

- The full book catalogue (Linked List)
- The most recent issue history (Stack)
- Members waiting for an unavailable book (Queue)
- Fast lookup and sorted display of books by ID (Binary Search Tree)

---

## 3. Objectives

- Implement and demonstrate four core data structures (Linked List, Stack, Queue, BST) in a single working C++ program.
- Provide a working menu-driven interface to add, remove, search, issue, and return books.
- Persist library data between runs using CSV file storage.
- Prevent data integrity issues such as duplicate book IDs.
- Analyze and document the time/space complexity of each operation.

---

## 4. System Overview / Architecture

The system is built around one shared `Book` record and four independent structures that each serve a different operational need:

```
                ┌─────────────────────┐
                │   library.csv       │   <-- persistent storage
                └──────────┬──────────┘
                           │ load / save
                           ▼
        ┌──────────────────────────────────────┐
        │              main()                  │
        │     (menu-driven control loop)       │
        └───────┬───────┬───────┬───────┬───────┘
                │       │       │       │
                ▼       ▼       ▼       ▼
        ┌──────────┐ ┌───────┐ ┌───────┐ ┌──────────┐
        │  Linked   │ │ Stack │ │ Queue │ │   BST    │
        │   List    │ │(issue │ │(wait- │ │ (search/ │
        │ (catalog) │ │history)│ │ list) │ │  sorted) │
        └──────────┘ └───────┘ └───────┘ └──────────┘
```

- The **Linked List** is the single source of truth for all book records.
- The **BST** mirrors the same books (indexed by ID) purely for fast search and sorted display.
- The **Stack** and **Queue** do not store full book records — only lightweight references (book ID + member name) tied to issue/return events.

---

## 5. Data Structures and Algorithms Used

| Structure | Used For | Why This Structure |
|---|---|---|
| Singly Linked List | Storing the full book catalogue | Dynamic size, no upfront capacity needed, simple sequential storage |
| Stack (LIFO) | Tracking issue history | The most recent issue is the most relevant record to undo/reference first |
| Queue (FIFO) | Waitlist for unavailable books | Fair ordering — first member to request a book gets it first when it's returned |
| Binary Search Tree | Searching books by ID, sorted display | O(log n) average search vs O(n) linear scan; in-order traversal gives sorted output for free |

**Algorithms used internally:**
- BST insertion and search (recursive, comparison-based)
- In-order tree traversal (for sorted book listing)
- Linear traversal (Linked List, Stack, Queue display operations)
- CSV parsing using string `find`/`substr` operations

---

## 6. Implementation Approach

The project is implemented as a single C++ file (`DSA.cpp`) using a beginner-friendly, function-based style (no classes) to keep the logic transparent and easy to follow.

**Design decisions:**
- All four data structures are implemented manually using `struct` and raw pointers (`new`/`delete`) — no STL containers — to demonstrate the underlying mechanics required by the assignment.
- The Linked List is the authoritative store; the BST is a secondary index built alongside it. Both are updated together whenever a book is added.
- `findBook(id)` returns a pointer (not a copy) to the real book record, so that issuing/returning a book updates the actual data in place.
- A duplicate-ID check (`findBook(id) != nullptr`) runs before any new book is added, preventing two books from silently sharing the same ID.
- On startup, the system loads `library.csv` if it exists; if not, it seeds the library with five default books, so the catalogue is never empty on a fresh run.
- On exit (or via the "Save" menu option), the entire catalogue is written back to `library.csv`, including each book's current issued/available status.

---

## 7. Time and Space Complexity Analysis

| Operation | Structure | Time Complexity | Space Complexity |
|---|---|---|---|
| Add Book | Linked List + BST | O(n) (list traversal to tail) + O(log n) avg (BST insert) | O(1) per insert |
| Remove Book | Linked List | O(n) | O(1) |
| Find Book (by traversal) | Linked List | O(n) | O(1) |
| Search Book (by ID) | BST | O(log n) average, O(n) worst case (unbalanced) | O(1) extra (recursive call stack: O(log n) avg) |
| Sorted Display | BST (in-order traversal) | O(n) | O(n) recursive call stack in worst case |
| Issue Book | Stack push + Linked List lookup | O(1) push, O(n) lookup | O(1) |
| Return Book | Stack pop + Queue dequeue | O(1) | O(1) |
| Add to Waitlist | Queue enqueue | O(1) | O(1) |
| Save to File | Linked List traversal | O(n) | O(1) (streaming write) |
| Load from File | File read + Linked List/BST insert per row | O(n) total, O(n log n) average for all BST inserts | O(n) for n books in memory |

**Note on BST worst case:** Since books are inserted in the order they are added (not randomized), inserting IDs in strictly increasing or decreasing order would degrade the BST into a linked-list-like shape, making search O(n) in that worst case. This project does not implement self-balancing (e.g., AVL), so this trade-off is a known limitation, discussed further in Section 11.

---

## 8. Execution Steps

### Prerequisites
- A C++ compiler supporting C++11 or later (e.g., `g++`)

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/foggypants/DSA.git
   cd DSA
   ```

2. Compile the program:
   ```bash
   g++ -o DSA DSA.cpp
   ```

3. Run the program:
   ```bash
   ./DSA
   ```
   (On Windows: `DSA.exe`)

4. On first run (no `library.csv` present), the system automatically loads 5 default books. On every run after that, it loads whatever was last saved in `library.csv`, located in the same folder as the executable.

5. Use the on-screen menu (1–10) to add, remove, search, issue, return, or view books. Choose `0` to exit (this also auto-saves).

---

## 9. Sample Inputs and Outputs

### Sample Run: Adding and Searching for a Book

**Input:**
```
Choice: 1
ID: 6
Title: Dune
Author: Frank Herbert

Choice: 5
ID: 6
```

**Output:**
```
Book added: Dune

1.  Add Book
2.  Remove Book
...
Choice: ID: Found: Dune | Frank Herbert | Available
```

### Sample Run: Issuing a Book That's Already Checked Out

**Input:**
```
Choice: 3
ID: 2
Member: Ashish

Choice: 3
ID: 2
Member: Priya
```

**Output:**
```
"The Hobbit" issued to Ashish

Already issued. Adding to waitlist.
Priya added to waitlist for Book 2
```

### Sample Run: Returning a Book With Someone Waiting

**Input:**
```
Choice: 4
ID: 2
```

**Output:**
```
"The Hobbit" returned.
Checking waitlist...
Priya is next for Book 2
```

### Sample Run: Rejecting a Duplicate ID

**Input:**
```
Choice: 1
ID: 2
```

**Output:**
```
A book with ID 2 already exists. Choose a different ID.
```

---

## 10. Screenshots
<img width="274" height="180" alt="Screenshot 2026-06-21 at 11 02 59 PM" src="https://github.com/user-attachments/assets/678314e4-b04e-4c14-9f13-10f1ee34d9a0" />
<img width="230" height="248" alt="Screenshot 2026-06-21 at 11 03 51 PM" src="https://github.com/user-attachments/assets/efbcd1e2-1171-48fa-a79c-6665b08d43e2" />
<img width="257" height="231" alt="Screenshot 2026-06-21 at 11 04 22 PM" src="https://github.com/user-attachments/assets/8e05bf3c-b2ec-4f52-8436-bc67492e2b19" />
<img width="246" height="233" alt="Screenshot 2026-06-21 at 11 04 50 PM" src="https://github.com/user-attachments/assets/d6263862-65be-4385-9dda-49d6ad79eef3" />
<img width="307" height="246" alt="Screenshot 2026-06-21 at 11 05 21 PM" src="https://github.com/user-attachments/assets/e4f2fdee-9259-4b81-ba6f-a0936504607c" />
<img width="229" height="230" alt="Screenshot 2026-06-21 at 11 05 48 PM" src="https://github.com/user-attachments/assets/7d1da8ea-4108-4260-acf4-d270f6131f0f" />


## 11. Results and Observations

- All four data structures (Linked List, Stack, Queue, BST) were implemented and integrated successfully into a single working console application.
- The BST consistently outperformed a plain linear scan for search operations once the catalogue grew beyond a handful of books, confirming the expected O(log n) average-case advantage.
- Persisting data via CSV proved effective for simulating a real-world library that retains its catalogue between sessions, rather than resetting on every run.
- One identified limitation: because the BST is not self-balancing, inserting books with strictly sequential IDs (e.g., always adding the next highest ID) causes the tree to degrade toward a linked-list shape, losing the O(log n) advantage in that specific case. An AVL or Red-Black tree would resolve this in a production system.
- Adding a duplicate-ID check during development surfaced a real bug: without it, the Linked List would silently store two nodes with the same ID, while the BST would silently ignore the second insert — leaving an unreachable, wasted node in memory. This was fixed by checking `findBook(id) != nullptr` before inserting.

---

## 12. Conclusion

This project demonstrates a practical, working application of four foundational data structures — Linked List, Stack, Queue, and Binary Search Tree — within a single cohesive system modeled on a real-world library workflow. Beyond simply implementing each structure in isolation, the project required reasoning about how they interact: how a Linked List and BST stay in sync, how a Stack and Queue coordinate during issue/return events, and how all of this state can be reliably persisted to and restored from disk. The resulting system, while built for a beginner-level academic setting, reflects design considerations — such as data integrity checks and startup/shutdown persistence — that are directly relevant to real-world software systems.
