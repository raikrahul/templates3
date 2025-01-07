#include <iostream>
#include <string>
#include <type_traits>
#include <cassert> // For assert

namespace n313_manufacturing
{

// Forward declarations to resolve circular dependency
template <typename ProductState, typename Department = void, typename... RemainingDepartments>
struct ProductionLine;

// Base case for the production line: no more departments
template <typename ProductState>
struct ProductionLine<ProductState>
{
    using CurrentState = ProductState;

    ProductionLine(ProductState initial_state) : current_state(initial_state) {}

    CurrentState get_final_state() const { return current_state; }

private:
    ProductState current_state;
};

// Recursive structure to represent the production line with multiple departments
template <typename ProductState, typename Department, typename... RemainingDepartments>
struct ProductionLine
{
    using CurrentState = ProductState;
    using CurrentDepartment = Department;

    ProductionLine(ProductState initial_state) : current_state(initial_state) {}

    auto process()
    {
        // Apply the current department's process to the current state
        auto next_state = CurrentDepartment::process(current_state);
        // Create the next step in the production line
        return ProductionLine<decltype(next_state), RemainingDepartments...>(next_state);
    }

    // Prevent get_final_state() from being called on intermediate steps
    void get_final_state() const = delete;

private:
    ProductState current_state;
};

// Function to create a production line with multiple departments
template <typename ProductState, typename... Departments>
auto create_production_line(ProductState initial_state)
{
    return ProductionLine<ProductState, Departments...>(initial_state);
}

// Example Product types
struct RawMaterial {
    std::string value;
};

struct SemiFinishedProduct {
    std::string value;
};

struct FinishedProduct {
    std::string value;
};

// Example Department types

struct AssemblyDepartment
{
    template <typename Product>
    static auto process(Product current_product)
    {
        return SemiFinishedProduct{current_product.value + " - Assembled"};
    }
};

struct QualityControlDepartment
{
    template <typename Product>
    static auto process(Product current_product)
    {
        if (current_product.value.find("Defect") != std::string::npos) {
            return SemiFinishedProduct{current_product.value + " - Rejected"};
        } else {
            return SemiFinishedProduct{current_product.value + " - Passed QC"};
        }
    }
};

template <typename PolishMaterial>
struct PolishingDepartment
{
    template <typename Product>
    static auto process(Product current_product)
    {
        return FinishedProduct{current_product.value + " - Polished with " + PolishMaterial::name};
    }
};

struct TypeAPolish { static constexpr const char* name = "Type A Polish"; };
struct TypeBPolish { static constexpr const char* name = "Type B Polish"; };

template <int DepartmentID>
struct PackagingDepartment
{
    template <typename Product>
    static auto process(Product current_product)
    {
        return FinishedProduct{current_product.value + " - Packed in Box #" + std::to_string(DepartmentID)};
    }
};

template <typename Logger>
struct LoggingDepartment
{
    template <typename Product>
    static auto process(Product current_product)
    {
        Logger::log("Processed: " + current_product.value);
        return current_product;
    }
};

struct ConsoleLogger {
    static void log(const std::string& message) {
        std::cout << "[LOG]: " << message << std::endl;
    }
};

} // namespace n313_manufacturing

namespace Test1950sManufacturing {
    using namespace n313_manufacturing;

    void basic_assembly_test() {
        RawMaterial steel{"Raw Steel"};
        auto line = create_production_line<RawMaterial, AssemblyDepartment>(steel);
        auto final_line = line.process();
        static_assert(std::is_same_v<decltype(final_line), ProductionLine<SemiFinishedProduct>>, "Incorrect final state type");
        SemiFinishedProduct final_product = final_line.get_final_state();
        std::cout << "Test 1: " << final_product.value << std::endl;
        assert(final_product.value == "Raw Steel - Assembled");
    }

