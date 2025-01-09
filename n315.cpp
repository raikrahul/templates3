#include <iostream>
#include <string>
#include <type_traits>
#include <cassert> // Include for assert

// All code in the global namespace as requested

// Forward declarations to resolve potential circular dependencies

template <typename Record, typename... Actions>
struct DataProcessingPipeline;

template <typename Record>
struct EndOfProcessing;

// Represents a data record in the 1950s business context
template <typename NameType, typename DepartmentType, typename SalaryType, typename HireDateType>
struct EmployeeRecord {
    NameType name;
    DepartmentType department;
    SalaryType salary;
    HireDateType hire_date;
};

// Define operator<< as a free function
template <typename NameType, typename DepartmentType, typename SalaryType, typename HireDateType>
std::ostream& operator<<(std::ostream& os, 
    const EmployeeRecord<NameType, DepartmentType, SalaryType, HireDateType>& record) {
    os << "Name: " << record.name
       << ", Department: " << record.department
       << ", Salary: " << record.salary
       << ", Hire Date: " << record.hire_date;
    return os;
}

// Base case for the processing pipeline
template <typename Record>
struct DataProcessingPipeline<Record> {
    using CurrentRecord = Record;

    DataProcessingPipeline(Record initial_record) : current_record(initial_record) {}

    CurrentRecord get_final_record() const { return current_record; }

private:
    CurrentRecord current_record;
};

// Recursive structure for the processing pipeline
template <typename Record, typename Action, typename... RemainingActions>
struct DataProcessingPipeline<Record, Action, RemainingActions...> {
    using CurrentRecord = Record;
    using CurrentAction = Action;
    using RemainingPipeline = DataProcessingPipeline<CurrentRecord, RemainingActions...>;

    CurrentRecord initial_record;

    DataProcessingPipeline(CurrentRecord initial_record) : initial_record(initial_record) {}

    template <typename... Args>
    auto process(Args&&... args) {
        // Apply the current action to the current record
        auto next_record = CurrentAction::template apply<CurrentRecord>(initial_record, std::forward<Args>(args)...);
        return DataProcessingPipeline<decltype(next_record), RemainingActions...>(next_record);
    }

    auto process() {
        auto next_record = CurrentAction::template apply<CurrentRecord>(initial_record);
        return DataProcessingPipeline<decltype(next_record), RemainingActions...>(next_record);
    }

    decltype(auto) get_final_record() const {
        // This should only be called on the final stage of the pipeline
        static_assert(std::is_same_v<DataProcessingPipeline<Record, Action, RemainingActions...>, DataProcessingPipeline<Record>>,
                      "get_final_record should only be called on the completed pipeline");
        return initial_record;
    }

private:
    CurrentRecord current_record = initial_record;
};

// Helper function to start the pipeline
template <typename Record, typename Action, typename... RemainingActions>
auto start_processing(Record initial_record) {
    return DataProcessingPipeline<Record, Action, RemainingActions...>(initial_record);
}

// ---------------------- Example Actions (Processing Steps) ----------------------

// A simple action to update the department
struct DepartmentUpdater {
    template <typename Record>
    static auto apply(Record record, std::string new_department) {
        return EmployeeRecord<typename std::remove_cvref_t<decltype(record.name)>, std::string, typename std::remove_cvref_t<decltype(record.salary)>, typename std::remove_cvref_t<decltype(record.hire_date)>>{
            record.name, new_department, record.salary, record.hire_date
        };
    }
};

// Another simple action to give a standard raise
struct StandardRaise {
    template <typename Record>
    static auto apply(Record record, double raise_percentage) {
        return EmployeeRecord<typename std::remove_cvref_t<decltype(record.name)>, typename std::remove_cvref_t<decltype(record.department)>, double, typename std::remove_cvref_t<decltype(record.hire_date)>>{
            record.name, record.department, record.salary * (1.0 + raise_percentage), record.hire_date
        };
    }
};

// An action that depends on a non-type template parameter (department code)
template <int NewDepartmentCode>
struct DepartmentChanger {
    template <typename Record>
    static auto apply(Record record) {
        return EmployeeRecord<typename std::remove_cvref_t<decltype(record.name)>, int, typename std::remove_cvref_t<decltype(record.salary)>, typename std::remove_cvref_t<decltype(record.hire_date)>>{
            record.name, NewDepartmentCode, record.salary, record.hire_date
        };
    }
};

