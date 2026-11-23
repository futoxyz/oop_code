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

// Данные из main. Год 1965 — корень, у него два ребёнка.
void fillTree(BookTree& tree) {
    tree.insert(Book("Dune", "Herbert", 1965, 412));
    tree.insert(Book("Foundation", "Asimov", 1951, 255));
    tree.insert(Book("Neuromancer", "Gibson", 1984, 271));
    tree.insert(Book("Solaris", "Lem", 1961, 204));
    tree.insert(Book("TheDispossessed", "LeGuin", 1974, 341));
    tree.insert(Book("SnowCrash", "Stephenson", 1992, 480));
}

TEST(Book, comparisons_use_year) {
    const Book first("A", "One", 2000, 100);
    const Book sameYear("B", "Two", 2000, 500);
    const Book key(2001);

    EXPECT_EQ(first, sameYear);
    EXPECT_FALSE(first != sameYear);
    EXPECT_FALSE(first < sameYear);
    EXPECT_FALSE(sameYear < first);
    EXPECT_TRUE(first <= sameYear);
    EXPECT_TRUE(first >= sameYear);
    EXPECT_NE(first, key);
    EXPECT_TRUE(first < key);
    EXPECT_FALSE(key < first);
    EXPECT_TRUE(key > first);
    EXPECT_FALSE(first >= key);
    EXPECT_FALSE(key <= first);
    EXPECT_TRUE(key.title().empty());
    EXPECT_TRUE(key.author().empty());
    EXPECT_EQ(key.pages(), 0);
}

TEST(Book, rejects_invalid_fields) {
    EXPECT_THROW(Book(0), std::invalid_argument);
    EXPECT_THROW(Book(-1), std::invalid_argument);
    EXPECT_THROW(Book("", "", 2000, -1), std::invalid_argument);
    EXPECT_EQ(Book::liveCount(), 0u);
}

TEST(Book, counts_copies_and_moved_objects) {
    {
        Book original("Title", "Author", 2000, 200);
        EXPECT_EQ(Book::liveCount(), 1u);
        const Book copy(original);
        EXPECT_EQ(Book::liveCount(), 2u);
        const Book moved(std::move(original));
        EXPECT_EQ(Book::liveCount(), 3u);
        EXPECT_EQ(copy.title(), "Title");
        EXPECT_EQ(moved.author(), "Author");
        EXPECT_EQ(moved.year(), 2000);
    }
    EXPECT_EQ(Book::liveCount(), 0u);
}

TEST(BookTree, rejects_duplicate_year) {
    BookTree tree;
    EXPECT_TRUE(tree.insert(Book("First", "A", 2000, 100)));
    EXPECT_EQ(tree.size(), 1u);
    EXPECT_FALSE(tree.insert(Book("Duplicate", "B", 2000, 900)));
    EXPECT_EQ(tree.size(), 1u);

    const auto stored = tree.find(Book(2000)).lock();
    ASSERT_TRUE(stored);
    EXPECT_EQ(stored->title(), "First");
    EXPECT_EQ(Book::liveCount(), 1u);
}

