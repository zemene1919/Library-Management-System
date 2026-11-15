#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>

const int MAX_BORROWED_BOOKS = 3;
const int MAX_LOAN_DAYS = 5;
const int FINE_PER_LATE_DAY = 10;

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

struct Loan {
    int memberId;
    int bookId;
};

bool confirmOperation() {
    char confirmation;
    std::cout << "Keep this operation? Enter n to reset/cancel it (y/n): ";
    if (!(std::cin >> confirmation)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Operation cancelled.\n";
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (confirmation != 'y' && confirmation != 'Y') {
        std::cout << "Operation cancelled. No changes were made.\n";
        return false;
    }
    return true;
}

bool loadData(std::vector<Book>& books, std::vector<Member>& members,
              std::vector<Loan>& loans) {
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
        if (section != "LOANS" || !(input >> count) || count < 0) {
            std::cerr << "Could not read the saved loans in library_data.txt.\n";
            return false;
        }
        for (int i = 0; i < count; ++i) {
            Loan loan{};
            if (!(input >> loan.memberId >> loan.bookId)) {
                std::cerr << "Invalid loan data in library_data.txt.\n";
                return false;
            }
            loans.push_back(loan);
        }
    }

    if (input >> section) {
        std::cerr << "Unexpected extra data in library_data.txt.\n";
        return false;
    }

    return true;
}

bool saveData(const std::vector<Book>& books, const std::vector<Member>& members,
              const std::vector<Loan>& loans) {
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

    output << "LOANS " << loans.size() << '\n';
    for (const Loan& loan : loans) {
        output << loan.memberId << ' ' << loan.bookId << '\n';
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
    std::vector<Loan> loans;
    int choice = 0;

    if (!loadData(books, members, loans)) {
        return 1;
    }

    while (choice != 7) {
        std::cout << "\n=== Library Management System ===\n"
                  << "Loan period: " << MAX_LOAN_DAYS
                  << " days | Late fee: " << FINE_PER_LATE_DAY
                  << " birr/day | Book limit per member: "
                  << MAX_BORROWED_BOOKS << '\n'
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
            } else if (confirmOperation()) {
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

            if (confirmOperation()) {
                members.push_back(member);
                std::cout << "Member registered.\n";
            }
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
                if (member->borrowedCount >= MAX_BORROWED_BOOKS) {
                    std::cout << "This member already has the maximum of "
                              << MAX_BORROWED_BOOKS << " books.\n";
                } else if (book->availableCopies == 0) {
                    std::cout << "No copies are currently available.\n";
                } else if (confirmOperation()) {
                    --book->availableCopies;
                    ++member->borrowedCount;
                    loans.push_back({memberId, bookId});
                    std::cout << "Book borrowed.\n";
                }
            } else {
                long long daysKept;
                std::cout << "Days the student kept the book: ";
                if (!(std::cin >> daysKept) || daysKept < 0 ||
                    daysKept > std::numeric_limits<long long>::max() /
                                   FINE_PER_LATE_DAY + MAX_LOAN_DAYS) {
                    std::cout << "Enter a non-negative number of days within the supported range.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                auto loan = loans.begin();
                while (loan != loans.end() &&
                       (loan->memberId != memberId || loan->bookId != bookId)) {
                    ++loan;
                }

                int knownLoans = 0;
                for (const Loan& recordedLoan : loans) {
                    if (recordedLoan.memberId == memberId) {
                        ++knownLoans;
                    }
                }
                const bool legacyLoan = loan == loans.end() &&
                    knownLoans < member->borrowedCount &&
                    book->availableCopies < book->totalCopies;

                if (loan == loans.end() && !legacyLoan) {
                    std::cout << "No matching borrowed copy was found.\n";
                } else if (confirmOperation()) {
                    if (loan != loans.end()) {
                        loans.erase(loan);
                    }
                    ++book->availableCopies;
                    --member->borrowedCount;

                    const long long lateDays =
                        daysKept > MAX_LOAN_DAYS ? daysKept - MAX_LOAN_DAYS : 0;
                    const long long fine = lateDays * FINE_PER_LATE_DAY;
                    std::cout << "Book returned. Late fee: " << fine << " birr";
                    if (lateDays > 0) {
                        std::cout << " (" << lateDays << " late day(s) at "
                                  << FINE_PER_LATE_DAY << " birr per day)";
                    }
                    std::cout << ".\n";
                    if (legacyLoan) {
                        std::cout << "This was an older loan without a saved book record.\n";
                    }
                }
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

    if (!saveData(books, members, loans)) {
        return 1;
    }

    std::cout << "Library data saved.\nGoodbye!\n";
    return 0;
}