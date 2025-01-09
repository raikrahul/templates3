#include <iostream>
#include <type_traits>

// Primary template for a tuple with at least one type.
// `tuple` is a recursive structure designed to store a heterogeneous collection of elements.
template <typename... Types>
struct tuple {
    // This primary template is intentionally left empty because the real implementation
    // depends on the specialization for non-empty parameter packs below.
};

// Specialized template for a non-empty tuple
// Each instantiation splits the first element (T) from the rest (Rest...),
// recursively nesting the `tuple` structure.
template <typename T, typename... Rest>
struct tuple<T, Rest...> {
    tuple(T const& t, Rest const&... rest) 
        : value(t), rest(rest...) {} 
    // `value`: The first element of the tuple.
    // `rest`: A recursively instantiated tuple containing the remaining elements.

    T value;                  // (1) Current value - e.g., for tuple<int, double>, value could be 42.
    tuple<Rest...> rest;      // (2) Remaining elements - recursively reduced, e.g., tuple<double>.
};

// Base case specialization for an empty tuple
template <>
struct tuple<> {
    // No members are required for an empty tuple.
    // Acts as the termination point for recursion.
};

// nth_type struct: Retrieves the Nth type from a parameter pack.
template <size_t N, typename T, typename... Ts>
struct nth_type : nth_type<N - 1, Ts...> {
    static_assert(N < sizeof...(Ts) + 1, "Index out of bounds");
    // `sizeof...(Ts) + 1` counts the first type `T` and the remaining types `Ts...`.
    // If N >= this size, the static assertion fails at compile time.
};

template <typename T, typename... Ts>
struct nth_type<0, T, Ts...> {
    using value_type = T; 
    // When N=0, recursion ends, and `value_type` resolves to the first type `T`.
    // Example: nth_type<0, int, double, char>::value_type -> int.
};

// Getter struct: Retrieves the Nth element from a tuple by leveraging `nth_type`.
template <size_t N>
struct getter {
    template <typename... Ts>
    static typename nth_type<N, Ts...>::value_type& get(tuple<Ts...>& t) {
        // This recursive function calls itself with the nested `rest` of the tuple,
        // decrementing `N` until it reaches 0.
        return getter<N - 1>::get(t.rest);
    }
};

// Base case for the getter: When N=0, it retrieves the `value` of the tuple.
template <>
struct getter<0> {
    template <typename T, typename... Ts>
    static T& get(tuple<T, Ts...>& t) {
        return t.value;
    }
};

// Main function demonstrating the tuple and getter usage.
int main() {
    // Create a tuple with elements (42, 3.14, 'A').
    tuple<int, double, char> t(42, 3.14, 'A');
    // Tuple structure:
    // t.value = 42 (int)
    // t.rest.value = 3.14 (double)
    // t.rest.rest.value = 'A' (char)
    // t.rest.rest.rest = empty tuple

    // Test retrieving the 0th element.
    int& elem0 = getter<0>::get(t); 
    // Execution:
    // getter<0>::get(t) -> t.value
    // elem0 = 42
    std::cout << "Element 0: " << elem0 << std::endl; // Should output 42.

    // Test retrieving the 1st element.
    double& elem1 = getter<1>::get(t);
    // Execution:
    // getter<1>::get(t) -> getter<0>::get(t.rest) -> t.rest.value
    // elem1 = 3.14
    std::cout << "Element 1: " << elem1 << std::endl; // Should output 3.14.

    // Test retrieving the 2nd element.
    char& elem2 = getter<2>::get(t);
    // Execution:
    // getter<2>::get(t) -> getter<1>::get(t.rest) -> getter<0>::get(t.rest.rest) -> t.rest.rest.value
    // elem2 = 'A'
    std::cout << "Element 2: " << elem2 << std::endl; // Should output 'A'.

    // Uncommenting the following line causes a compile-time error due to index out of bounds:
    // char& elem3 = getter<3>::get(t); 

    // Additional test with a single-element tuple.
    tuple<std::string> singleElemTuple("Hello");
    std::string& singleElem = getter<0>::get(singleElemTuple);
    // Execution:
    // getter<0>::get(singleElemTuple) -> singleElemTuple.value
    // singleElem = "Hello"
    std::cout << "Single Element: " << singleElem << std::endl; // Should output "Hello".

    // Uncommenting the following lines will cause a compile-time error due to invalid access:
    // tuple<> emptyTuple;
    // int& invalidElem = getter<0>::get(emptyTuple); 

    return 0;
}
