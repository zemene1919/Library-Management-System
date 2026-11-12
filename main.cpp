#include <iostream>
#include <string>
#include <vector>
#include <limits>

struct Book {
    int id;
    std::string title;
    std::string author;
    int totalCopies;
    int availableCopies;
};

struct Member {
    int id;
    std::string name;
    int borrowedCount = 0;
};

int main() {
    std::vector<Book> books;
    std::vector<Member> members;
    int choice = 0;

    while (choice != 7) {
        std::cout << "\n=== Library Management System ===\n"
                  << "1. Add book\n"
                  << "2. Show books\n"
                  << "3. Register member\n"
                  << "4. Borrow book\n"
                  << "5. Return book\n"
                  << "6. Show members\n"
                  << "7. Exit\n"
                  << "Choose an option: ";

        if (!(std::cin >> choice)) {
            std::cout << "Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            Book book{};
            std::cout << "Book ID: ";
            std::cin >> book.id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Title: ";
            std::getline(std::cin, book.title);
            std::cout << "Author: ";
            std::getline(std::cin, book.author);
            std::cout << "Number of copies: ";
            std::cin >> book.totalCopies;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (book.totalCopies < 1) {
                std::cout << "Copies must be at least 1.\n";
            } else {
                book.availableCopies = book.totalCopies;
                books.push_back(book);
                std::cout << "Book added.\n";
            }
        } else if (choice == 2) {
            if (books.empty()) {
                std::cout << "No books in the library yet.\n";
            }
            for (const Book& book : books) {
                std::cout << "ID: " << book.id
                          << " | " << book.title
                          << " by " << book.author
                          << " | Available: " << book.availableCopies
                          << "/" << book.totalCopies << '\n';
            }
        } else if (choice == 3) {
            Member member{};
            std::cout << "Member ID: ";
            std::cin >> member.id;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Member name: ";
            std::getline(std::cin, member.name);

            members.push_back(member);
            std::cout << "Member registered.\n";
        } else if (choice == 4 || choice == 5) {
            int memberId, bookId;
            std::cout << "Member ID: ";
            std::cin >> memberId;
            std::cout << "Book ID: ";
            std::cin >> bookId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            auto member = members.begin();
            while (member != members.end() && member->id != memberId) {
                ++member;
            }

            auto book = books.begin();
            while (book != books.end() && book->id != bookId) {
                ++book;
            }

            if (member == members.end() || book == books.end()) {
                std::cout << "Member or book not found.\n";
            } else if (choice == 4) {
                if (member->borrowedCount >= 3) {
                    std::cout << "This member already has 3 books.\n";
                } else if (book->availableCopies == 0) {
                    std::cout << "No copies are currently available.\n";
                } else {
                    --book->availableCopies;
                    ++member->borrowedCount;
                    std::cout << "Book borrowed.\n";
                }
            } else if (member->borrowedCount == 0 ||
                       book->availableCopies == book->totalCopies) {
                std::cout << "No matching borrowed copy was found.\n";
            } else {
                ++book->availableCopies;
                --member->borrowedCount;
                std::cout << "Book returned.\n";
            }
        } else if (choice == 6) {
            if (members.empty()) {
                std::cout << "No members registered yet.\n";
            }
            for (const Member& member : members) {
                std::cout << "ID: " << member.id
                          << " | " << member.name
                          << " | Borrowed books: "
                          << member.borrowedCount << '\n';
            }
        } else if (choice != 7) {
            std::cout << "Invalid option. Choose 1–7.\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}