#include <iostream>
#include <string>
#include <fstream>
#include <climits>
using namespace std;
struct Investment {
    int clientId;
    string ticker;
    string clientName;
    double quantity;
    double pricePerUnit;

    double totalValue() const {
        return quantity * pricePerUnit;
    }
};

struct LedgerNode {
    Investment record;
    LedgerNode* next;
};

class InvestmentLedger {
private:
    LedgerNode* head;

public:
    InvestmentLedger() {
        head = nullptr;
    }

    void addRecord(int clientId, string ticker, string clientName,
                   double quantity, double pricePerUnit) {
        LedgerNode* newNode = new LedgerNode;
        newNode->record.clientId     = clientId;
        newNode->record.ticker       = ticker;
        newNode->record.clientName   = clientName;
        newNode->record.quantity     = quantity;
        newNode->record.pricePerUnit = pricePerUnit;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        } else {
            LedgerNode* temp = head;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = newNode;
        }
        cout << "Investment recorded: " << clientName << " - " << ticker << endl;
    }

    Investment* findByTicker(string ticker) {
        LedgerNode* temp = head;
        while (temp != nullptr) {
            if (temp->record.ticker == ticker)
                return &temp->record;
            temp = temp->next;
        }
        return nullptr;
    }

    int countRecords() {
        int count = 0;
        LedgerNode* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }
    Investment* toArray(int& outCount) {
        outCount = countRecords();
        Investment* arr = new Investment[outCount];
        LedgerNode* temp = head;
        int i = 0;
        while (temp != nullptr) {
            arr[i] = temp->record;
            i++;
            temp = temp->next;
        }
        return arr;
    }

    void displayAll() {
        if (head == nullptr) {
            cout << "Ledger is empty." << endl;
            return;
        }
        cout << "\n--- Investment Ledger ---" << endl;
        LedgerNode* temp = head;
        while (temp != nullptr) {
            cout << "Client #" << temp->record.clientId
                 << " (" << temp->record.clientName << ")"
                 << " | Ticker: " << temp->record.ticker
                 << " | Qty: " << temp->record.quantity
                 << " | Price: " << temp->record.pricePerUnit
                 << " | Value: " << temp->record.totalValue()
                 << endl;
            temp = temp->next;
        }
    }

    LedgerNode* getHead() { return head; }

    ~InvestmentLedger() {
        LedgerNode* temp = head;
        while (temp != nullptr) {
            LedgerNode* next = temp->next;
            delete temp;
            temp = next;
        }
    }
};

struct SplitEvent {
    string ticker;
    double oldQuantity;
    double splitRatio;
};

struct EventNode {
    SplitEvent event;
    EventNode* next;
};

class EventHistory {
private:
    EventNode* top;

public:
    EventHistory() {
        top = nullptr;
    }

    void recordSplit(string ticker, double oldQuantity, double splitRatio) {
        EventNode* newNode = new EventNode;
        newNode->event.ticker      = ticker;
        newNode->event.oldQuantity = oldQuantity;
        newNode->event.splitRatio  = splitRatio;
        newNode->next = top;
        top = newNode;
    }
    bool undoLastSplit(SplitEvent& outEvent) {
        if (top == nullptr) {
            cout << "No financial events to undo." << endl;
            return false;
        }
        EventNode* del = top;
        outEvent = top->event;
        top = top->next;
        delete del;
        return true;
    }

    void displayHistory() {
        if (top == nullptr) {
            cout << "No financial event history." << endl;
            return;
        }
        cout << "\n--- Financial Event History (most recent first) ---" << endl;
        EventNode* temp = top;
        while (temp != nullptr) {
            cout << "Ticker: " << temp->event.ticker
                 << " | Split Ratio: " << temp->event.splitRatio
                 << " | Quantity Before Split: " << temp->event.oldQuantity
                 << endl;
            temp = temp->next;
        }
    }

