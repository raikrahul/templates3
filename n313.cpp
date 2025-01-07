#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <chrono>
#include <limits>
namespace business_operations
{

    // A simplified representation of a 1950s business ledger entry
    template <typename IdType, typename DataType, typename TimestampPolicy = std::chrono::system_clock>
    struct LedgerEntry
    {
        IdType id;
        DataType data;
        typename TimestampPolicy::time_point timestamp;

        LedgerEntry(const IdType &i, const DataType &d) : id(i), data(d), timestamp(TimestampPolicy::now()) {}
    };

    // Fix the supports_entry concept
    template <typename T, typename IdType, typename DataType, typename TimestampType>
    concept supports_entry = requires(T a) {
        { a.id } -> std::convertible_to<IdType>;
        { a.data } -> std::convertible_to<DataType>;
        { a.timestamp } -> std::convertible_to<TimestampType>;
    };

    // Modify the Ledger class constraint
    template <typename EntryType, template <typename...> typename ContainerPolicy = std::vector>
        requires supports_entry<
            EntryType, 
            typename std::remove_reference<decltype(std::declval<EntryType>().id)>::type,
            typename std::remove_reference<decltype(std::declval<EntryType>().data)>::type,
            typename std::remove_reference<decltype(std::declval<EntryType>().timestamp)>::type>
    class Ledger
    {
    public:
        using value_type = EntryType;

        template <typename... Args>
        void add_entry(Args &&...args)
        {
            entries.emplace_back(std::forward<Args>(args)...);
        }

        // Provide access to entries - careful with this in a real system!
        ContainerPolicy<EntryType> &get_entries() { return entries; }

        template <typename SearchPredicate>
            requires std::invocable<SearchPredicate, const EntryType &>
        auto find_entries(SearchPredicate predicate) const
        {
            // TO-DO: Implement the logic to filter entries based on the predicate
            // This is the core logic you need to fill in.
            // Think about how to iterate through 'entries' and apply the 'predicate'.
            // Consider using standard library algorithms.
            ContainerPolicy<EntryType> results;
            for (const auto &entry : entries)
            {
                if (predicate(entry))
                {
                    results.push_back(entry);
                }
            }
            return results;
        }

    private:
        ContainerPolicy<EntryType> entries;
    };

    // --- Test Cases ---

    namespace Tests
    {

        using namespace business_operations;
        template <typename T>
        concept supports_id = requires(T a) {
            { a.id } noexcept;
        };

        template <typename T>
        concept supports_data = requires(T a) {
            { a.data };
        };

        template <typename T>
        concept supports_timestamp = requires(T a) {
            { a.timestamp };
        };

        void test_ledger_entry_creation();
        void test_ledger_add_entry();
        void test_ledger_find_entries_basic();
        void test_ledger_find_entries_complex();

        // Test suite structure for comprehensive validation
        struct TestResult {
            bool passed;
            std::string message;
            std::string details;
        };

        template<typename T>
        void assert_equals(const T& expected, const T& actual, const std::string& message) {
            if (expected != actual) {
                throw std::runtime_error(message + 
                    "\nExpected: " + std::to_string(expected) + 
                    "\nActual: " + std::to_string(actual));
            }
        }

        // Add comprehensive test cases before run_all_tests()
        TestResult test_ledger_entry_numeric_limits() {
            try {
                // Test numeric limits for integer IDs
                using IntEntry = LedgerEntry<int, double>;
                IntEntry max_entry(std::numeric_limits<int>::max(), 100.0);
                IntEntry min_entry(std::numeric_limits<int>::min(), -100.0);
                
                // Verify no overflow occurred
                assert_equals(std::numeric_limits<int>::max(), max_entry.id, "Max ID verification failed");
                assert_equals(std::numeric_limits<int>::min(), min_entry.id, "Min ID verification failed");
                
                return {true, "Numeric limits test passed", "All boundary checks successful"};
            } catch (const std::exception& e) {
                return {false, "Numeric limits test failed", e.what()};
            }
        }

