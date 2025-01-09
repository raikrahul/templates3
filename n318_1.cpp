#include <iostream>

// In outer<>, we use typename... because we're declaring types
template <typename... T>
struct outer {
    // In inner<>, we don't use typename because we're declaring values of types T
    // T... args means "values of types T"
    // If we used typename... here, it would expect types rather than values
    template <T... args>
    struct inner {
        static void print() {
            std::cout << "Inner template instantiated with parameters: ";
            // Fix: Use a fold expression over comma operator
            int unused[] = { (std::cout << args << ' ', 0)... };
            (void)unused; // Suppress unused variable warning
            std::cout << std::endl;
        }
    };
};

int main() {
    // Example: T is the type (int), args are values (1, 2, 3)
    outer<int, int, int>::inner<1, 2, 3> obj;
    obj.print();

    outer<float, bool>::inner<4.0f, true> another_obj;
    another_obj.print();

    return 0;
}

