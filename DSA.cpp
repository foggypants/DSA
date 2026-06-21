#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// ---- Book ----
struct Book {
    int id;
    string title;
    string author;
    bool isIssued;
};

// ---- Linked List Node ----
struct BookNode {
    Book book;
    BookNode* next;
};

// ---- Stack Node ----
struct StackNode {
    int bookId;
    string member;
    StackNode* next;
};

// ---- Queue Node ----
struct QueueNode {
    int bookId;
    string member;
    QueueNode* next;
};

// ---- BST Node ----
struct BSTNode {
    Book book;
    BSTNode* left;
    BSTNode* right;
};

// ==========================
// LINKED LIST FUNCTIONS
// ==========================

BookNode* listHead = nullptr;

void addBook(int id, string title, string author) {
    BookNode* newNode = new BookNode;
    newNode->book.id       = id;
    newNode->book.title    = title;
    newNode->book.author   = author;
    newNode->book.isIssued = false;
    newNode->next          = nullptr;

    if (listHead == nullptr) {
        listHead = newNode;
    } else {
        BookNode* temp = listHead;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = newNode;
    }
    cout << "Book added: " << title << endl;
}

void removeBook(int id) {
    if (listHead == nullptr) {
        cout << "No books in library." << endl;
        return;
    }
    if (listHead->book.id == id) {
        BookNode* del = listHead;
        listHead = listHead->next;
        delete del;
        cout << "Book removed." << endl;
        return;
    }
    BookNode* temp = listHead;
    while (temp->next != nullptr && temp->next->book.id != id)
        temp = temp->next;
    if (temp->next == nullptr) {
        cout << "Book not found." << endl;
        return;
    }
    BookNode* del = temp->next;
    temp->next = del->next;
    delete del;
    cout << "Book removed." << endl;
}

Book* findBook(int id) {
    BookNode* temp = listHead;
    while (temp != nullptr) {
        if (temp->book.id == id)
            return &temp->book;
        temp = temp->next;
    }
    return nullptr;
}

void showAllBooks() {
    if (listHead == nullptr) {
        cout << "No books available." << endl;
        return;
    }
    cout << "\n--- All Books ---" << endl;
    BookNode* temp = listHead;
    while (temp != nullptr) {
        cout << "ID: " << temp->book.id
             << " | Title: "  << temp->book.title
             << " | Author: " << temp->book.author
             << " | "         << (temp->book.isIssued ? "Issued" : "Available")
             << endl;
        temp = temp->next;
    }
}

// ==========================
// STACK FUNCTIONS
// ==========================

StackNode* stackTop = nullptr;

void stackPush(int bookId, string member) {
    StackNode* newNode = new StackNode;
    newNode->bookId = bookId;
    newNode->member = member;
    newNode->next   = stackTop;
    stackTop        = newNode;
}

void stackPop() {
    if (stackTop == nullptr) {
        cout << "Issue history is empty." << endl;
        return;
    }
    StackNode* del = stackTop;
    stackTop = stackTop->next;
    cout << "Removed: Book " << del->bookId << " issued to " << del->member << endl;
    delete del;
}

void showIssueHistory() {
    if (stackTop == nullptr) {
        cout << "No issue history." << endl;
        return;
    }
    cout << "\n--- Issue History ---" << endl;
    StackNode* temp = stackTop;
    while (temp != nullptr) {
        cout << "Book ID: " << temp->bookId
             << " | Member: " << temp->member << endl;
        temp = temp->next;
    }
}

// ==========================
// QUEUE FUNCTIONS
// ==========================

QueueNode* queueFront = nullptr;
QueueNode* queueRear  = nullptr;

void enqueue(int bookId, string member) {
    QueueNode* newNode = new QueueNode;
    newNode->bookId = bookId;
    newNode->member = member;
    newNode->next   = nullptr;

    if (queueRear == nullptr) {
        queueFront = queueRear = newNode;
    } else {
        queueRear->next = newNode;
        queueRear       = newNode;
    }
    cout << member << " added to waitlist for Book " << bookId << endl;
}

void dequeue() {
    if (queueFront == nullptr) {
        cout << "Waitlist is empty." << endl;
        return;
    }
    QueueNode* del = queueFront;
    cout << del->member << " is next for Book " << del->bookId << endl;
    queueFront = queueFront->next;
    if (queueFront == nullptr)
        queueRear = nullptr;
    delete del;
}

void showWaitlist() {
    if (queueFront == nullptr) {
        cout << "Waitlist is empty." << endl;
        return;
    }
    cout << "\n--- Waitlist ---" << endl;
    QueueNode* temp = queueFront;
    while (temp != nullptr) {
        cout << "Member: " << temp->member
             << " | Book ID: " << temp->bookId << endl;
        temp = temp->next;
    }
}

// ==========================
// BST FUNCTIONS
// ==========================

BSTNode* bstRoot = nullptr;

BSTNode* bstInsert(BSTNode* node, Book book) {
    if (node == nullptr) {
        BSTNode* newNode  = new BSTNode;
        newNode->book     = book;
        newNode->left     = nullptr;
        newNode->right    = nullptr;
        return newNode;
    }
    if (book.id < node->book.id)
        node->left  = bstInsert(node->left,  book);
    else if (book.id > node->book.id)
        node->right = bstInsert(node->right, book);
    return node;
}

void bstSearch(BSTNode* node, int id) {
    if (node == nullptr) {
        cout << "Book not found." << endl;
        return;
    }
    if (id == node->book.id) {
        cout << "Found: " << node->book.title
             << " | " << node->book.author
             << " | " << (node->book.isIssued ? "Issued" : "Available")
             << endl;
        return;
    }
    if (id < node->book.id)
        bstSearch(node->left,  id);
    else
        bstSearch(node->right, id);
}

