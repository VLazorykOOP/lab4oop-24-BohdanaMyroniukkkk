#include <iostream>
#include <map>
#include <string>

class VectorULong {
private:
    unsigned long* data; // Вказівник на масив даних
    size_t size; // Розмір вектора
    int State; // Код стану: 0 - норма, 1 - помилка пам'яті, 2 - виходить за межі масиву

public:
    // Конструктори класу
    VectorULong() : size(1), State(0) {
        data = new unsigned long[size];
        data[0] = 0;
    }

    VectorULong(size_t s) : size(s), State(0) {
        data = new unsigned long[size];
        for (size_t i = 0; i < size; ++i)
            data[i] = 0;
    }

    VectorULong(size_t s, unsigned long value) : size(s), State(0) {
        data = new unsigned long[size];
        for (size_t i = 0; i < size; ++i)
            data[i] = value;
    }

    // Конструктор копіювання
    VectorULong(const VectorULong& other) : size(other.size), State(other.State) {
        data = new unsigned long[size];
        for (size_t i = 0; i < size; ++i)
            data[i] = other.data[i];
    }

    ~VectorULong() {
        delete[] data;
    }

    // Перевантаження присвоєння =
    VectorULong& operator=(const VectorULong& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            State = other.State;
            data = new unsigned long[size];
            for (size_t i = 0; i < size; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }

    // Перевантаження унарного префіксного ++
    VectorULong& operator++() {
        for (size_t i = 0; i < size; ++i)
            ++data[i];
        return *this;
    }

    // Перевантаження унарного постфіксного ++
    VectorULong operator++(int) {
        VectorULong temp(*this);
        ++(*this);
        return temp;
    }

    // Перевантаження бінарного +
    VectorULong operator+(const VectorULong& other) const {
        if (size != other.size) {
            // Обробка помилки
            VectorULong result; // Повернути порожній вектор
            result.State = 2; // Встановити стан помилки
            return result;
        }
        VectorULong result(size);
        for (size_t i = 0; i < size; ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    // Перевантаження оператора виведення <<
    friend std::ostream& operator<<(std::ostream& out, const VectorULong& vec) {
        out << "[ ";
        for (size_t i = 0; i < vec.size; ++i)
            out << vec.data[i] << " ";
        out << "]";
        return out;
    }
};

class Library {
private:
    std::map<int, std::string> books; // Асоціативний контейнер для збереження інвентарних номерів та назв книг
    int CodeError; // Код помилки, якщо сутність не знайдена

public:
    // Конструктор класу
    Library() : CodeError(0) {}

    // Додавання нової книги до бібліотеки
    void addBook(int inventoryNumber, const std::string& title) {
        books[inventoryNumber] = title;
    }

    // Операція індексації []
    std::string operator[](int inventoryNumber) {
        auto it = books.find(inventoryNumber);
        if (it != books.end()) {
            CodeError = 0; // Сутність знайдена, код помилки скидається
            return it->second; // Повертаємо назву книги
        }
        else {
            CodeError = 1; // Сутність не знайдена, встановлюємо код помилки
            return ""; // Повертаємо порожню рядок
        }
    }

    // Функція для отримання коду помилки
    int getErrorCode() const {
        return CodeError;
    }

    // Перевантаження оператора виклику функції ()
    std::string operator()(int inventoryNumber) {
        return (*this)[inventoryNumber];
    }

    // Дружня функція для введення даних
    friend std::istream& operator>>(std::istream& in, Library& lib) {
        int inventoryNumber;
        std::string title;
        std::cout << "Enter inventory number: ";
        in >> inventoryNumber;
        std::cout << "Enter title: ";
        in.ignore(); // Пропускаємо символ нового рядка, залишений після попереднього введення
        std::getline(in, title);
        lib.addBook(inventoryNumber, title);
        return in;
    }

    // Дружня функція для виведення даних
    friend std::ostream& operator<<(std::ostream& out, const Library& lib) {
        out << "Inventory Number\tTitle\n";
        for (const auto& book : lib.books) {
            out << book.first << "\t\t\t" << book.second << std::endl;
        }
        return out;
    }
};

// Функція для вибору класу та дії в меню
void menu() {
    std::cout << "Menu:\n";
    std::cout << "1. VectorULong Operations\n";
    std::cout << "2. Library Operations\n";
    std::cout << "3. Exit\n";
}

int main() {
    int choice;
    menu(); // Виводимо меню
    std::cout << "Enter your choice: ";
    std::cin >> choice; // Отримуємо вибір користувача

    switch (choice) { // Обробка вибору користувача
    case 1: {
        // Виконати операції з класом VectorULong
        VectorULong vec1(5, 3);
        VectorULong vec2(5, 2);
        VectorULong result = vec1 + vec2;
        std::cout << "Result: " << result << std::endl;
        VectorULong result2 = vec1++;
        std::cout << "Result: " << result2 << std::endl;
        VectorULong result3 = vec1 = vec2;
        std::cout << "Result: " << result3 << std::endl;
        break;
    }
    case 2: {
        // Виконати операції з класом Library
        Library library;
        library.addBook(12345, "Introduction to C++");
        library.addBook(54321, "Data Structures and Algorithms");
        std::cout << "Library contents:\n" << library << std::endl;
        std::cout << "Title of book with inventory number 12345: " << library[12345] << std::endl;
        std::cout << "Title of book with inventory number 54321: " << library(54321) << std::endl;
        break;
    }
    case 3:
        std::cout << "Exiting...\n"; // Вихід з програми
        break;
    default:
        std::cout << "Invalid choice\n"; // Невідомий вибір
    }

    return 0;
}
