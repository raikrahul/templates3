#include <iostream>
#include <string>
#include <type_traits>

// Define a template struct `foo` that can hold a primary type `T` and a variadic pack `Args...`
template <typename T, typename... Args>
struct foo
{
    // TO-DO: Add a member variable to store the primary value of type `T`
    // Hint: This will be the "main" attribute of the inventory item (e.g., name).

    // TO-DO: Add a constructor that initializes the primary value

    // TO-DO: Add a member variable to store the variadic pack `Args...`
    // Hint: This will represent additional attributes (e.g., price, quantity, etc.).

    // TO-DO: Implement a constructor that initializes the primary value and the variadic pack
    // Hint: Use parameter packs to handle the variable number of arguments.

    // TO-DO: Implement a method `print()` that prints the primary value and all additional attributes
    // Hint: Use recursion or fold expressions to iterate over the variadic pack.

    T primary_value;
    std::tuple<Args...> additional_values;

    foo(T primary, Args... additional) : primary_value(primary), additional_values(additional...) {}

    void print()
    {
        std::cout << primary_value;
        ((std::cout << ", " << std::get<Args>(additional_values)), ...);
        std::cout << '\n';
    }
};

// Test cases
int main()
{
    // Test case 1: Inventory item with name (string) and price (double)
    foo<std::string, double> item1("Widget", 19.99);
    item1.print(); // Expected output: "Widget, 19.99"

    // Test case 2: Inventory item with name (string), price (double), and quantity (int)
    foo<std::string, double, int> item2("Gadget", 29.99, 100);
    item2.print(); // Expected output: "Gadget, 29.99, 100"

    // Test case 3: Inventory item with name (string), price (double), quantity (int), and category (string)
    foo<std::string, double, int, std::string> item3("Tool", 9.99, 50, "Hardware");
    item3.print(); // Expected output: "Tool, 9.99, 50, Hardware"

    // Static assertions to ensure the template works as expected
    static_assert(std::is_same_v<decltype(item1), foo<std::string, double>>, "Test case 1 type mismatch!");
    static_assert(std::is_same_v<decltype(item2), foo<std::string, double, int>>, "Test case 2 type mismatch!");
    static_assert(std::is_same_v<decltype(item3), foo<std::string, double, int, std::string>>, "Test case 3 type mismatch!");

    return 0;
}