void bstInOrder(BSTNode* node) {
    if (node == nullptr) return;
    bstInOrder(node->left);
    cout << "ID: " << node->book.id
         << " | " << node->book.title
         << " | " << node->book.author << endl;
    bstInOrder(node->right);
}

void showSortedBooks() {
    cout << "\n--- Books Sorted by ID ---" << endl;
    bstInOrder(bstRoot);
}

// ==========================
// FILE SAVE AND LOAD
// ==========================

void saveToFile() {
    ofstream outFile("library.csv");
    outFile << "ID,Title,Author,IsIssued" << endl;
    BookNode* temp = listHead;
    while (temp != nullptr) {
        outFile << temp->book.id << ","
                << temp->book.title << ","
                << temp->book.author << ","
                << temp->book.isIssued << endl;
        temp = temp->next;
    }
    outFile.close();
    cout << "Library saved to library.csv" << endl;
}

void loadStartingBooks() {
    addBook(1, "Harry Potter", "J.K. Rowling");
    bstRoot = bstInsert(bstRoot, *findBook(1));

    addBook(2, "The Hobbit", "J.R.R. Tolkien");
    bstRoot = bstInsert(bstRoot, *findBook(2));

    addBook(3, "1984", "George Orwell");
    bstRoot = bstInsert(bstRoot, *findBook(3));

    addBook(4, "To Kill a Mockingbird", "Harper Lee");
    bstRoot = bstInsert(bstRoot, *findBook(4));

    addBook(5, "The Great Gatsby", "F. Scott Fitzgerald");
    bstRoot = bstInsert(bstRoot, *findBook(5));

    cout << "Starting library stocked with 5 default books (IDs 1 to 5)." << endl;
    showAllBooks();
}

void loadFromFile() {
    ifstream inFile("library.csv");
    if (!inFile) {
        cout << "No saved file found. Loading default starting books." << endl;
        loadStartingBooks();
        return;
    }

    string line;
    getline(inFile, line);   // skip the header row (ID,Title,Author,IsIssued)

    while (getline(inFile, line)) {
        int firstComma  = line.find(',');
        int secondComma = line.find(',', firstComma + 1);
        int thirdComma  = line.find(',', secondComma + 1);

        int id          = stoi(line.substr(0, firstComma));
        string title    = line.substr(firstComma + 1, secondComma - firstComma - 1);
        string author   = line.substr(secondComma + 1, thirdComma - secondComma - 1);
        bool issuedFlag = stoi(line.substr(thirdComma + 1));

        addBook(id, title, author);
        Book* b = findBook(id);
        b->isIssued = issuedFlag;
        bstRoot = bstInsert(bstRoot, *b);
    }
    inFile.close();
    cout << "Library loaded from library.csv" << endl;
}

// ==========================
// ISSUE AND RETURN
// ==========================

void issueBook(int id, string member) {
    Book* b = findBook(id);
    if (b == nullptr) {
        cout << "Book not found." << endl;
        return;
    }
    if (b->isIssued) {
        cout << "Already issued. Adding to waitlist." << endl;
        enqueue(id, member);
        return;
    }
    b->isIssued = true;
    stackPush(id, member);
    cout << "\"" << b->title << "\" issued to " << member << endl;
}

void returnBook(int id) {
    Book* b = findBook(id);
    if (b == nullptr) {
        cout << "Book not found." << endl;
        return;
    }
    if (!b->isIssued) {
        cout << "Book was not issued." << endl;
        return;
    }
    b->isIssued = false;
    stackPop();
    cout << "\"" << b->title << "\" returned." << endl;
    cout << "Checking waitlist..." << endl;
    dequeue();
}

// ==========================
// MAIN MENU
// ==========================

int main() {
    int choice;
    cout << "===== Library Management System =====" << endl;
    loadFromFile();

    do {
        cout << "\n1.  Add Book"
             << "\n2.  Remove Book"
             << "\n3.  Issue Book"
             << "\n4.  Return Book"
             << "\n5.  Search Book"
             << "\n6.  Show All Books"
             << "\n7.  Show Sorted Books"
             << "\n8.  Issue History"
             << "\n9.  Waitlist"
             << "\n10. Save Library to File"
             << "\n0.  Exit"
             << "\nChoice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            int id;
            string title, author;
            cout << "ID: ";     cin >> id; cin.ignore();
            cout << "Title: ";  getline(cin, title);
            cout << "Author: "; getline(cin, author);
            addBook(id, title, author);
            bstRoot = bstInsert(bstRoot, *findBook(id));

        } else if (choice == 2) {
            int id;
            cout << "ID: "; cin >> id;
            removeBook(id);

        } else if (choice == 3) {
            int id; string name;
            cout << "ID: ";     cin >> id; cin.ignore();
            cout << "Member: "; getline(cin, name);
            issueBook(id, name);

        } else if (choice == 4) {
            int id;
            cout << "ID: "; cin >> id;
            returnBook(id);

        } else if (choice == 5) {
            int id;
            cout << "ID: "; cin >> id;
            bstSearch(bstRoot, id);

        } else if (choice == 6) { showAllBooks();
        } else if (choice == 7) { showSortedBooks();
        } else if (choice == 8) { showIssueHistory();
        } else if (choice == 9) { showWaitlist();
        } else if (choice == 10) { saveToFile();
        } else if (choice != 0) { cout << "Invalid choice." << endl; }

    } while (choice != 0);

    saveToFile();
    cout << "Goodbye." << endl;
    return 0;
}