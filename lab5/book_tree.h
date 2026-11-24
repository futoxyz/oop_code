#pragma once
#include "book.h"
#include <memory>

class Selection {
private:
    std::unique_ptr<std::weak_ptr<const Book>[]> data_;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
public:
    Selection() noexcept = default;
    Selection(const Selection& other);
    Selection(Selection&& other) noexcept;
    Selection& operator=(const Selection& other);
    Selection& operator=(Selection&& other) noexcept;
    virtual ~Selection() = default;

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    bool empty() const noexcept;
    const std::weak_ptr<const Book>& operator[](std::size_t index) const;
    void append(std::weak_ptr<const Book> book);
    void swap(Selection& other) noexcept;
};



class BookTree {
private:
    struct Node;
    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;
    std::unique_ptr<Node>& findLink(const Book& key) noexcept;
    static std::size_t nodeHeight(const Node* node) noexcept;
    static void traverse(const Node* node, const Criterion* criterion, bool reverse, Selection& result);

public:
    BookTree() noexcept = default;
    virtual ~BookTree() = default;
    BookTree(const BookTree&) = delete;
    BookTree& operator=(const BookTree&) = delete;
    BookTree(BookTree&& other) noexcept;
    BookTree& operator=(BookTree&& other) noexcept;

    bool insert(const Book& book);
    std::weak_ptr<const Book> find(const Book& key) const noexcept;
    bool remove(const Book& key) noexcept;
    std::size_t size() const noexcept;
    std::size_t height() const noexcept;
    Selection select(const Criterion& criterion) const;
    Selection inOrder() const;
    Selection reverseOrder() const;
};