TEST(BookTree, finds_its_own_full_record_by_key) {
    BookTree tree;
    {
        const Book original("Original", "Author", 2000, 123);
        ASSERT_TRUE(tree.insert(original));
        const auto stored = tree.find(Book(2000)).lock();
        ASSERT_TRUE(stored);
        EXPECT_NE(stored.get(), &original);
        EXPECT_EQ(Book::liveCount(), 2u);
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

TEST(BookTree, height_counts_levels) {
    BookTree tree;
    EXPECT_EQ(tree.height(), 0u);
    tree.insert(Book(1950));
    EXPECT_EQ(tree.height(), 1u);
    tree.insert(Book(1930));
    tree.insert(Book(1980));
    EXPECT_EQ(tree.height(), 2u);
    tree.insert(Book(1960));
    tree.insert(Book(1970));
    EXPECT_EQ(tree.height(), 4u);
}

TEST(BookTree, orders_after_each_insert) {
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

TEST(BookTree, removes_leaf) {
    BookTree tree;
    fillTree(tree);
    ASSERT_TRUE(tree.remove(Book(1961)));
    EXPECT_EQ(tree.size(), 5u);
    expectYears(tree.inOrder(), {1951, 1965, 1974, 1984, 1992});
}

TEST(BookTree, removes_node_with_right_child) {
    BookTree tree;
    fillTree(tree);
    ASSERT_TRUE(tree.remove(Book(1951)));
    EXPECT_EQ(tree.size(), 5u);
    expectYears(tree.inOrder(), {1961, 1965, 1974, 1984, 1992});
}

TEST(BookTree, removes_node_with_left_child) {
    BookTree tree;
    tree.insert(Book(2000));
    tree.insert(Book(1990));
    tree.insert(Book(1980));
    ASSERT_TRUE(tree.remove(Book(1990)));
    EXPECT_EQ(tree.size(), 2u);
    expectYears(tree.inOrder(), {1980, 2000});
}

TEST(BookTree, removes_root_with_two_children) {
    BookTree tree;
    fillTree(tree);
    tree.insert(Book(1978)); // Правый ребёнок преемника 1974.
    const auto removed = tree.find(Book(1965));
    const auto successor = tree.find(Book(1974));

    ASSERT_TRUE(tree.remove(Book(1965)));
    EXPECT_EQ(tree.size(), 6u);
    expectYears(tree.inOrder(), {1951, 1961, 1974, 1978, 1984, 1992});
    EXPECT_TRUE(removed.expired());
    const auto book = successor.lock();
    ASSERT_TRUE(book);
    EXPECT_EQ(tree.find(Book(1974)).lock(), book);
}

TEST(BookTree, removes_node_when_successor_is_right_child) {
    BookTree tree;
    fillTree(tree);
    const auto successor = tree.find(Book(1992));
    ASSERT_TRUE(tree.remove(Book(1984)));
    EXPECT_EQ(tree.size(), 5u);
    expectYears(tree.inOrder(), {1951, 1961, 1965, 1974, 1992});
    EXPECT_FALSE(successor.expired());
}

TEST(BookTree, removes_root_with_one_child_then_last_node) {
    BookTree tree;
    tree.insert(Book(2000));
    tree.insert(Book(2001));
    ASSERT_TRUE(tree.remove(Book(2000)));
    EXPECT_EQ(tree.size(), 1u);
    expectYears(tree.inOrder(), {2001});

    ASSERT_TRUE(tree.remove(Book(2001)));
    EXPECT_EQ(tree.size(), 0u);
    EXPECT_EQ(tree.height(), 0u);
    EXPECT_EQ(Book::liveCount(), 0u);
}

TEST(BookTree, remove_missing_keeps_size_and_order) {
    BookTree tree;
    EXPECT_FALSE(tree.remove(Book(2000)));
    fillTree(tree);
    EXPECT_FALSE(tree.remove(Book(2000)));
    EXPECT_EQ(tree.size(), 6u);
    expectYears(tree.inOrder(), {1951, 1961, 1965, 1974, 1984, 1992});
}

TEST(BookTree, move_constructor_empties_source) {
    BookTree source;
    fillTree(source);
    const auto reference = source.find(Book(1965));
    BookTree target(std::move(source));

    EXPECT_EQ(source.size(), 0u);
    EXPECT_EQ(source.height(), 0u);
    EXPECT_FALSE(reference.expired());
    EXPECT_EQ(target.size(), 6u);
    expectYears(target.inOrder(), {1951, 1961, 1965, 1974, 1984, 1992});
    EXPECT_TRUE(source.insert(Book(2000)));
}

TEST(BookTree, move_assignment_releases_old_tree) {
    BookTree source;
    fillTree(source);
    BookTree target;
    target.insert(Book(2000));
    const auto old = target.find(Book(2000));
    const auto reference = source.find(Book(1965));

    target = std::move(source);
    EXPECT_TRUE(old.expired());
    EXPECT_FALSE(reference.expired());
    EXPECT_EQ(source.size(), 0u);
    EXPECT_EQ(source.height(), 0u);

    BookTree& same = target;
    target = std::move(same);
    EXPECT_EQ(target.size(), 6u);
    expectYears(target.inOrder(), {1951, 1961, 1965, 1974, 1984, 1992});
}

TEST(Criterion, checks_year_bounds_and_page_threshold) {
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

TEST(Criterion, different_filters_on_same_tree) {
    BookTree tree;
    fillTree(tree);

    expectYears(tree.select(AllBooks()), {1951, 1961, 1965, 1974, 1984, 1992});
    const Selection selected = tree.select(BookSelector(1960, 1990, 250));
    ASSERT_NO_FATAL_FAILURE(expectYears(selected, {1965, 1974, 1984}));
    EXPECT_EQ(selected[0].lock()->title(), "Dune");
    EXPECT_EQ(selected[1].lock()->author(), "LeGuin");
    EXPECT_EQ(selected[2].lock()->pages(), 271);
}

TEST(Criterion, empty_tree_and_no_matches) {
    BookTree tree;
    EXPECT_TRUE(tree.select(AllBooks()).empty());
    EXPECT_TRUE(tree.inOrder().empty());
    EXPECT_TRUE(tree.reverseOrder().empty());

    fillTree(tree);
    const Selection selected = tree.select(BookSelector(2000, 2010, 0));
    EXPECT_TRUE(selected.empty());
    EXPECT_EQ(selected.capacity(), 0u);
}

TEST(Criterion, selection_keeps_tree_unchanged) {
    BookTree tree;
    fillTree(tree);
    const BookTree& readOnly = tree;
    const auto size = readOnly.size();
    const auto height = readOnly.height();
    const Selection before = readOnly.inOrder();

    const Selection selected = readOnly.select(AllBooks());

    EXPECT_EQ(readOnly.size(), size);
    EXPECT_EQ(readOnly.height(), height);
    EXPECT_EQ(Book::liveCount(), size);
    const Selection after = readOnly.inOrder();
    ASSERT_EQ(after.size(), before.size());
    ASSERT_EQ(selected.size(), before.size());
    for (std::size_t i = 0; i < before.size(); ++i) {
        const auto book = before[i].lock();
        ASSERT_TRUE(book);
        EXPECT_EQ(after[i].lock(), book);
        EXPECT_EQ(selected[i].lock(), book);
    }
}

TEST(Selection, grows_by_doubling_and_checks_indices) {
    Selection selected;
    EXPECT_TRUE(selected.empty());
    EXPECT_EQ(selected.capacity(), 0u);
    EXPECT_THROW(selected[0], std::out_of_range);

    const std::size_t capacities[] = {1, 2, 4, 4, 8, 8, 8, 8, 16};
    std::size_t i = 0;
    for (std::size_t capacity : capacities) {
        selected.append({});
        EXPECT_EQ(selected.capacity(), capacity);
        EXPECT_EQ(selected.size(), i + 1);
        EXPECT_TRUE(selected[i].expired());
        ++i;
    }
    EXPECT_THROW(selected[i], std::out_of_range);
}

TEST(Selection, copy_has_own_array_and_both_refs_expire) {
    BookTree tree;
    fillTree(tree);
    const Selection original = tree.select(BookSelector(1960, 1990, 250));
    Selection copy(original);
    ASSERT_EQ(copy.size(), 3u);
    EXPECT_NE(&original[0], &copy[0]);                 // Два элемента массива.
    EXPECT_EQ(original[0].lock(), copy[0].lock());    // Одна и та же книга.
    copy.append({});
    EXPECT_EQ(copy.size(), 4u);
    EXPECT_EQ(original.size(), 3u);
    EXPECT_EQ(Book::liveCount(), 6u);

    ASSERT_TRUE(tree.remove(Book(1965)));
    EXPECT_EQ(original.size(), 3u);
    EXPECT_EQ(copy.size(), 4u);
    EXPECT_TRUE(original[0].expired());
    EXPECT_TRUE(copy[0].expired());
    EXPECT_FALSE(copy[1].expired());
}

TEST(Selection, copy_assignment_replaces_array) {
    BookTree tree;
    fillTree(tree);
    Selection original = tree.select(BookSelector(1960, 1990, 250));
    Selection copy = tree.inOrder();
    copy = original;
    EXPECT_NE(&copy[0], &original[0]);

    Selection& same = copy;
    copy = same;
    original = Selection();
    expectYears(copy, {1965, 1974, 1984});
}

TEST(Selection, move_constructor_transfers_array) {
    BookTree tree;
    fillTree(tree);
    Selection source = tree.select(BookSelector(1960, 1990, 250));
    const auto* address = &source[0];
    const Selection target(std::move(source));

    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.capacity(), 0u);
    EXPECT_EQ(&target[0], address);
    expectYears(target, {1965, 1974, 1984});
    source.append({});
    EXPECT_EQ(source.size(), 1u);
}

TEST(Selection, move_assignment_replaces_array) {
    BookTree tree;
    fillTree(tree);
    Selection source = tree.select(BookSelector(1960, 1990, 250));
    const auto* address = &source[0];
    Selection target = tree.inOrder();

    target = std::move(source);
    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.capacity(), 0u);
    EXPECT_EQ(&target[0], address);
    Selection& same = target;
    target = std::move(same);
    expectYears(target, {1965, 1974, 1984});
}

TEST(Selection, prints_record_or_expired_marker) {
    BookTree tree;
    tree.insert(Book("Title", "Author", 2000, 250));
    const Selection selected = tree.inOrder();
    std::ostringstream before;
    before << selected;
    EXPECT_EQ(before.str(), "1\nTitle Author 2000 250\n");

    ASSERT_TRUE(tree.remove(Book(2000)));
    std::ostringstream after;
    after << selected;
    EXPECT_EQ(after.str(), "1\n[expired]\n");

    std::ostringstream empty;
    empty << Selection();
    EXPECT_EQ(empty.str(), "0\n");
}

TEST(Lifetime, removal_and_destruction_release_records) {
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

TEST(Lifetime, lock_keeps_book_alive_until_released) {
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

} // namespace