// An action demonstrating a more complex logical operation - TO DO for the user
template <typename BonusCriteria, typename BonusCalculator>
struct BonusAllocator {
    template <typename Record>
    static auto apply(Record record) {
        // TO DO: This is the tricky part. Implement the logic to allocate a bonus.
        //       The `BonusCriteria` type will dictate if an employee is eligible for a bonus.
        //       The `BonusCalculator` type will calculate the bonus amount.
        //       Consider template specializations or other advanced template techniques
        //       within `BonusCriteria` and `BonusCalculator`.

        // Example of how you might access members of the Record (adjust as needed):
        // using NameType = typename std::remove_cvref_t<decltype(record.name)>;
        // using SalaryType = typename std::remove_cvref_t<decltype(record.salary)>;

        // For now, a placeholder that always returns the original record:
        return record;
    }
};

// Example Bonus Criteria (you can add more)
struct SalesBonusCriteria {
    template <typename Record>
    static constexpr bool is_eligible(const Record& record) {
        return record.department == "Sales";
    }
};

struct ManagementBonusCriteria {
    template <typename Record>
    static constexpr bool is_eligible(const Record& record) {
        return record.department == "Management";
    }
};

// Example Bonus Calculators (you can add more)
struct PercentageBonusCalculator {
    template <typename Record>
    static double calculate_bonus(const Record& record, double percentage) {
        return record.salary * percentage;
    }
};

struct FixedAmountBonusCalculator {
    template <typename Record>
    static double calculate_bonus(const Record& record, double amount) {
        return amount;
    }
};

// An action that uses a third template parameter for configuration
template <typename PayrollConfiguration>
struct PayrollProcessor {
    template <typename Record>
    static auto apply(Record record) {
        // In the 1950s, payroll might involve manual calculations based on configuration.
        // This action could perform checks or transformations based on PayrollConfiguration.
        // For simplicity, we'll just add a note for now.
        return EmployeeRecord<typename std::remove_cvref_t<decltype(record.name)>, typename std::remove_cvref_t<decltype(record.department)>, typename std::remove_cvref_t<decltype(record.salary)>, typename std::remove_cvref_t<decltype(record.hire_date)>>{
            record.name, record.department, record.salary, record.hire_date // No actual modification here for simplicity
        };
    }
};

struct StandardPayroll {
    static constexpr double tax_rate = 0.20;
};

struct ExecutivePayroll {
    static constexpr double bonus_rate = 0.10;
};

// An action demonstrating a fourth template parameter - more specific configuration
template <typename ReviewerName, typename ReviewCriteria, typename ReviewResultProcessor>
struct PerformanceReviewer {
    template <typename Record>
    static auto apply(Record record) {
        // TO DO: Implement the core logic for the performance review.
        //       `ReviewerName` is a type representing the reviewer (e.g., a struct with a name).
        //       `ReviewCriteria` is a type that checks if the employee meets the criteria.
        //       `ReviewResultProcessor` handles the outcome of the review.

        // For now, just return the original record.
        return record;
    }
};

struct JohnDoeReviewer {};
struct ExceedsExpectationsCriteria {};
struct PromoteEmployeeProcessor {};

