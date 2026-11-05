# Вариант 5 (static и friend)

Реализуйте с полем static и методом friendly класс `Counter`:

- Поле `static int total_;`
- Поле `int id_;`
- Конструктор при создании увеличивает `total_` и записывает `id_`
- Метод `int id() const;`
- Метод `static int total();`
- `friend void print(const Counter& c);` выводит его `id` и `total`


В `main()` создать 3 объекта класса и вывести их `total_` и `id_`

