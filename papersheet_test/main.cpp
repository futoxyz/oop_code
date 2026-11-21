/*
Первая КР по ООП. Проводилась на листочках. Реализация всех конструкторов необязательна,
но важно добавить их общий вид и задать их по умолчанию, например:
Counter(Counter&& other) noexcept = default;
В условии просят только увеличивать total_ при инициализации, был другой вариант с аналогичным классом,
где просилось обратное - при вызове деструктора уменьшать total_. Но наверное лучше делать полноценный логичный класс.
Некоторые задания были странными и непонятными, с этим соглашались лабники.
*/

#include <iostream>

class Counter {
private:
    static int total_;
    int id_;

public:
    Counter(int id): id_(id) {
        ++total_;
    }
    
    int id() const { return id_; }
    static int total() { return total_; }
    
    Counter(const Counter& other) {
        id_ = other.id_;
        ++total_;
    }
    
    Counter(Counter&& other) noexcept {
        id_ = other.id_;
        ++total_;
    }
    
    Counter& operator=(const Counter& other) {
        if (this != &other) {
            id_ = other.id_;
        }
        return *this;
    }
    
    Counter& operator=(Counter&& other) noexcept {
        if (this != &other) {
            id_ = other.id_;
        }
        return *this;
    }
    
    virtual ~Counter() noexcept {
        --total_;
    }
    
    friend void print(const Counter& c) {
        std::cout << "Id " << c.id_ << "\n"
                  << "Total " << c.total() << std::endl;
    }
};

int Counter::total_ = 0;

int main() {
    Counter a = Counter(3);
    print(a);
    Counter b = Counter(5);
    print(b);
    Counter c = Counter(10);
    print(c);
}
