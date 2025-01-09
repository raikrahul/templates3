#include <iostream>
#include <type_traits>
  
// Primary template for tuple with at least one type
template <typename... Types>
struct tuple {
    // Primary template is intentionally empty
};

// Specialized template for non-empty tuple
template <typename T, typename... Rest>
struct tuple<T, Rest...> {
    tuple(T const& t, Rest const&... rest) 
        : value(t), rest(rest...) {
        // Debug print: Show the value being stored in the current tuple level
        std::cout << "Tuple value: " << value << std::endl;
    }

    constexpr int size() const {
        // Debug print: Show the size calculation at the current tuple level
        std::cout << "Calculating size at current level. Current size: " << 1 + rest.size() << std::endl;
        return 1 + rest.size(); // Recursively calculate size
    }

    T value;                  // Current value
    tuple<Rest...> rest;      // Nested tuple for the remaining values
};

// Base case specialization for empty tuple
template <>
struct tuple<> {
    constexpr int size() const {
        // Debug print: Show that we've reached the base case (empty tuple)
        // std::cout << "Base case reached. Size: 0" << std::endl;
        return 0; // Base case: empty tuple has size 0
    }
};

// Function to get the size of a tuple
template <typename... Ts>
constexpr int get_size(const tuple<Ts...>& t) {
    // Debug print: Show that we're calling size() on the tuple
    std::cout << "Calling get_size() on tuple." << std::endl;
    return t.size();
}

int main() {
    // Create a tuple with 3 elements
    std::cout << "Creating tuple t with elements (42, 3.14, 'A')..." << std::endl;
    tuple<int, double, char> t(42, 3.14, 'A');

    // Create a tuple with 1 element
    std::cout << "\nCreating tuple t2 with element (420)..." << std::endl;
    tuple<int> t2(420);

    // Get and print the size of the tuple t
    std::cout << "\nGetting size of tuple t..." << std::endl;
    std::cout << "Tuple size from main: " << get_size(t) << " (Done)" << std::endl;

    return 0;
}