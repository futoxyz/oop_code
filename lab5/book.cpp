#include "book.h"
#include <stdexcept>
#include <utility>

std::size_t Book::liveCount_ = 0;

Book::Book(std::string title, std::string author, int year, int pages)
    : title_(std::move(title)), author_(std::move(author)), year_(year), pages_(pages) {
    if (year < 1 || pages < 0) {
        throw std::invalid_argument("Year must be positive; pages must be nonnegative");
    }
    ++liveCount_;
}

Book::Book(int year) : Book("", "", year, 0) {}

Book::Book(const Book& other)
    : Book(other.title_, other.author_, other.year_, other.pages_) {}

Book::Book(Book&& other) noexcept
    : title_(std::move(other.title_)), author_(std::move(other.author_)),
      year_(other.year_), pages_(other.pages_) {
    ++liveCount_;
}

Book::~Book() { --liveCount_; }

const std::string& Book::title() const noexcept { return title_; }
const std::string& Book::author() const noexcept { return author_; }
int Book::year() const noexcept { return year_; }
int Book::pages() const noexcept { return pages_; }
bool Book::operator==(const Book& other) const noexcept { return year_ == other.year_; }
std::strong_ordering Book::operator<=>(const Book& other) const noexcept {
    return year_ <=> other.year_;
}

std::size_t Book::liveCount() noexcept { return liveCount_; }

BookSelector::BookSelector(int lo, int hi, int minPages)
    : lo_(lo), hi_(hi), minPages_(minPages) {
    if (lo < 1 || hi < lo || minPages < 0) {
        throw std::invalid_argument("Invalid year interval or page threshold");
    }
}

bool BookSelector::matches(const Book& book) const {
    return lo_ <= book.year() && book.year() < hi_ && book.pages() >= minPages_;
}

bool AllBooks::matches(const Book&) const {
    return true;
}