    ~EventHistory() {
        while (top != nullptr) {
            EventNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
};

struct TradeRequest {
    int clientId;
    string ticker;
    string action;
    double quantity;
};

struct TradeNode {
    TradeRequest trade;
    TradeNode* next;
};

class TradeQueue {
private:
    TradeNode* front;
    TradeNode* rear;

public:
    TradeQueue() {
        front = nullptr;
        rear  = nullptr;
    }

    void submitTrade(int clientId, string ticker, string action, double quantity) {
        TradeNode* newNode = new TradeNode;
        newNode->trade.clientId = clientId;
        newNode->trade.ticker   = ticker;
        newNode->trade.action   = action;
        newNode->trade.quantity = quantity;
        newNode->next = nullptr;

        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Trade queued: " << action << " " << quantity << " of " << ticker << endl;
    }

    bool processNextTrade(TradeRequest& outTrade) {
        if (front == nullptr) {
            cout << "No pending trades." << endl;
            return false;
        }
        TradeNode* del = front;
        outTrade = front->trade;
        front = front->next;
        if (front == nullptr)
            rear = nullptr;
        delete del;
        return true;
    }

    void displayQueue() {
        if (front == nullptr) {
            cout << "Trading line is empty." << endl;
            return;
        }
        cout << "\n--- Pending Trades (processing order) ---" << endl;
        TradeNode* temp = front;
        while (temp != nullptr) {
            cout << "Client #" << temp->trade.clientId
                 << " | " << temp->trade.action
                 << " " << temp->trade.quantity
                 << " of " << temp->trade.ticker
                 << endl;
            temp = temp->next;
        }
    }

    ~TradeQueue() {
        while (front != nullptr) {
            TradeNode* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

struct TickerNode {
    Investment* record;
    TickerNode* left;
    TickerNode* right;
};

class TickerIndex {
private:
    TickerNode* root;

    TickerNode* insert(TickerNode* node, Investment* record) {
        if (node == nullptr) {
            TickerNode* newNode = new TickerNode;
            newNode->record = record;
            newNode->left   = nullptr;
            newNode->right  = nullptr;
            return newNode;
        }
        if (record->ticker < node->record->ticker)
            node->left  = insert(node->left,  record);
        else if (record->ticker > node->record->ticker)
            node->right = insert(node->right, record);
        return node;
    }

    TickerNode* search(TickerNode* node, string ticker) {
        if (node == nullptr || node->record->ticker == ticker)
            return node;
        if (ticker < node->record->ticker)
            return search(node->left, ticker);
        return search(node->right, ticker);
    }

    void destroy(TickerNode* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    TickerIndex() {
        root = nullptr;
    }

    void insert(Investment* record) {
        root = insert(root, record);
    }

    void lookup(string ticker) {
        TickerNode* result = search(root, ticker);
        if (result == nullptr) {
            cout << "Ticker \"" << ticker << "\" not found." << endl;
        } else {
            cout << "Found - Ticker: " << result->record->ticker
                 << " | Client: " << result->record->clientName
                 << " | Qty: " << result->record->quantity
                 << " | Price: " << result->record->pricePerUnit
                 << " | Value: " << result->record->totalValue()
                 << endl;
        }
    }

    ~TickerIndex() {
        destroy(root);
    }
};

void merge(Investment arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Investment* L = new Investment[n1];
    Investment* R = new Investment[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].totalValue() >= R[j].totalValue())
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(Investment arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void wealthSorter(InvestmentLedger& ledger) {
    int count;
    Investment* arr = ledger.toArray(count);

    if (count == 0) {
        cout << "No investments to rank." << endl;
        delete[] arr;
        return;
    }

    mergeSort(arr, 0, count - 1);

    cout << "\n--- Wealth Sorter: Clients Ranked by Portfolio Value ---" << endl;
    for (int i = 0; i < count; i++) {
        cout << (i + 1) << ". " << arr[i].clientName
             << " (" << arr[i].ticker << ")"
             << " | Value: " << arr[i].totalValue()
             << endl;
    }

    delete[] arr;
}

const int MAX_INSTITUTIONS = 20;

struct Edge {
    int to;
    int riskWeight;
    Edge* next;
};

class FinancialNetwork {
private:
    Edge* adjList[MAX_INSTITUTIONS];
    string institutionNames[MAX_INSTITUTIONS];
    int institutionCount;

public:
    FinancialNetwork() {
        institutionCount = 0;
        for (int i = 0; i < MAX_INSTITUTIONS; i++)
            adjList[i] = nullptr;
    }

    int addInstitution(string name) {
        institutionNames[institutionCount] = name;
        institutionCount++;
        return institutionCount - 1;
    }

    void connectInstitutions(int fromId, int toId, int riskWeight) {
        Edge* edge1 = new Edge{toId, riskWeight, adjList[fromId]};
        adjList[fromId] = edge1;

        Edge* edge2 = new Edge{fromId, riskWeight, adjList[toId]};
        adjList[toId] = edge2;

        cout << "Connected " << institutionNames[fromId]
             << " <-> " << institutionNames[toId]
             << " (risk weight: " << riskWeight << ")" << endl;
    }

    void displayNetwork() {
        cout << "\n--- Financial Network ---" << endl;
        for (int i = 0; i < institutionCount; i++) {
            cout << institutionNames[i] << " connects to: ";
            Edge* temp = adjList[i];
            while (temp != nullptr) {
                cout << institutionNames[temp->to] << "(" << temp->riskWeight << ") ";
                temp = temp->next;
            }
            cout << endl;
        }
    }

    string nameOf(int id) { return institutionNames[id]; }
    int count() { return institutionCount; }
    Edge* neighbors(int id) { return adjList[id]; }

    ~FinancialNetwork() {
        for (int i = 0; i < institutionCount; i++) {
            Edge* temp = adjList[i];
            while (temp != nullptr) {
                Edge* next = temp->next;
                delete temp;
                temp = next;
            }
        }
    }
};

void safestTrade(FinancialNetwork& network, int startId, int endId) {
    int n = network.count();
    int dist[MAX_INSTITUTIONS];
    bool visited[MAX_INSTITUTIONS];
    int parent[MAX_INSTITUTIONS];

    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1;
    }
    dist[startId] = 0;
    for (int count = 0; count < n; count++) {
        int u = -1;
        int minDist = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;
        visited[u] = true;

        Edge* temp = network.neighbors(u);
        while (temp != nullptr) {
            int v = temp->to;
            int weight = temp->riskWeight;
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
            temp = temp->next;
        }
    }

    if (dist[endId] == INT_MAX) {
        cout << "No settlement path exists between these institutions." << endl;
        return;
    }

    cout << "\n--- Safest Trade Path ---" << endl;
    cout << "Total settlement risk: " << dist[endId] << endl;
    int path[MAX_INSTITUTIONS];
    int pathLength = 0;
    int current = endId;
    while (current != -1) {
        path[pathLength] = current;
        pathLength++;
        current = parent[current];
    }

    cout << "Path: ";
    for (int i = pathLength - 1; i >= 0; i--) {
        cout << network.nameOf(path[i]);
        if (i > 0) cout << " -> ";
    }
    cout << endl;
}
struct AssetCategory {
    string name;
    double currentValue;
    double targetPercent;
};

void portfolioBalancer(AssetCategory categories[], int numCategories, double newFunds) {
    double totalValue = newFunds;
    for (int i = 0; i < numCategories; i++)
        totalValue += categories[i].currentValue;

    cout << "\n--- Portfolio Balancer ---" << endl;
    cout << "New funds to allocate: " << newFunds << endl;

    double remaining = newFunds;
    while (remaining > 0.01) {
        int worstIndex = -1;
        double worstGap = -1.0;

        for (int i = 0; i < numCategories; i++) {
            double currentPercent = (categories[i].currentValue / totalValue) * 100.0;
            double gap = categories[i].targetPercent - currentPercent;
            if (gap > worstGap) {
                worstGap = gap;
                worstIndex = i;
            }
        }
        double step = (remaining < 100.0) ? remaining : 100.0;
        categories[worstIndex].currentValue += step;
        remaining -= step;
    }

    cout << "\nFinal allocation:" << endl;
    for (int i = 0; i < numCategories; i++) {
        double finalPercent = (categories[i].currentValue / totalValue) * 100.0;
        cout << categories[i].name
             << " | Target: " << categories[i].targetPercent << "%"
             << " | Actual: " << finalPercent << "%"
             << " | Value: " << categories[i].currentValue
             << endl;
    }
}

void saveLedgerToFile(InvestmentLedger& ledger) {
    ofstream outFile("ledger.csv");
    outFile << "ClientID,Ticker,ClientName,Quantity,PricePerUnit" << endl;

    LedgerNode* temp = ledger.getHead();
    while (temp != nullptr) {
        outFile << temp->record.clientId << ","
                << temp->record.ticker << ","
                << temp->record.clientName << ","
                << temp->record.quantity << ","
                << temp->record.pricePerUnit << endl;
        temp = temp->next;
    }
    outFile.close();
    cout << "Ledger saved to ledger.csv" << endl;
}

void loadDefaultPortfolio(InvestmentLedger& ledger, TickerIndex& index) {
    ledger.addRecord(1, "AAPL", "Ravi Shah", 50, 180.25);
    index.insert(ledger.findByTicker("AAPL"));

    ledger.addRecord(2, "TSLA", "Meera Iyer", 30, 245.10);
    index.insert(ledger.findByTicker("TSLA"));

    ledger.addRecord(3, "GOOG", "Arjun Mehta", 15, 2750.00);
    index.insert(ledger.findByTicker("GOOG"));

    ledger.addRecord(4, "MSFT", "Priya Nair", 40, 410.75);
    index.insert(ledger.findByTicker("MSFT"));

    ledger.addRecord(5, "AMZN", "Karan Verma", 10, 3380.50);
    index.insert(ledger.findByTicker("AMZN"));

    cout << "Starting portfolio stocked with 5 default investments." << endl;
    ledger.displayAll();
}

void loadLedgerFromFile(InvestmentLedger& ledger, TickerIndex& index) {
    ifstream inFile("ledger.csv");
    if (!inFile) {
        cout << "No saved ledger found. Loading default starting portfolio." << endl;
        loadDefaultPortfolio(ledger, index);
        return;
    }

    string line;
    getline(inFile, line);

    while (getline(inFile, line)) {
        int firstComma  = line.find(',');
        int secondComma = line.find(',', firstComma + 1);
        int thirdComma  = line.find(',', secondComma + 1);
        int fourthComma = line.find(',', thirdComma + 1);

        int clientId         = stoi(line.substr(0, firstComma));
        string ticker         = line.substr(firstComma + 1, secondComma - firstComma - 1);
        string clientName     = line.substr(secondComma + 1, thirdComma - secondComma - 1);
        double quantity        = stod(line.substr(thirdComma + 1, fourthComma - thirdComma - 1));
        double pricePerUnit    = stod(line.substr(fourthComma + 1));

        ledger.addRecord(clientId, ticker, clientName, quantity, pricePerUnit);
        index.insert(ledger.findByTicker(ticker));
    }
    inFile.close();
    cout << "Ledger loaded from ledger.csv" << endl;
}
int main() {
    InvestmentLedger ledger;
    EventHistory eventHistory;
    TradeQueue tradeQueue;
    TickerIndex tickerIndex;
    FinancialNetwork network;

    cout << "===== WealthTrack: Digital Banking Asset Valuation Suite =====" << endl;
    loadLedgerFromFile(ledger, tickerIndex);
    int bankA = network.addInstitution("BankA");
    int bankB = network.addInstitution("BankB");
    int bankC = network.addInstitution("BankC");
    int bankD = network.addInstitution("BankD");
    network.connectInstitutions(bankA, bankB, 4);
    network.connectInstitutions(bankA, bankC, 1);
    network.connectInstitutions(bankC, bankB, 2);
    network.connectInstitutions(bankB, bankD, 1);
    network.connectInstitutions(bankC, bankD, 5);

    int choice;

    do {
        cout << "\n1.  Add Investment Record (Ledger)"
             << "\n2.  Ticker Lookup (BST Search)"
             << "\n3.  Submit Trade Request (Trading Line)"
             << "\n4.  Process Next Trade"
             << "\n5.  Record Stock Split (Split Undo)"
             << "\n6.  Undo Last Stock Split"
             << "\n7.  View Event History"
             << "\n8.  View Trading Line"
             << "\n9.  Show Investment Ledger"
             << "\n10. Wealth Sorter (Rank by Portfolio Value)"
             << "\n11. View Financial Network"
             << "\n12. Safest Trade Path (Dijkstra's Algorithm)"
             << "\n13. Portfolio Balancer (Greedy Allocation)"
             << "\n14. Save Ledger to File"
             << "\n0.  Exit"
             << "\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            int id; string ticker, name; double qty, price;
            cout << "Client ID: ";    cin >> id; cin.ignore();
            cout << "Ticker: ";       getline(cin, ticker);
            cout << "Client Name: ";  getline(cin, name);
            cout << "Quantity: ";     cin >> qty;
            cout << "Price Per Unit: "; cin >> price;
            ledger.addRecord(id, ticker, name, qty, price);
            tickerIndex.insert(ledger.findByTicker(ticker));

        } else if (choice == 2) {
            string ticker;
            cout << "Enter Ticker Symbol: ";
            getline(cin, ticker);
            tickerIndex.lookup(ticker);

        } else if (choice == 3) {
            int id; string ticker, action; double qty;
            cout << "Client ID: "; cin >> id; cin.ignore();
            cout << "Ticker: ";    getline(cin, ticker);
            cout << "Action (BUY/SELL): "; getline(cin, action);
            cout << "Quantity: ";  cin >> qty;
            tradeQueue.submitTrade(id, ticker, action, qty);

        } else if (choice == 4) {
            TradeRequest trade;
            if (tradeQueue.processNextTrade(trade)) {
                cout << "Processed: " << trade.action << " "
                     << trade.quantity << " of " << trade.ticker
                     << " for Client #" << trade.clientId << endl;
            }

        } else if (choice == 5) {
            string ticker; double ratio;
            cout << "Ticker: "; getline(cin, ticker);
            Investment* inv = ledger.findByTicker(ticker);
            if (inv == nullptr) {
                cout << "Ticker not found in ledger." << endl;
            } else {
                cout << "Split Ratio (e.g. 2 for 2-for-1 split): ";
                cin >> ratio;
                eventHistory.recordSplit(ticker, inv->quantity, ratio);
                inv->quantity *= ratio;
                cout << "Split applied. New quantity: " << inv->quantity << endl;
            }

        } else if (choice == 6) {
            SplitEvent event;
            if (eventHistory.undoLastSplit(event)) {
                Investment* inv = ledger.findByTicker(event.ticker);
                if (inv != nullptr) {
                    inv->quantity = event.oldQuantity;
                    cout << "Reverted " << event.ticker
                         << " to quantity " << event.oldQuantity << endl;
                }
            }

        } else if (choice == 7)  { eventHistory.displayHistory();
        } else if (choice == 8)  { tradeQueue.displayQueue();
        } else if (choice == 9)  { ledger.displayAll();
        } else if (choice == 10) { wealthSorter(ledger);
        } else if (choice == 11) { network.displayNetwork();

        } else if (choice == 12) {
            cout << "Available institutions: ";
            for (int i = 0; i < network.count(); i++)
                cout << i << "=" << network.nameOf(i) << " ";
            cout << endl;
            int from, to;
            cout << "From institution ID: "; cin >> from;
            cout << "To institution ID: ";   cin >> to;
            safestTrade(network, from, to);

        } else if (choice == 13) {
            AssetCategory categories[3] = {
                {"Stocks", 4000, 50},
                {"Bonds", 3000, 30},
                {"Mutual Funds", 1000, 20}
            };
            double funds;
            cout << "New funds to invest: ";
            cin >> funds;
            portfolioBalancer(categories, 3, funds);

        } else if (choice == 14) { saveLedgerToFile(ledger);
        } else if (choice != 0)  { cout << "Invalid choice." << endl; }

    } while (choice != 0);

    saveLedgerToFile(ledger);
    cout << "Goodbye." << endl;
    return 0;
}
