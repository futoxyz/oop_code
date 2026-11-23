#pragma once
#include <compare>
#include <cstddef>
#include <iosfwd>
#include <string>

class Book {
public:
    // Пустые название/автор и pages == 0 допустимы для эталона ключа.
    Book(std::string title, std::string author, int year, int pages);
    explicit Book(int year);
    Book(const Book& other);
    Book(Book&& other) noexcept;
    ~Book();

    // Ключ записи после создания менять нельзя.
    Book& operator=(const Book&) = delete;
    Book& operator=(Book&&) = delete;

    const std::string& title() const noexcept;
    const std::string& author() const noexcept;
    int year() const noexcept;
    int pages() const noexcept;
    bool operator==(const Book& other) const noexcept;
    std::strong_ordering operator<=>(const Book& other) const noexcept;
    static std::size_t liveCount() noexcept;

private:
    std::string title_;
    std::string author_;
    int year_;
    int pages_;
    static std::size_t liveCount_;
};

std::ostream& operator<<(std::ostream& out, const Book& book);

class Criterion {
public:
    virtual ~Criterion() = default;
    virtual bool matches(const Book& book) const = 0;
};

class BookSelector final : public Criterion {
public:
    BookSelector(int lo, int hi, int minPages);
    bool matches(const Book& book) const override;

private:
    int lo_;
    int hi_;
    int minPages_;
};

class AllBooks final : public Criterion {
public:
    bool matches(const Book& book) const override;
};
