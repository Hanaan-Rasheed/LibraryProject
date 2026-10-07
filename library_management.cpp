/*
 * Library Management System
 * Data Structures used:
 *   1. Singly Linked List -> stores all the books in the library
 *   2. Queue (linked)     -> waiting list for each book (FIFO)
 *
 * Features:
 *   - Add a book
 *   - Display all books
 *   - Search a book by ID
 *   - Issue a book (if already issued, student joins the waiting queue)
 *   - Return a book (next student in the queue gets it automatically)
 *   - View waiting queue of a book
 *   - Delete a book
 */

#include <iostream>
#include <string>
#include <limits>
using namespace std;

/* ---------------------------------------------------------
   QUEUE: waiting list of students (implemented using nodes)
   --------------------------------------------------------- */
struct QueueNode {
    string studentName;
    QueueNode* next;
    QueueNode(string name) : studentName(name), next(NULL) {}
};

class WaitingQueue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    WaitingQueue() : front(NULL), rear(NULL) {}

    bool isEmpty() { return front == NULL; }

    // add student at the rear
    void enqueue(string name) {
        QueueNode* newNode = new QueueNode(name);
        if (rear == NULL) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // remove student from the front and return the name
    string dequeue() {
        if (isEmpty()) return "";
        QueueNode* temp = front;
        string name = temp->studentName;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete temp;
        return name;
    }

    void display() {
        if (isEmpty()) {
            cout << "   (no one is waiting)\n";
            return;
        }
        QueueNode* temp = front;
        int pos = 1;
        while (temp != NULL) {
            cout << "   " << pos++ << ". " << temp->studentName << "\n";
            temp = temp->next;
        }
    }

    void clear() {
        while (!isEmpty()) dequeue();
    }
};

/* ---------------------------------------------------------
   LINKED LIST: each node is one book
   --------------------------------------------------------- */
struct Book {
    int id;
    string title;
    string author;
    bool isIssued;
    string issuedTo;
    WaitingQueue waitList;   // queue of students waiting for this book
    Book* next;

    Book(int i, string t, string a)
        : id(i), title(t), author(a), isIssued(false), issuedTo(""), next(NULL) {}
};

class Library {
private:
    Book* head;

    Book* findBook(int id) {
        Book* temp = head;
        while (temp != NULL) {
            if (temp->id == id) return temp;
            temp = temp->next;
        }
        return NULL;
    }

public:
    Library() : head(NULL) {}

    ~Library() {
        while (head != NULL) {
            Book* temp = head;
            head = head->next;
            temp->waitList.clear();
            delete temp;
        }
    }

    // 1. Add a book (inserted at the end of the list)
    void addBook(int id, string title, string author) {
        if (findBook(id) != NULL) {
            cout << "Book with this ID already exists!\n";
            return;
        }
        Book* newBook = new Book(id, title, author);
        if (head == NULL) {
            head = newBook;
        } else {
            Book* temp = head;
            while (temp->next != NULL) temp = temp->next;
            temp->next = newBook;
        }
        cout << "Book added successfully.\n";
    }

    // 2. Display all books
    void displayBooks() {
        if (head == NULL) {
            cout << "No books in the library.\n";
            return;
        }
        cout << "\n----------------- BOOK LIST -----------------\n";
        Book* temp = head;
        while (temp != NULL) {
            cout << "ID     : " << temp->id << "\n";
            cout << "Title  : " << temp->title << "\n";
            cout << "Author : " << temp->author << "\n";
            if (temp->isIssued)
                cout << "Status : Issued to " << temp->issuedTo << "\n";
            else
                cout << "Status : Available\n";
            cout << "---------------------------------------------\n";
            temp = temp->next;
        }
    }

    // 3. Search a book
    void searchBook(int id) {
        Book* b = findBook(id);
        if (b == NULL) {
            cout << "Book not found.\n";
            return;
        }
        cout << "Found -> " << b->title << " by " << b->author << " | ";
        if (b->isIssued)
            cout << "Issued to " << b->issuedTo << "\n";
        else
            cout << "Available\n";
    }

    // 4. Issue a book (or join waiting queue)
    void issueBook(int id, string student) {
        Book* b = findBook(id);
        if (b == NULL) {
            cout << "Book not found.\n";
            return;
        }
        if (!b->isIssued) {
            b->isIssued = true;
            b->issuedTo = student;
            cout << "Book \"" << b->title << "\" issued to " << student << ".\n";
        } else {
            b->waitList.enqueue(student);
            cout << "Book is already issued to " << b->issuedTo << ".\n";
            cout << student << " has been added to the waiting queue.\n";
        }
    }

    // 5. Return a book (next student in queue gets it)
    void returnBook(int id) {
        Book* b = findBook(id);
        if (b == NULL) {
            cout << "Book not found.\n";
            return;
        }
        if (!b->isIssued) {
            cout << "This book was not issued.\n";
            return;
        }
        cout << b->issuedTo << " returned \"" << b->title << "\".\n";
        if (b->waitList.isEmpty()) {
            b->isIssued = false;
            b->issuedTo = "";
            cout << "Book is now available.\n";
        } else {
            string nextStudent = b->waitList.dequeue();
            b->issuedTo = nextStudent;
            cout << "Book automatically issued to " << nextStudent
                 << " (next in the waiting queue).\n";
        }
    }

    // 6. Show waiting queue of a book
    void showWaitingQueue(int id) {
        Book* b = findBook(id);
        if (b == NULL) {
            cout << "Book not found.\n";
            return;
        }
        cout << "Waiting queue for \"" << b->title << "\":\n";
        b->waitList.display();
    }

    // 7. Delete a book
    void deleteBook(int id) {
        Book* temp = head;
        Book* prev = NULL;
        while (temp != NULL && temp->id != id) {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL) {
            cout << "Book not found.\n";
            return;
        }
        if (prev == NULL) head = temp->next;
        else prev->next = temp->next;
        temp->waitList.clear();
        delete temp;
        cout << "Book deleted.\n";
    }
};

/* ---------------------------------------------------------
   Helper functions for safe input
   --------------------------------------------------------- */
int readInt() {
    int x;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}

string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

/* ---------------------------------------------------------
   MAIN: menu driven program
   --------------------------------------------------------- */
int main() {
    Library lib;
    int choice;

    do {
        cout << "\n====== LIBRARY MANAGEMENT SYSTEM ======\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "6. View Waiting Queue of a Book\n";
        cout << "7. Delete Book\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        choice = readInt();

        switch (choice) {
            case 1: {
                cout << "Enter Book ID: ";
                int id = readInt();
                string title = readLine("Enter Title: ");
                string author = readLine("Enter Author: ");
                lib.addBook(id, title, author);
                break;
            }
            case 2:
                lib.displayBooks();
                break;
            case 3: {
                cout << "Enter Book ID: ";
                lib.searchBook(readInt());
                break;
            }
            case 4: {
                cout << "Enter Book ID: ";
                int id = readInt();
                string student = readLine("Enter Student Name: ");
                lib.issueBook(id, student);
                break;
            }
            case 5: {
                cout << "Enter Book ID: ";
                lib.returnBook(readInt());
                break;
            }
            case 6: {
                cout << "Enter Book ID: ";
                lib.showWaitingQueue(readInt());
                break;
            }
            case 7: {
                cout << "Enter Book ID: ";
                lib.deleteBook(readInt());
                break;
            }
            case 0:
                cout << "Thank you for using the Library System!\n";
                break;
            default:
                cout << "Invalid choice, try again.\n";
        }
    } while (choice != 0);

    return 0;
}
