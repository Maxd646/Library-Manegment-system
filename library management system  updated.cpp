#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

struct Book {
    int id;
    string title;
    string author;
    int copies;
};

struct Member {
    int id;
    string name;
    vector<int> borrowedBooks; // List of book IDs the member has borrowed
};

void clearScreen() {
    system("cls");
}

void pause(int seconds) {
    Sleep(seconds * 1000);
}

void displayBooks(const vector<Book>& library) {
    clearScreen();
    cout << "===================================" << endl;
    cout << "         Library Books             " << endl;
    cout << "===================================" << endl;
    if (library.empty()) {
        cout << "No books available in the library." << endl;
    } else {
        cout << left << setw(10) << "ID"
             << setw(30) << "Title"
             << setw(20) << "Author"
             << setw(10) << "Copies" << endl;
        cout << "-----------------------------------" << endl;
        for (const auto& book : library) {
            cout << left << setw(10) << book.id
                 << setw(30) << book.title
                 << setw(20) << book.author
                 << setw(10) << book.copies << endl;
        }
    }
    cout << "===================================" << endl;
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void addBook(vector<Book>& library) {
    clearScreen();

    int id, copies;
    string title, author;

    cout << "Enter book ID: ";
    cin >> id;
    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter book author: ";
    getline(cin, author);
    cout << "Enter number of copies: ";
    cin >> copies;

    library.push_back({id, title, author, copies});
    cout << "Book added successfully!" << endl;
    pause(2);
}

void updateBookCopies(vector<Book>& library, int id, int new_copies) {
    clearScreen();
    for (auto& book : library) {
        if (book.id == id) {
            book.copies = new_copies;
            cout << "Updated the number of copies for book ID " << id << " to " << new_copies << "." << endl;
            pause(2);
            return;
        }
    }
    cout << "Book with ID " << id << " not found!" << endl;
    pause(2);
}

void removeBook(vector<Book>& library, int id) {
    clearScreen();
    auto it = remove_if(library.begin(), library.end(), [id](const Book& book) {
        return book.id == id;
    });

    if (it != library.end()) {
        library.erase(it, library.end());
        cout << "Book with ID " << id << " removed successfully!" << endl;
        pause(2);
    } else {
        cout << "Book with ID " << id << " not found!" << endl;
        pause(2);
    }
}

void searchBook(const vector<Book>& library, int id) {
    clearScreen();
    for (const auto& book : library) {
        if (book.id == id) {
            cout << "Book found!" << endl;
            cout << "ID: " << book.id << endl;
            cout << "Title: " << book.title << endl;
            cout << "Author: " << book.author << endl;
            cout << "Copies: " << book.copies << endl;
            pause(2);
            return;
        }
    }
    cout << "Book with ID " << id << " not found!" << endl;
    pause(2);
}

void addMember(vector<Member>& members) {
    clearScreen();
    int id;
    string name;

    cout << "Enter member ID: ";
    cin >> id;
    cout << "Enter member name: ";
    cin.ignore();
    getline(cin, name);

    members.push_back({id, name, {}});  // Add a new member to the list
    cout << "Member added successfully!" << endl;
    pause(2);
}

void displayMembers(const vector<Member>& members) {
    clearScreen();
    cout << "===================================" << endl;
    cout << "        Registered Members         " << endl;
    cout << "===================================" << endl;
    if (members.empty()) {
        cout << "No members registered." << endl;
    } else {
        cout << left << setw(10) << "ID"
             << setw(30) << "Name" << endl;
        cout << "-----------------------------------" << endl;
        for (const auto& member : members) {
            cout << left << setw(10) << member.id
                 << setw(30) << member.name << endl;
        }
    }
    cout << "===================================" << endl;
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

void borrowBook(vector<Book>& library, vector<Member>& members, int memberId, int bookId) {
    bool bookFound = false;
    bool memberFound = false;

    for (auto& member : members) {
        if (member.id == memberId) {
            memberFound = true;
            for (auto& book : library) {
                if (book.id == bookId && book.copies > 0) {
                    bookFound = true;
                    member.borrowedBooks.push_back(bookId);
                    book.copies--;  // Decrease the number of available copies
                    cout << "Book borrowed successfully!" << endl;
                    pause(2);
                    return;
                }
            }
            break;
        }
    }

    if (!memberFound) {
        cout << "Member not found!" << endl;
    } else if (!bookFound) {
        cout << "Book not found or no copies available!" << endl;
    }
    pause(2);
}

void saveLibrary(const vector<Book>& library) {
    ofstream outFile("library_data.txt");
    if (outFile.is_open()) {
        for (const auto& book : library) {
            outFile << book.id << '\n'
                    << book.title << '\n'
                    << book.author << '\n'
                    << book.copies << '\n';
        }
        outFile.close();
        cout << "Library data saved successfully!" << endl;
    } else {
        cout << "Error saving library data!" << endl;
    }
    pause(2);
}

void loadLibrary(vector<Book>& library) {
    ifstream inFile("library_data.txt");
    if (inFile.is_open()) {
        library.clear();
        Book book;
        while (inFile >> book.id) {
            inFile.ignore(); // ignore newline
            getline(inFile, book.title);
            getline(inFile, book.author);
            inFile >> book.copies;
            inFile.ignore(); // ignore newline
            library.push_back(book);
        }
        inFile.close();
        cout << "Library data loaded successfully!" << endl;
    } else {
        cout << "Error loading library data!" << endl;
    }
    pause(2);
}

int main() {
    vector<Book> library;
    vector<Member> members;

    int choice;
    do {
        clearScreen();
        cout << "===================================" << endl;
        cout << "     Library Management System     " << endl;
        cout << "===================================" << endl;
        cout << "1. Display Books" << endl;
        cout << "2. Add Book" << endl;
        cout << "3. Update Book Copies" << endl;
        cout << "4. Remove Book" << endl;
        cout << "5. Search Book" << endl;
        cout << "6. Add Member" << endl;
        cout << "7. Display Members" << endl;
        cout << "8. Borrow Book" << endl;
        cout << "9. Save Library Data" << endl;
        cout << "10. Load Library Data" << endl;
        cout << "11. Exit" << endl;
        cout << "===================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number!" << endl;
            pause(2);
            continue;
        }

        switch (choice) {
            case 1:
                displayBooks(library);
                break;
            case 2:
                addBook(library);
                break;
            case 3: {
                int id, new_copies;
                cout << "Enter book ID to update: ";
                cin >> id;
                cout << "Enter new number of copies: ";
                cin >> new_copies;
                updateBookCopies(library, id, new_copies);
                break;
            }
            case 4: {
                int id;
                cout << "Enter book ID to remove: ";
                cin >> id;
                removeBook(library, id);
                break;
            }
            case 5: {
                int id;
                cout << "Enter book ID to search: ";
                cin >> id;
                searchBook(library, id);
                break;
            }
            case 6:
                addMember(members);
                break;
            case 7:
                displayMembers(members);
                break;
            case 8: {
                int memberId, bookId;
                cout << "Enter member ID: ";
                cin >> memberId;
                cout << "Enter book ID to borrow: ";
                cin >> bookId;
                borrowBook(library, members, memberId, bookId);
                break;
            }
            case 9:
                saveLibrary(library);
                break;
            case 10:
                loadLibrary(library);
                break;
            case 11:
                cout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please select a valid option!" << endl;
                pause(2);
                break;
        }
    } while (choice != 11);

    return 0;
}
