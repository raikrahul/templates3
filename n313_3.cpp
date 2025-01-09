#include <type_traits>

// nth_type: A compile-time utility to retrieve the Nth type from a parameter pack.
template <size_t N, typename T, typename... Ts> // (1)
// Explanation:
// `N`: Index of the type to retrieve from the parameter pack. For example, N = 0 means the first type.
// `T`: The first type in the parameter pack.
// `Ts...`: The remaining types in the parameter pack.
// Example: For nth_type<2, int, float, double>, N=2, T=int, Ts...={float, double}.

struct nth_type : nth_type<N - 1, Ts...> { // (2)
// Explanation:
// Recursive definition. For each instantiation, the parameter pack is reduced by one element, 
// decrementing `N` until N=0 is reached, at which point the base case is triggered.
// Example: nth_type<2, int, float, double> reduces as follows:
// 1. nth_type<2, int, float, double> -> nth_type<1, float, double>
// 2. nth_type<1, float, double> -> nth_type<0, double>
// 3. nth_type<0, double>: Base case reached.
// Each reduction removes the first element of the pack and decrements N.
    static_assert(N < sizeof...(Ts) + 1, "Index out of bounds"); // (3)
// Explanation:
// Ensures `N` is within the bounds of the parameter pack size. `sizeof...(Ts)` gives the count of Ts....
// +1 accounts for the presence of T (the first type). If N >= sizeof...(Ts) + 1, this assertion fails.
};

// Base case for nth_type when N = 0 (4)
// This specialization stops recursion and defines `value_type` as the first type, T.
template <typename T, typename... Ts>
struct nth_type<0, T, Ts...> { 
    using value_type = T; // (5)
// Explanation:
// For nth_type<0, int, float, double>, `value_type` resolves directly to T, which is `int` in this case.
};

// Main function: Testing nth_type with static_assert to ensure compile-time correctness.
int main() {
    // Test case 1: Verify that the 0th type in <int, float, double> is `int`.
    static_assert(std::is_same<nth_type<0, int, float, double>::value_type, int>::value, "Test case 1 failed");
    // Breakdown:
    // nth_type<0, int, float, double>::value_type resolves directly to `int`.
    // `std::is_same` checks whether the resolved type matches `int`.

    // Test case 2: Verify that the 1st type in <int, float, double> is `float`.
    static_assert(std::is_same<nth_type<1, int, float, double>::value_type, float>::value, "Test case 2 failed");
    // Breakdown:
    // nth_type<1, int, float, double> -> nth_type<0, float, double>
    // nth_type<0, float, double>::value_type -> `float`.
    // `std::is_same` ensures correctness.

    // Test case 3: Verify that the 2nd type in <int, float, double> is `double`.
    static_assert(std::is_same<nth_type<2, int, float, double>::value_type, double>::value, "Test case 3 failed");
    // Breakdown:
    // nth_type<2, int, float, double> -> nth_type<1, float, double> -> nth_type<0, double>
    // nth_type<0, double>::value_type -> `double`.

    // Test case 4: Verify that the 0th type in <char> is `char`.
    static_assert(std::is_same<nth_type<0, char>::value_type, char>::value, "Test case 4 failed");
    // Breakdown:
    // nth_type<0, char>::value_type resolves directly to `char`.

    // Test case 5: Uncommenting this line results in an "Index out of bounds" error.
    // static_assert(std::is_same<nth_type<1, char>::value_type, int>::value, "Test case 5 failed");
    // Breakdown:
    // nth_type<1, char> fails because there's only one type (`char`), making N out of bounds.

    // Test case 6: Uncommenting this line results in a "No valid types" error.
    // static_assert(std::is_same<nth_type<0,>::value_type, int>::value, "Test case 6 failed");
    // Breakdown:
    // nth_type<0,> fails due to an empty parameter pack.

    return 0;
}
