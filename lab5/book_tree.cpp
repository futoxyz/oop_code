#include "book_tree.h"
#include <algorithm>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>


struct BookTree::Node {
    explicit Node(const Book& value) : book(std::make_shared<Book>(value)) {}

    std::shared_ptr<Book> book;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

BookTree::BookTree() noexcept = default;
BookTree::~BookTree() = default;

BookTree::BookTree(BookTree&& other) noexcept
    : root_(std::move(other.root_)), size_(other.size_) {
    other.size_ = 0;
}

BookTree& BookTree::operator=(BookTree&& other) noexcept {
    if (this != &other) {
        root_ = std::move(other.root_);
        size_ = other.size_;
        other.size_ = 0;
    }
    return *this;
}

std::unique_ptr<BookTree::Node>& BookTree::findLink(const Book& key) noexcept {
    // Идём по владельцам узлов: root_, left или right.
    std::unique_ptr<Node>* link = &root_;
    while (*link && key != *(*link)->book) {
        Node& node = **link;
        link = key < *node.book ? &node.left : &node.right;
    }
    return *link;
}

bool BookTree::insert(const Book& book) {
    auto& link = findLink(book);
    if (link) {
        return false;
    }
    link = std::make_unique<Node>(book);
    ++size_;
    return true;
}

std::weak_ptr<const Book> BookTree::find(const Book& key) const noexcept {
    const Node* node = root_.get();
    while (node) {
        if (key == *node->book) {
            return node->book;
        }
        node = key < *node->book ? node->left.get() : node->right.get();
    }
    return {};
}

bool BookTree::remove(const Book& key) noexcept {
    auto& link = findLink(key);
    if (!link) {
        return false;
    }

    if (!link->left || !link->right) {
        // Держим узел живым, пока забираем его единственного ребёнка.
        auto removed = std::move(link);
        link = removed->left ? std::move(removed->left) : std::move(removed->right);
    } else {
        // Преемник — минимум правого поддерева. Сама книга остаётся той же.
        auto* successorLink = &link->right;
        while ((*successorLink)->left) {
            successorLink = &(*successorLink)->left;
        }
        auto successor = std::move(*successorLink);
        link->book = std::move(successor->book);
        *successorLink = std::move(successor->right);
    }
    --size_;
    return true;
}

std::size_t BookTree::size() const noexcept { return size_; }

std::size_t BookTree::nodeHeight(const Node* node) noexcept {
    if (!node) {
        return 0;
    }
    return 1 + std::max(nodeHeight(node->left.get()), nodeHeight(node->right.get()));
}

std::size_t BookTree::height() const noexcept { return nodeHeight(root_.get()); }

void BookTree::traverse(const Node* node, const Criterion* criterion,
                        bool reverse, Selection& result) {
    if (!node) {
        return;
    }
    const Node* first = node->left.get();
    const Node* second = node->right.get();
    if (reverse) {
        std::swap(first, second);
    }
    traverse(first, criterion, reverse, result);
    if (!criterion || criterion->matches(*node->book)) {
        result.append(node->book);
    }
    traverse(second, criterion, reverse, result);
}

Selection BookTree::select(const Criterion& criterion) const {
    Selection result;
    traverse(root_.get(), &criterion, false, result);
    return result;
}

Selection BookTree::inOrder() const {
    Selection result;
    traverse(root_.get(), nullptr, false, result);
    return result;
}

Selection BookTree::reverseOrder() const {
    Selection result;
    traverse(root_.get(), nullptr, true, result);
    return result;
}

Selection::Selection(const Selection& other)
    : size_(other.size_), capacity_(other.capacity_) {
    if (capacity_ > 0) {
        data_ = std::make_unique<std::weak_ptr<const Book>[]>(capacity_);
    }
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

Selection::Selection(Selection&& other) noexcept
    : data_(std::move(other.data_)), size_(other.size_), capacity_(other.capacity_) {
    other.size_ = 0;
    other.capacity_ = 0;
}

Selection& Selection::operator=(const Selection& other) {
    if (this != &other) {
        Selection copy(other);
        swap(copy);
    }
    return *this;
}

Selection& Selection::operator=(Selection&& other) noexcept {
    if (this != &other) {
        data_ = std::move(other.data_);
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

std::size_t Selection::size() const noexcept { return size_; }
std::size_t Selection::capacity() const noexcept { return capacity_; }
bool Selection::empty() const noexcept { return size_ == 0; }

const std::weak_ptr<const Book>& Selection::operator[](std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Selection index out of range");
    }
    return data_[index];
}

void Selection::append(std::weak_ptr<const Book> book) {
    if (size_ == capacity_) {
        if (capacity_ > std::numeric_limits<std::size_t>::max() / 2) {
            throw std::length_error("Selection capacity overflow");
        }
        const std::size_t nextCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
        auto next = std::make_unique<std::weak_ptr<const Book>[]>(nextCapacity);
        for (std::size_t i = 0; i < size_; ++i) {
            next[i] = data_[i];
        }
        data_ = std::move(next);
        capacity_ = nextCapacity;
    }
    data_[size_] = std::move(book);
    ++size_;
}

void Selection::swap(Selection& other) noexcept {
    data_.swap(other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

std::ostream& operator<<(std::ostream& out, const Selection& selection) {
    out << selection.size() << '\n';
    for (std::size_t i = 0; i < selection.size(); ++i) {
        if (const auto book = selection[i].lock()) {
            out << *book << '\n';
        } else {
            out << "[expired]\n";
        }
    }
    return out;
}