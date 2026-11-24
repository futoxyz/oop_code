#include "book_tree.h"
#include <gtest/gtest.h>
#include <initializer_list>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>


static_assert(!std::is_copy_constructible_v<BookTree>);
static_assert(!std::is_copy_assignable_v<BookTree>);
static_assert(std::is_abstract_v<Criterion>);
static_assert(std::has_virtual_destructor_v<Criterion>);

namespace {
TEST(Book, invalid_fields) {
    EXPECT_THROW(Book(0), std::invalid_argument);
    EXPECT_THROW(Book(-1), std::invalid_argument);
    EXPECT_THROW(Book("", "", 2000, -1), std::invalid_argument);
    EXPECT_EQ(Book::liveCount(), 0);
}

TEST(Book, live_count) {
    {
        Book original("Title", "Author", 2000, 200);
        EXPECT_EQ(Book::liveCount(), 1);
        const Book copy(original);
        EXPECT_EQ(Book::liveCount(), 2);
        const Book moved(std::move(original));
        EXPECT_EQ(Book::liveCount(), 3);
        EXPECT_EQ(copy.title(), "Title");
        EXPECT_EQ(moved.author(), "Author");
        EXPECT_EQ(moved.year(), 2000);
    }
    EXPECT_EQ(Book::liveCount(), 0);
}

TEST(BookTree, duplicate_year) {
    BookTree tree;
    EXPECT_TRUE(tree.insert(Book("First", "A", 2000, 100)));
    EXPECT_EQ(tree.size(), 1);
    EXPECT_FALSE(tree.insert(Book("Duplicate", "B", 2000, 900)));
    EXPECT_EQ(tree.size(), 1);

    const auto stored = tree.find(Book(2000)).lock();
    ASSERT_TRUE(stored);
    EXPECT_EQ(stored->title(), "First");
    EXPECT_EQ(Book::liveCount(), 1);
}

TEST(BookTree, finds_own) {
    BookTree tree;
    {
        const Book original("Original", "Author", 2000, 123);
        ASSERT_TRUE(tree.insert(original));
        const auto stored = tree.find(Book(2000)).lock();
        ASSERT_TRUE(stored);
        EXPECT_NE(stored.get(), &original);
        EXPECT_EQ(Book::liveCount(), 2);
    }
    const auto stored = tree.find(Book(2000)).lock();
    ASSERT_TRUE(stored);
    EXPECT_EQ(stored->title(), "Original");
    EXPECT_EQ(stored->author(), "Author");
    EXPECT_EQ(stored->pages(), 123);
}

TEST(BookTree, find_missing_or_removed) {
    BookTree tree;
    EXPECT_TRUE(tree.find(Book(2000)).expired());
    tree.insert(Book(2000));
    EXPECT_TRUE(tree.find(Book(1999)).expired());
    const auto old = tree.find(Book(2000));
    ASSERT_TRUE(tree.remove(Book(2000)));
    EXPECT_TRUE(tree.find(Book(2000)).expired());
    EXPECT_TRUE(old.expired());
}

TEST(BookTree, height) {
    BookTree tree;
    EXPECT_EQ(tree.height(), 0);
    tree.insert(Book(1950));
    EXPECT_EQ(tree.height(), 1);
    tree.insert(Book(1930));
    tree.insert(Book(1980));
    EXPECT_EQ(tree.height(), 2);
    tree.insert(Book(1960));
    tree.insert(Book(1970));
    EXPECT_EQ(tree.height(), 4);
}

void expectYears(const Selection& selection, std::initializer_list<int> years) {
    ASSERT_EQ(selection.size(), years.size());
    std::size_t i = 0;
    for (int year : years) {
        const auto book = selection[i].lock();
        ASSERT_TRUE(book);
        EXPECT_EQ(book->year(), year);
        ++i;
    }
}
void fillTree(BookTree& tree) {
    tree.insert(Book("Dune", "Herbert", 1965, 412));
    tree.insert(Book("Foundation", "Asimov", 1951, 255));
    tree.insert(Book("Neuromancer", "Gibson", 1984, 271));
    tree.insert(Book("Solaris", "Lem", 1961, 204));
    tree.insert(Book("TheDispossessed", "LeGuin", 1974, 341));
    tree.insert(Book("SnowCrash", "Stephenson", 1992, 480));
}


TEST(BookTree, order) {
    BookTree tree;
    tree.insert(Book(2003));
    expectYears(tree.inOrder(), {2003});
    tree.insert(Book(2001));
    expectYears(tree.inOrder(), {2001, 2003});
    tree.insert(Book(2005));
    expectYears(tree.inOrder(), {2001, 2003, 2005});
    tree.insert(Book(2002));
    expectYears(tree.inOrder(), {2001, 2002, 2003, 2005});
    tree.insert(Book(2004));
    expectYears(tree.inOrder(), {2001, 2002, 2003, 2004, 2005});
    expectYears(tree.reverseOrder(), {2005, 2004, 2003, 2002, 2001});
}

TEST(BookTree, remove_leaf) {
    BookTree tree;
    fillTree(tree);
    ASSERT_TRUE(tree.remove(Book(1961)));
    EXPECT_EQ(tree.size(), 5u);
    expectYears(tree.inOrder(), {1951, 1965, 1974, 1984, 1992});
}

TEST(BookTree, remove_node_right_child) {
    BookTree tree;
    fillTree(tree);
    ASSERT_TRUE(tree.remove(Book(1951)));
    EXPECT_EQ(tree.size(), 5);
    expectYears(tree.inOrder(), {1961, 1965, 1974, 1984, 1992});
}

TEST(BookTree, remove_node_left_child) {
    BookTree tree;
    tree.insert(Book(2000));
    tree.insert(Book(1990));
    tree.insert(Book(1980));
    ASSERT_TRUE(tree.remove(Book(1990)));
    EXPECT_EQ(tree.size(), 2);
    expectYears(tree.inOrder(), {1980, 2000});
}

TEST(BookTree, remove_ending) {
    BookTree tree;
    tree.insert(Book(2000));
    tree.insert(Book(2001));
    ASSERT_TRUE(tree.remove(Book(2000)));
    EXPECT_EQ(tree.size(), 1);
    expectYears(tree.inOrder(), {2001});

    ASSERT_TRUE(tree.remove(Book(2001)));
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.height(), 0);
    EXPECT_EQ(Book::liveCount(), 0);
}

