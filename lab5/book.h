#pragma once
#include <compare>
#include <string>

class Book {
private:
    std::string title_;
    std::string author_;
    int year_;
    int pages_;
    static std::size_t liveCount_;
public:
    Book(std::string title, std::string author, int year, int pages);
    explicit Book(int year);
    Book(const Book& other);
    Book(Book&& other) noexcept;
    virtual ~Book();
    Book& operator=(const Book&) = delete;
    Book& operator=(Book&&) = delete;

    const std::string& title() const noexcept;
    const std::string& author() const noexcept;
    int year() const noexcept;
    int pages() const noexcept;
    bool operator==(const Book& other) const noexcept;
    std::strong_ordering operator<=>(const Book& other) const noexcept;
    static std::size_t liveCount() noexcept;
};


class Criterion {
public:
    virtual ~Criterion() = default;
    virtual bool matches(const Book& book) const = 0;
};

class BookSelector final : public Criterion {
private:
    int lo_;
    int hi_;
    int minPages_;
public:
    BookSelector(int lo, int hi, int minPages);
    bool matches(const Book& book) const override;
};

class AllBooks final : public Criterion {
public:
    bool matches(const Book& book) const override;
};