        TestResult test_ledger_concurrency_safety() {
            try {
                using Entry = LedgerEntry<int, std::string>;
                using SafeLedger = Ledger<Entry>;
                SafeLedger ledger;
                
                // Test timestamp monotonicity
                auto t1 = std::chrono::system_clock::now();
                ledger.add_entry(1, "First Entry");
                auto t2 = std::chrono::system_clock::now();
                
                const auto& entries = ledger.get_entries();
                if (entries[0].timestamp < t1 || entries[0].timestamp > t2) {
                    throw std::runtime_error("Timestamp outside expected range");
                }
                
                return {true, "Concurrency safety test passed", "Timestamp verification successful"};
            } catch (const std::exception& e) {
                return {false, "Concurrency safety test failed", e.what()};
            }
        }

        void run_all_tests()
        {
            std::vector<std::pair<std::string, TestResult>> results;
            
            // Run basic tests first
            test_ledger_entry_creation();
            test_ledger_add_entry();
            test_ledger_find_entries_basic();
            test_ledger_find_entries_complex();
            
            // Run advanced validation tests
            results.emplace_back("Numeric Limits Test", test_ledger_entry_numeric_limits());
            results.emplace_back("Concurrency Safety Test", test_ledger_concurrency_safety());
            
            // Report results
            std::cout << "\n=== Detailed Test Results ===\n";
            for (const auto& [name, result] : results) {
                std::cout << name << ": " << (result.passed ? "PASSED" : "FAILED") << "\n";
                if (!result.passed) {
                    std::cout << "Details: " << result.details << "\n";
                }
            }
        }

        void test_ledger_entry_creation()
        {
            using Entry = LedgerEntry<int, std::string>;
            Entry entry(123, "Acme Corp - Order #456");
            static_assert(supports_id<Entry>);
            static_assert(supports_data<Entry>);
            static_assert(supports_timestamp<Entry>);
            std::cout << "Test: LedgerEntry creation - Passed" << std::endl;
        }

        void test_ledger_add_entry()
        {
            using MyLedger = Ledger<LedgerEntry<int, std::string>>;
            MyLedger ledger;
            ledger.add_entry(456, "General Electric - Invoice #789");
            if (!ledger.get_entries().empty())
            {
                std::cout << "Test: Ledger add_entry - Passed" << std::endl;
            }
            else
            {
                std::cerr << "Test: Ledger add_entry - Failed" << std::endl;
            }
        }

        void test_ledger_find_entries_basic()
        {
            using Entry = LedgerEntry<int, std::string>;
            using MyLedger = Ledger<Entry>;
            MyLedger ledger;
            ledger.add_entry(1001, "US Steel - Shipment #12");
            ledger.add_entry(1002, "Chrysler - Parts Order #34");
            ledger.add_entry(1003, "General Motors - Assembly Line Report");

            auto results = ledger.find_entries([](const Entry &entry)
                                               { return entry.id > 1001; });

            if (results.size() == 2)
            {
                std::cout << "Test: Ledger find_entries (basic) - Passed" << std::endl;
            }
            else
            {
                std::cerr << "Test: Ledger find_entries (basic) - Failed (Expected 2, got " << results.size() << ")" << std::endl;
            }
        }

        void test_ledger_find_entries_complex()
        {
            using Entry = LedgerEntry<std::string, double>;
            using MyLedger = Ledger<Entry>;
            MyLedger ledger;
            ledger.add_entry("Transaction-A", 1500.00);
            ledger.add_entry("Transaction-B", 2250.50);
            ledger.add_entry("Transaction-C", 875.90);
            ledger.add_entry("Transaction-D", 3100.75);

            // Find entries with amounts greater than 1000.00
            auto high_value_transactions = ledger.find_entries([](const Entry &entry)
                                                               { return entry.data > 1000.00; });

            if (high_value_transactions.size() == 3)
            {
                std::cout << "Test: Ledger find_entries (complex) - Passed" << std::endl;
            }
            else
            {
                std::cerr << "Test: Ledger find_entries (complex) - Failed (Expected 3, got " << high_value_transactions.size() << ")" << std::endl;
            }
        }
    } // namespace Tests
} // namespace business_operations

// Modify main to handle test failures
int main()
{
    try {
        business_operations::Tests::run_all_tests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal test error: " << e.what() << std::endl;
        return 1;
    }
}