TEST(BookTree, remove_missing) {
    BookTree tree;
    EXPECT_FALSE(tree.remove(Book(2000)));
    fillTree(tree);
    EXPECT_FALSE(tree.remove(Book(2000)));
    EXPECT_EQ(tree.size(), 6);
    expectYears(tree.inOrder(), {1951, 1961, 1965, 1974, 1984, 1992});
}

TEST(Criterion, bounds) {
    const BookSelector criterion(1960, 1990, 250);
    EXPECT_FALSE(criterion.matches(Book("", "", 1959, 500)));
    EXPECT_TRUE(criterion.matches(Book("", "", 1960, 250)));
    EXPECT_FALSE(criterion.matches(Book("", "", 1960, 249)));
    EXPECT_TRUE(criterion.matches(Book("", "", 1989, 300)));
    EXPECT_FALSE(criterion.matches(Book("", "", 1990, 500)));
    EXPECT_FALSE(BookSelector(2000, 2000, 0).matches(Book(2000)));
    EXPECT_THROW(BookSelector(2000, 1999, 0), std::invalid_argument);
    EXPECT_THROW(BookSelector(0, 2000, 0), std::invalid_argument);
    EXPECT_THROW(BookSelector(1900, 2000, -1), std::invalid_argument);
}

TEST(Criterion, filters) {
    BookTree tree;
    fillTree(tree);
    expectYears(tree.select(AllBooks()), {1951, 1961, 1965, 1974, 1984, 1992});
    const Selection selected = tree.select(BookSelector(1960, 1990, 250));
    ASSERT_NO_FATAL_FAILURE(expectYears(selected, {1965, 1974, 1984}));
    EXPECT_EQ(selected[0].lock()->title(), "Dune");
    EXPECT_EQ(selected[1].lock()->author(), "LeGuin");
    EXPECT_EQ(selected[2].lock()->pages(), 271);
}

TEST(Criterion, empty) {
    BookTree tree;
    EXPECT_TRUE(tree.select(AllBooks()).empty());
    EXPECT_TRUE(tree.inOrder().empty());
    EXPECT_TRUE(tree.reverseOrder().empty());

    fillTree(tree);
    const Selection selected = tree.select(BookSelector(2000, 2010, 0));
    EXPECT_TRUE(selected.empty());
    EXPECT_EQ(selected.capacity(), 0u);
}

TEST(Selection, copy_weak) {
    BookTree tree;
    fillTree(tree);
    const Selection original = tree.select(BookSelector(1960, 1990, 250));
    Selection copy(original);
    ASSERT_EQ(copy.size(), 3);
    EXPECT_NE(&original[0], &copy[0]);
    EXPECT_EQ(original[0].lock(), copy[0].lock()); 
    copy.append({});
    EXPECT_EQ(copy.size(), 4);
    EXPECT_EQ(original.size(), 3);
    EXPECT_EQ(Book::liveCount(), 6);

    ASSERT_TRUE(tree.remove(Book(1965)));
    EXPECT_EQ(original.size(), 3);
    EXPECT_EQ(copy.size(), 4);
    EXPECT_TRUE(original[0].expired());
    EXPECT_TRUE(copy[0].expired());
    EXPECT_FALSE(copy[1].expired());
}

TEST(Lifetime, removal) {
    Selection selected;
    {
        BookTree tree;
        fillTree(tree);
        selected = tree.inOrder();
        EXPECT_EQ(Book::liveCount(), 6u);

        ASSERT_TRUE(tree.remove(Book(1965)));
        EXPECT_EQ(Book::liveCount(), 5u);
        EXPECT_TRUE(selected[2].expired());
    }
    EXPECT_EQ(Book::liveCount(), 0u);
    ASSERT_EQ(selected.size(), 6u);
    for (std::size_t i = 0; i < selected.size(); ++i) {
        EXPECT_TRUE(selected[i].expired());
    }
}

TEST(Lifetime, lock) {
    BookTree tree;
    tree.insert(Book(2000));
    const auto weak = tree.find(Book(2000));
    auto book = weak.lock();
    ASSERT_TRUE(book);

    ASSERT_TRUE(tree.remove(Book(2000)));
    EXPECT_FALSE(weak.expired());
    EXPECT_EQ(Book::liveCount(), 1u);
    book.reset();
    EXPECT_TRUE(weak.expired());
    EXPECT_EQ(Book::liveCount(), 0u);
}
}