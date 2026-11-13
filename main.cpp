#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>

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

bool loadData(std::vector<Book>& books, std::vector<Member>& members) {
    std::ifstream input("library_data.txt");
    if (!input) {
        std::cout << "No saved library data found. Starting with an empty library.\n";
        return true;
    }

    std::string section;
    int count;
    if (!(input >> section >> count) || section != "BOOKS" || count < 0) {
        std::cerr << "Could not read the saved books in library_data.txt.\n";
        return false;
    }
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < count; ++i) {
        Book book{};
        if (!(input >> book.id >> book.totalCopies >> book.availableCopies) ||
            book.totalCopies < 1 || book.availableCopies < 0 ||
            book.availableCopies > book.totalCopies) {
            std::cerr << "Invalid book data in library_data.txt.\n";
            return false;
        }
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!std::getline(input, book.title) || !std::getline(input, book.author)) {
            std::cerr << "Invalid book text in library_data.txt.\n";
            return false;
        }
        books.push_back(book);
    }

    if (!(input >> section >> count) || section != "MEMBERS" || count < 0) {
        std::cerr << "Could not read the saved members in library_data.txt.\n";
        return false;
    }
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < count; ++i) {
        Member member{};
        if (!(input >> member.id >> member.borrowedCount) || member.borrowedCount < 0) {
            std::cerr << "Invalid member data in library_data.txt.\n";
            return false;
        }
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!std::getline(input, member.name)) {
            std::cerr << "Invalid member name in library_data.txt.\n";
            return false;
        }
        members.push_back(member);
    }

    if (input >> section) {
        std::cerr << "Unexpected extra data in library_data.txt.\n";
        return false;
    }

    return true;
}

bool saveData(const std::vector<Book>& books, const std::vector<Member>& members) {
    std::ofstream output("library_data.txt");
    if (!output) {
        std::cerr << "Could not open library_data.txt for saving.\n";
        return false;
    }

    output << "BOOKS " << books.size() << '\n';
    for (const Book& book : books) {
        output << book.id << ' ' << book.totalCopies << ' ' << book.availableCopies
               << '\n' << book.title << '\n' << book.author << '\n';
    }

    output << "MEMBERS " << members.size() << '\n';
    for (const Member& member : members) {
        output << member.id << ' ' << member.borrowedCount
               << '\n' << member.name << '\n';
    }

    if (!output) {
        std::cerr << "Failed while writing library_data.txt.\n";
        return false;
    }

    return true;
}

int main() {
    std::vector<Book> books;
    std::vector<Member> members;
    int choice = 0;

    if (!loadData(books, members)) {
        return 1;
    }

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

    if (!saveData(books, members)) {
        return 1;
    }

    std::cout << "Library data saved.\nGoodbye!\n";
    return 0;
}