# DSA
# WealthTrack: Digital Banking Asset Valuation Suite (C++ / DSA Project)

## 1. Project Title

**WealthTrack** — a console-based C++ portfolio management system for a bank, built to track client investments, process trades, manage risk, and model relationships between financial institutions. Implemented using six core data structures and algorithms: Linked List, Stack, Queue, Binary Search Tree, Merge Sort, Graph (with Dijkstra's Algorithm), and a Greedy Allocation Algorithm.

GitHub Repository: https://github.com/foggypants/DSA

---

## 2. Problem Statement

WealthTrack is a portfolio management system for a major bank tracking investment records for thousands of clients across stocks, bonds, and mutual funds. The system must calculate portfolio values, process trades, and analyze risk.

The bank currently faces several operational problems:
- The client investment database is too massive for efficient in-memory storage.
- Complex financial events, like stock splits, cannot be rolled back if entered incorrectly.
- Trade requests are processed out of order.
- The system cannot rank clients by portfolio value for risk monitoring.
- There is no model of how financial institutions connect, making it impossible to find the lowest-risk settlement path for large transactions.
- There is no automated way to allocate new funds to meet diversification targets.

This project addresses each of these problems with the data structure or algorithm best suited to it.

---

## 3. Objectives

- Implement a disk-friendly, dynamically growing investment ledger using a Linked List.
- Provide reliable rollback of financial events (such as stock splits) using a Stack.
- Guarantee strict first-in-first-out trade processing using a Queue.
- Enable instant lookup of a stock's trading information by ticker symbol using a Binary Search Tree.
- Rank client portfolios by value using a Merge Sort, in a way that performs consistently regardless of input order.
- Model the financial institution network as a Graph, and compute the lowest-risk settlement path between institutions using Dijkstra's Algorithm.
- Allocate new client funds toward diversification targets immediately using a Greedy Algorithm.
- Persist the investment ledger between sessions using CSV file storage.

---

## 4. System Overview / Architecture

```
                  ledger.csv  (persistent storage)
                       |
                 load / save
                       |
                    main()
             (menu-driven control loop)
        ___________|_______|_______|_______|___________
       |           |       |       |       |           |
  Linked List    Stack   Queue    BST     Graph
  (ledger)     (split  (trade  (ticker  (financial
                undo)   line)  lookup)   network)
                                            |
                                   Dijkstra's Algorithm
                                   (safest trade path)

  Merge Sort -> Wealth Sorter (ranks ledger by portfolio value)
  Greedy Algorithm -> Portfolio Balancer (fund allocation)
```

The Investment Ledger (Linked List) is the single source of truth for every client's holdings. The Ticker Index (BST) stores **pointers** into the same ledger records — not copies — so that any update to a record (such as a stock split) is immediately reflected everywhere the record is referenced. The Stack, Queue, Graph, Merge Sort, and Greedy Allocator each operate independently on top of this shared ledger.

---

## 5. Data Structures and Algorithms Used

| Case Study Feature | Structure / Algorithm | Why This Choice |
|---|---|---|
| Investment Ledger | Linked List | Grows dynamically without a pre-declared size limit, addressing the case study's note that the database is "too massive for efficient in-memory storage" with fixed structures |
| Split Undo | Stack (LIFO) | The most recent financial event is always the one most likely to need correcting first |
| Trading Line | Queue (FIFO) | Guarantees trades are executed in the exact order they were received, as the case study requires |
| Ticker Lookup | Binary Search Tree | O(log n) average lookup by ticker symbol, far faster than scanning the full ledger linearly |
| Wealth Sorter | Merge Sort | O(n log n) in all cases (best, average, worst) — chosen over Quick Sort because client records are not randomly ordered, which could trigger Quick Sort's O(n^2) worst case |
| Financial Network | Graph (adjacency list) | Models real-world institution relationships, which are sparse (each institution connects to a handful of others, not all of them) |
| Safest Trade | Dijkstra's Algorithm | A greedy shortest-path algorithm; correct here because settlement risk weights are non-negative |
| Portfolio Balancer | Greedy Algorithm | Makes an immediate, irreversible allocation decision at each step, matching the case study's requirement for "immediate allocation decisions" |

---

## 6. Implementation Approach

The project is implemented as a single C++ file (`WealthTrack.cpp`) using a class-based design, reflecting the larger scope of this system compared to a single-structure demo.

**Design decisions:**
- `InvestmentLedger` is the authoritative store for all client records, implemented as a linked list of `Investment` structs.
- `TickerIndex` (the BST) stores `Investment*` pointers rather than copies of the records. This was a deliberate fix during development: an earlier version stored copies, which meant a stock split updated the ledger but left the BST's cached copy stale. Storing pointers ensures both structures always reflect the same underlying data.
- `EventHistory` (the Stack) stores only the minimal information needed to reverse a split — the ticker, the pre-split quantity, and the split ratio — rather than a full snapshot of the investment.
- `TradeQueue` processes trades strictly in arrival order; nothing can skip the line.
- `FinancialNetwork` uses an adjacency list (an array of linked edge lists) rather than an adjacency matrix, since a real institution network is sparse.
- `wealthSorter()` copies the ledger into a plain array before sorting, since Merge Sort needs random-access indexing that a linked list cannot provide efficiently; the linked list itself is left untouched and unsorted.
- `portfolioBalancer()` repeatedly allocates funds to whichever asset category is currently furthest below its target percentage, committing each allocation permanently (no backtracking), which is the defining property of a greedy algorithm.
- On startup, the system loads `ledger.csv` if present; otherwise, it seeds five default client investments so the system is never empty on first run.
- On exit, or via the Save menu option, the full ledger is written back to `ledger.csv`.

---

## 7. Time and Space Complexity Analysis

| Operation | Structure / Algorithm | Time Complexity | Space Complexity |
|---|---|---|---|
| Add Investment Record | Linked List | O(n) (traversal to tail) | O(1) per record |
| Find by Ticker (internal) | Linked List traversal | O(n) | O(1) |
| Ticker Lookup (menu) | Binary Search Tree | O(log n) average, O(n) worst case | O(log n) average recursion depth |
| Record Stock Split | Stack push | O(1) | O(1) |
| Undo Stock Split | Stack pop | O(1) | O(1) |
| Submit Trade | Queue enqueue | O(1) | O(1) |
| Process Trade | Queue dequeue | O(1) | O(1) |
| Wealth Sorter | Merge Sort | O(n log n) in all cases | O(n) (temporary arrays during merge) |
| Connect Institutions | Graph edge insert | O(1) | O(1) per edge |
| Safest Trade | Dijkstra's Algorithm (array-based) | O(V^2) for V institutions | O(V) |
| Portfolio Balancer | Greedy allocation loop | O(k x c) for k allocation steps and c categories | O(c) |
| Save to File | Linked List traversal | O(n) | O(1) (streaming write) |
| Load from File | File read + Linked List + BST insert per row | O(n) for list, O(n log n) average for all BST inserts | O(n) |

**Note on BST worst case:** Since tickers are inserted in the order client records are added (not randomized), inserting alphabetically sorted tickers in sequence would degrade the BST into a linked-list shape, making lookup O(n) in that case. This project does not implement self-balancing (e.g., AVL), which is a known and accepted limitation for a system at this scope.

**Note on Dijkstra's implementation:** This project uses the simple O(V^2) array-based version of Dijkstra's Algorithm (scanning all institutions each round to find the minimum), rather than the O((V+E) log V) priority-queue version, since the financial network in this system is small (a handful of institutions). A priority-queue-based implementation would be preferable at a larger scale.

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
   g++ -o WealthTrack WealthTrack.cpp
   ```

3. Run the program:
   ```bash
   ./WealthTrack
   ```
   (On Windows: `WealthTrack.exe`)

4. On first run (no `ledger.csv` present), the system automatically loads 5 default client investments and a sample 4-institution financial network. On every run after that, it loads whatever was last saved in `ledger.csv`.

5. Use the on-screen menu (1-14) to manage investments, trades, splits, and analysis. Choose `0` to exit (this also auto-saves the ledger).

---

## 9. Sample Inputs and Outputs

### Recording a Stock Split and Verifying It Through Ticker Lookup

**Input:**
```
Choice: 5
Ticker: AAPL
Split Ratio (e.g. 2 for 2-for-1 split): 2

Choice: 2
Enter Ticker Symbol: AAPL
```

**Output:**
```
Split applied. New quantity: 100

Found - Ticker: AAPL | Client: Ravi Shah | Qty: 100 | Price: 180.25 | Value: 18025
```

### Undoing the Split

**Input:**
```
Choice: 6
```

**Output:**
```
Reverted AAPL to quantity 50
```

### Wealth Sorter (Merge Sort)

**Input:**
```
Choice: 10
```

**Output:**
```
--- Wealth Sorter: Clients Ranked by Portfolio Value ---
1. Arjun Mehta (GOOG) | Value: 41250
2. Karan Verma (AMZN) | Value: 33805
3. Priya Nair (MSFT) | Value: 16430
4. Ravi Shah (AAPL) | Value: 9012.5
5. Meera Iyer (TSLA) | Value: 7353
```

### Safest Trade Path (Dijkstra's Algorithm)

**Input:**
```
Choice: 12
From institution ID: 0
To institution ID: 3
```

**Output:**
```
--- Safest Trade Path ---
Total settlement risk: 4
Path: BankA -> BankC -> BankB -> BankD
```

### Portfolio Balancer (Greedy Allocation)

**Input:**
```
Choice: 13
New funds to invest: 2000
```

**Output:**
```
Final allocation:
Stocks | Target: 50% | Actual: 50% | Value: 5000
Bonds | Target: 30% | Actual: 30% | Value: 3000
Mutual Funds | Target: 20% | Actual: 20% | Value: 2000
```

---

## 10. Screenshots
<img width="754" height="482" alt="WhatsApp Image 2026-06-23 at 7 39 21 AM" src="https://github.com/user-attachments/assets/2c5ae228-12f4-452b-a158-d22b3870c2d0" />

---

## 11. Results and Observations

- All required features from the case study — Investment Ledger, Split Undo, Trading Line, Ticker Lookup, Wealth Sorter, Financial Network, Safest Trade, and Portfolio Balancer — were implemented and verified working.
- A real data-integrity bug was found and fixed during development: the BST initially stored copies of investment records rather than pointers. After a stock split updated the ledger, the BST's lookup results remained stale (showing the pre-split quantity). This was fixed by storing `Investment*` pointers in the BST instead of `Investment` values, ensuring both structures always reflect the same underlying data.
- Dijkstra's Algorithm correctly identified the lowest-risk settlement path in the sample financial network (BankA to BankC to BankB to BankD, total risk 4) over more direct-looking but costlier alternatives (BankA to BankB to BankD, total risk 5), confirming the greedy approach finds the true minimum, not just a locally appealing option.
- The Greedy Portfolio Balancer correctly distributed new funds to bring all three asset categories to their exact target percentages, demonstrating that a greedy approach is well-suited to this particular allocation problem.
- A known limitation: the BST is not self-balancing, so a worst-case insertion order (e.g., tickers added in already-sorted order) would degrade lookup performance toward O(n). An AVL or Red-Black tree would resolve this in a production system.
- A second known limitation: the Dijkstra's implementation here uses the simpler O(V^2) array-scan approach rather than a priority-queue-based O((V+E) log V) approach, which is an acceptable trade-off at the small scale of institutions modeled here, but would need revisiting for a larger real-world network.

---

## 12. Conclusion

WealthTrack demonstrates how a single banking system can be decomposed into independent, well-suited data structures and algorithms — a Linked List for bulk storage, a Stack for rollback, a Queue for fairness, a Binary Search Tree for fast lookup, Merge Sort for reliable ranking, a Graph with Dijkstra's Algorithm for risk-aware pathfinding, and a Greedy Algorithm for immediate allocation decisions. Beyond implementing each piece in isolation, the project required ensuring these structures stayed consistent with one another — most notably, fixing a real synchronization bug between the ledger and its BST index. The result reflects design considerations directly relevant to real-world wealth management platforms such as Zerodha or Charles Schwab, where accuracy, auditability, and risk-awareness are not optional features but core requirements.
