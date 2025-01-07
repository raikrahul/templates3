#include <iostream>
#include <array>
#include <string>
#include <type_traits>

// namespace n308
// {
//    template<typename... Ts>
//    constexpr auto get_type_sizes()
//    {
//       return std::array<std::size_t, sizeof...(Ts)>{sizeof(Ts)...};
//    }
// } // namespace n308

// namespace punch_card_era {

// // In the 1950s, data storage was often on punch cards.
// // Let's imagine different data types take up a certain number of "columns" on a card.

// // Representation of a worker's basic information on a punch card.
// struct WorkerId {
//     char departmentCode; // 1 column
//     int employeeNumber;   // Let's assume 4 columns for simplicity
// };

// // Representation of financial data on a punch card.
// struct FinancialRecord {
//     double salary;       // Assuming 8 columns for high precision
//     short year;          // Assuming 2 columns
// };

// // Function to test the get_type_sizes function for punch card column calculations.
// void testPunchCardColumnSizes() {
//     std::cout << "--- Testing Punch Card Column Sizes ---" << std::endl;

//     // Test case 1: Single data type - Employee Department Code
//     constexpr auto sizes1 = n308::get_type_sizes<char>();
//     static_assert(sizes1.size() == 1, "Test Case 1 Failed: Incorrect number of sizes");
//     // TODO: The actual size of char in columns
//     static_assert(sizes1[0] == /* FILL IN THE HARDEST PART HERE */, "Test Case 1 Failed: Incorrect char size");
//     std::cout << "Test Case 1 Passed" << std::endl;

//     // Test case 2: Multiple data types of the same size (hypothetically)
//     constexpr auto sizes2 = n308::get_type_sizes<int, int, int>();
//     static_assert(sizes2.size() == 3, "Test Case 2 Failed: Incorrect number of sizes");
//     // TODO: The actual size of int in columns
//     static_assert(sizes2[0] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes2[1] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes2[2] == /* FILL IN THE HARDEST PART HERE */, "Test Case 2 Failed: Incorrect int sizes");
//     std::cout << "Test Case 2 Passed" << std::endl;

//     // Test case 3: Multiple data types of different sizes - Worker ID
//     constexpr auto sizes3 = n308::get_type_sizes<WorkerId>();
//     static_assert(sizes3.size() == 1, "Test Case 3 Failed: Incorrect number of sizes");
//     // TODO: The actual size of WorkerId in columns (sum of its members)
//     static_assert(sizes3[0] == /* FILL IN THE HARDEST PART HERE */, "Test Case 3 Failed: Incorrect WorkerId size");
//     std::cout << "Test Case 3 Passed" << std::endl;

//     // Test case 4: Mixed fundamental types - Financial Data Year and Salary
//     constexpr auto sizes4 = n308::get_type_sizes<short, double>();
//     static_assert(sizes4.size() == 2, "Test Case 4 Failed: Incorrect number of sizes");
//     // TODO: The actual sizes of short and double in columns
//     static_assert(sizes4[0] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes4[1] == /* FILL IN THE HARDEST PART HERE */, "Test Case 4 Failed: Incorrect short and double sizes");
//     std::cout << "Test Case 4 Passed" << std::endl;

//     // Test case 5: Multiple members of the WorkerId struct individually
//     constexpr auto sizes5 = n308::get_type_sizes<char, int>();
//     static_assert(sizes5.size() == 2, "Test Case 5 Failed: Incorrect number of sizes");
//     // TODO: The actual sizes of char and int
//     static_assert(sizes5[0] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes5[1] == /* FILL IN THE HARDEST PART HERE */, "Test Case 5 Failed: Incorrect char and int sizes");
//     std::cout << "Test Case 5 Passed" << std::endl;

//     // Test case 6: An empty parameter pack. What should the size be?
//     constexpr auto sizes6 = n308::get_type_sizes<>();
//     static_assert(sizes6.size() == 0, "Test Case 6 Failed: Incorrect number of sizes for empty pack");
//     std::cout << "Test Case 6 Passed" << std::endl;

//     // Test case 7: Combining WorkerId and FinancialRecord
//     constexpr auto sizes7 = n308::get_type_sizes<WorkerId, FinancialRecord>();
//     static_assert(sizes7.size() == 2, "Test Case 7 Failed: Incorrect number of sizes");
//     // TODO: The actual sizes of WorkerId and FinancialRecord
//     static_assert(sizes7[0] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes7[1] == /* FILL IN THE HARDEST PART HERE */, "Test Case 7 Failed: Incorrect struct sizes");
//     std::cout << "Test Case 7 Passed" << std::endl;

//     // Test case 8:  Multiple instances of different structs
//     constexpr auto sizes8 = n308::get_type_sizes<WorkerId, WorkerId, FinancialRecord>();
//     static_assert(sizes8.size() == 3, "Test Case 8 Failed: Incorrect number of sizes");
//     // TODO: The actual sizes of the structs
//     static_assert(sizes8[0] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes8[1] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes8[2] == /* FILL IN THE HARDEST PART HERE */, "Test Case 8 Failed: Incorrect struct sizes");
//     std::cout << "Test Case 8 Passed" << std::endl;

//     // Test case 9: Fundamental types of varying sizes
//     constexpr auto sizes9 = n308::get_type_sizes<bool, char, short, int, long, long long, float, double, long double>();
//     static_assert(sizes9.size() == 9, "Test Case 9 Failed: Incorrect number of sizes");
//     // TODO: The actual sizes of fundamental types
//     static_assert(sizes9[0] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes9[1] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes9[2] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes9[3] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes9[4] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes9[5] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes9[6] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes9[7] == /* FILL IN THE HARDEST PART HERE */ &&
//                   sizes9[8] == /* FILL IN THE HARDEST PART HERE */, "Test Case 9 Failed: Incorrect fundamental type sizes");
//     std::cout << "Test Case 9 Passed" << std::endl;

//     std::cout << "Punch Card Column Size Tests Completed" << std::endl;
// }

// } // namespace punch_card_era

int main() {
    // punch_card_era::testPunchCardColumnSizes();
    return 0;
}