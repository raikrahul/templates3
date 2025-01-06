#include <iostream>
#include <cstddef>
#include <climits>

namespace n304 {
    struct ProductOrder {
        double pricePerUnit;
        int quantity;
    };

    struct Adder {
        template <typename T1, typename T2>
        constexpr T1 operator()(const T1 &current, const T2 &order) const {
            return current + order.quantity;
        }
    };

    // Primary template (variadic version)
    template <typename ResultType, typename Accumulator, typename OrderType, typename... Args>
    constexpr ResultType calculateCumulative(const OrderType& first, const Args&... rest);

    // Base case for the recursion (1 argument)
    template <typename ResultType, typename Accumulator, typename OrderType>
    constexpr ResultType calculateCumulative(const OrderType& order) {
        Accumulator acc;
        return acc(ResultType{0}, order);
    }

    // Specialization for 2 arguments
    template <typename ResultType, typename Accumulator, typename OrderType>
    constexpr ResultType calculateCumulative(const OrderType& order1, const OrderType& order2) {
        Accumulator acc;
        return acc(acc(ResultType{0}, order1), order2);
    }

    // Specialization for 3 arguments
    template <typename ResultType, typename Accumulator, typename OrderType>
    constexpr ResultType calculateCumulative(const OrderType& order1, const OrderType& order2, 
                                           const OrderType& order3) {
        Accumulator acc;
        return acc(acc(acc(ResultType{0}, order1), order2), order3);
    }

    // Specialization for 4 arguments
    template <typename ResultType, typename Accumulator, typename OrderType>
    constexpr ResultType calculateCumulative(const OrderType& order1, const OrderType& order2,
                                           const OrderType& order3, const OrderType& order4) {
        Accumulator acc;
        return acc(acc(acc(acc(ResultType{0}, order1), order2), order3), order4);
    }

    // Implementation of variadic template (for more than 4 arguments)
    template <typename ResultType, typename Accumulator, typename OrderType, typename... Args>
    constexpr ResultType calculateCumulative(const OrderType& first, const Args&... rest) {
        Accumulator acc;
        return acc(acc(ResultType{0}, first), calculateCumulative<ResultType, Accumulator, OrderType>(rest...));
    }

    template<typename... Args>
struct PackSize {
    static constexpr size_t value = sizeof...(Args);
};

    // Test cases
    static_assert(calculateCumulative<int, Adder, ProductOrder>(
        ProductOrder{10.0, 5}) == 5, "Test Case 1 Failed");
    
    static_assert(calculateCumulative<int, Adder, ProductOrder>(
        ProductOrder{10.0, 5}, ProductOrder{5.0, 3}) == 8, "Test Case 2 Failed");
    
    static_assert(calculateCumulative<int, Adder, ProductOrder>(
        ProductOrder{10.0, 5}, ProductOrder{5.0, 3}, ProductOrder{2.0, 7}) == 15, 
        "Test Case 3 Failed");
    
    static_assert(calculateCumulative<int, Adder, ProductOrder>(
        ProductOrder{10.0, 5}, ProductOrder{5.0, 3}, ProductOrder{2.0, 7}, 
        ProductOrder{8.0, 2}) == 17, "Test Case 4 Failed");
}

int main() {
    std::cout << "All static asserts passed." << std::endl;
    return 0;
}