    void assembly_and_qc_test() {
        RawMaterial steel{"Raw Steel"};
        auto line = create_production_line<RawMaterial, AssemblyDepartment, QualityControlDepartment>(steel);
        auto after_assembly = line.process();
        auto final_line = after_assembly.process();
        static_assert(std::is_same_v<decltype(final_line), ProductionLine<SemiFinishedProduct>>, "Incorrect final state type");
        SemiFinishedProduct final_product = final_line.get_final_state();
        std::cout << "Test 2: " << final_product.value << std::endl;
        assert(final_product.value == "Raw Steel - Assembled - Passed QC");
    }

    void assembly_qc_and_polish_test() {
        RawMaterial steel{"Raw Steel"};
        auto line = create_production_line<RawMaterial, AssemblyDepartment, QualityControlDepartment, PolishingDepartment<TypeAPolish>>(steel);
        auto after_assembly = line.process();
        auto after_qc = after_assembly.process();
        auto final_line = after_qc.process();
        static_assert(std::is_same_v<decltype(final_line), ProductionLine<FinishedProduct>>, "Incorrect final state type");
        FinishedProduct final_product = final_line.get_final_state();
        std::cout << "Test 3: " << final_product.value << std::endl;
        assert(final_product.value == "Raw Steel - Assembled - Passed QC - Polished with Type A Polish");
    }

    void assembly_qc_polish_and_packaging_test() {
        RawMaterial steel{"Raw Steel"};
        auto line = create_production_line<RawMaterial, AssemblyDepartment, QualityControlDepartment, PolishingDepartment<TypeBPolish>, PackagingDepartment<5>>(steel);
        auto after_assembly = line.process();
        auto after_qc = after_assembly.process();
        auto after_polish = after_qc.process();
        auto final_line = after_polish.process();
        static_assert(std::is_same_v<decltype(final_line), ProductionLine<FinishedProduct>>, "Incorrect final state type");
        FinishedProduct final_product = final_line.get_final_state();
        std::cout << "Test 4: " << final_product.value << std::endl;
        assert(final_product.value == "Raw Steel - Assembled - Passed QC - Polished with Type B Polish - Packed in Box #5");
    }

    void assembly_defect_qc_test() {
        RawMaterial steel{"Raw Steel - Defect"};
        auto line = create_production_line<RawMaterial, AssemblyDepartment, QualityControlDepartment>(steel);
        auto after_assembly = line.process();
        auto final_line = after_assembly.process();
        static_assert(std::is_same_v<decltype(final_line), ProductionLine<SemiFinishedProduct>>, "Incorrect final state type");
        SemiFinishedProduct final_product = final_line.get_final_state();
        std::cout << "Test 5: " << final_product.value << std::endl;
        assert(final_product.value == "Raw Steel - Defect - Assembled - Rejected");
    }

    void assembly_logging_qc_test() {
        RawMaterial steel{"Raw Steel"};
        auto line = create_production_line<RawMaterial, AssemblyDepartment, LoggingDepartment<ConsoleLogger>, QualityControlDepartment>(steel);
        auto after_assembly = line.process();
        auto after_logging = after_assembly.process();
        auto final_line = after_logging.process();
        static_assert(std::is_same_v<decltype(final_line), ProductionLine<SemiFinishedProduct>>, "Incorrect final state type");
        SemiFinishedProduct final_product = final_line.get_final_state();
        std::cout << "Test 6: " << final_product.value << std::endl;
        assert(final_product.value == "Raw Steel - Assembled - Passed QC");
    }
}

int main() {
    Test1950sManufacturing::basic_assembly_test();
    Test1950sManufacturing::assembly_and_qc_test();
    Test1950sManufacturing::assembly_qc_and_polish_test();
    Test1950sManufacturing::assembly_qc_polish_and_packaging_test();
    Test1950sManufacturing::assembly_defect_qc_test();
    Test1950sManufacturing::assembly_logging_qc_test();
    return 0;
}