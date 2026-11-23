#include "book_tree.h"
#include <iostream>

int main() {
    BookTree tree;
    tree.insert(Book("Dune", "Herbert", 1965, 412));
    tree.insert(Book("Foundation", "Asimov", 1951, 255));
    tree.insert(Book("Neuromancer", "Gibson", 1984, 271));
    tree.insert(Book("Solaris", "Lem", 1961, 204));
    tree.insert(Book("TheDispossessed", "LeGuin", 1974, 341));
    tree.insert(Book("SnowCrash", "Stephenson", 1992, 480));

    std::cout << "In-order traversal:\n" << tree.inOrder();
    const BookSelector criterion(1960, 1990, 250);
    const Selection selected = tree.select(criterion);
    std::cout << "Selection: year in [1960, 1990), pages >= 250:\n" << selected;

    tree.remove(Book(1965)); // Корень с двумя детьми; временный эталон — только год.
    std::cout << "Same selection after removing 1965:\n" << selected;
    std::cout << "Tree after removal:\n" << tree.inOrder();
}