int main() {
    // --- Test Cases ---

    // Test 1: Simple department update
    {
        EmployeeRecord record{"Alice Baker", "Sales", 5000.0, "1950-01-15"};
        auto pipeline = start_processing<EmployeeRecord<const char*, const char*, double, const char*>, DepartmentUpdater>(record);
        auto final_pipeline = pipeline.process(std::string{"Marketing"});
        EmployeeRecord updated_record = final_pipeline.get_final_record();
        static_assert(std::is_same_v<decltype(updated_record), EmployeeRecord<const char*, std::string, double, const char*>>, "Test 1 Failed: Incorrect record type");
        std::cout << "Test 1: Updated Department: " << updated_record.department << std::endl;
        assert(updated_record.department == "Marketing");
    }

    // Test 2: Standard raise application
    {
        EmployeeRecord record{"Bob Charlie", "Engineering", 6000.0, "1951-03-20"};
        auto pipeline = start_processing<EmployeeRecord<const char*, const char*, double, const char*>, StandardRaise>(record);
        auto final_pipeline = pipeline.process(0.05);
        EmployeeRecord updated_record = final_pipeline.get_final_record();
        static_assert(std::is_same_v<decltype(updated_record), EmployeeRecord<const char*, const char*, double, const char*>>, "Test 2 Failed: Incorrect record type");
        std::cout << "Test 2: Updated Salary: " << updated_record.salary << std::endl;
        assert(updated_record.salary == 6300.0);
    }

    // Test 3: Changing department using a non-type template parameter
    {
        EmployeeRecord record{"Catherine Davis", "Accounting", 5500.0, "1952-07-01"};
        auto pipeline = start_processing<EmployeeRecord<const char*, const char*, double, const char*>, DepartmentChanger<101>>(record);
        auto final_pipeline = pipeline.process();
        EmployeeRecord updated_record = final_pipeline.get_final_record();
        static_assert(std::is_same_v<decltype(updated_record), EmployeeRecord<const char*, int, double, const char*>>, "Test 3 Failed: Incorrect record type");
        std::cout << "Test 3: Updated Department Code: " << updated_record.department << std::endl;
        assert(updated_record.department == 101);
    }

    // Test 4: Applying multiple actions
    {
        EmployeeRecord record{"David Edwards", "Clerical", 4000.0, "1953-09-10"};
        auto pipeline = start_processing<EmployeeRecord<const char*, const char*, double, const char*>, DepartmentUpdater, StandardRaise>(record);
        auto after_department = pipeline.process(std::string{"HR"});
        auto final_pipeline = after_department.process(0.10);
        EmployeeRecord updated_record = final_pipeline.get_final_record();
        static_assert(std::is_same_v<decltype(updated_record), EmployeeRecord<const char*, std::string, double, const char*>>, "Test 4 Failed: Incorrect record type");
        std::cout << "Test 4: Updated Department: " << updated_record.department << ", Updated Salary: " << updated_record.salary << std::endl;
        assert(updated_record.department == "HR");
        assert(updated_record.salary == 4400.0);
    }

    // Test 5: Bonus Allocation (Needs your implementation in BonusAllocator)
    {
        EmployeeRecord record{"Eve Frank", "Sales", 7000.0, "1954-11-05"};
        auto pipeline = start_processing<EmployeeRecord<const char*, const char*, double, const char*>, BonusAllocator<SalesBonusCriteria, PercentageBonusCalculator>>(record);
        auto final_pipeline = pipeline.process();
        EmployeeRecord updated_record = final_pipeline.get_final_record();
        std::cout << "Test 5: Bonus Allocation - Check implementation in BonusAllocator, Record: " << updated_record << std::endl;
        // The actual assertion depends on your BonusAllocator implementation
    }

    // Test 6: Payroll Processing
    {
        EmployeeRecord record{"George Howard", "Management", 8000.0, "1955-02-18"};
        auto pipeline = start_processing<EmployeeRecord<const char*, const char*, double, const char*>, PayrollProcessor<StandardPayroll>>(record);
        auto final_pipeline = pipeline.process();
        EmployeeRecord updated_record = final_pipeline.get_final_record();
        std::cout << "Test 6: Payroll Processing, Record: " << updated_record << std::endl;
        // You might add logic to PayrollProcessor to modify the record.
    }

    // Test 7: Performance Review (Needs your implementation in PerformanceReviewer)
    {
        EmployeeRecord record{"Ivy Jones", "Engineering", 6500.0, "1956-04-22"};
        auto pipeline = start_processing<EmployeeRecord<const char*, const char*, double, const char*>, PerformanceReviewer<JohnDoeReviewer, ExceedsExpectationsCriteria, PromoteEmployeeProcessor>>(record);
        auto final_pipeline = pipeline.process();
        EmployeeRecord updated_record = final_pipeline.get_final_record();
        std::cout << "Test 7: Performance Review - Check implementation in PerformanceReviewer, Record: " << updated_record << std::endl;
        // The actual outcome depends on your PerformanceReviewer implementation
    }

    return 0;
}