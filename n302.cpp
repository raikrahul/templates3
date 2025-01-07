#include <iostream>
#include <array>
#include <concepts>
#include <stdexcept>

namespace n302
{
    // Concept for a Base Commission Calculator
    template <typename T>
    concept BaseCommissionCalc = requires(T a, double sales) {
        { a(sales) } -> std::convertible_to<double>;
    };

    // Concept for Commission Tiers
    template <typename T>
    concept CommissionTierDefinition = requires(const T& a, double sales) {
        { a.getCommissionRate(sales) } -> std::convertible_to<double>;
    };

    // Concept for Bonus Policy
    template <typename T>
    concept BonusPolicyDefinition = requires(const T& a, int tenureYears) {
        { a.getBonusPercentage(tenureYears) } -> std::convertible_to<double>;
    };

    // SimpleCommissionTiers struct
    struct SimpleCommissionTiers
    {
        std::array<std::pair<double, double>, 3> tiers;

        double getCommissionRate(double sales) const
        {
            if (sales < 0) {
                throw std::invalid_argument("Sales amount cannot be negative");
            }
            
            for (const auto& tier : tiers)
            {
                if (sales <= tier.first)
                    return tier.second;
            }
            return tiers.back().second;
        }
    };

    // TenureBonusPolicy struct
    struct TenureBonusPolicy
    {
        double getBonusPercentage(int tenureYears) const
        {
            if (tenureYears < 0) {
                throw std::invalid_argument("Tenure years cannot be negative");
            }
            
            if (tenureYears >= 10)
                return 0.05;
            if (tenureYears >= 5)
                return 0.02;
            return 0.00;
        }
    };

    // calculateCommission function
    template <
        typename SalesDataType,
        typename BaseCommissionCalculator,
        typename CommissionTiers,
        typename BonusPolicy>
        requires BaseCommissionCalc<BaseCommissionCalculator>
            && CommissionTierDefinition<CommissionTiers>
            && BonusPolicyDefinition<BonusPolicy>
    double calculateCommission(
        SalesDataType sales,
        const CommissionTiers& tiers,
        const BonusPolicy& bonusPolicy,
        int employeeTenureYears = 7)  // Made tenure parameter explicit
    {
        static_assert(std::is_arithmetic_v<SalesDataType>, "SalesDataType must be arithmetic");

        if (sales < 0) {
            throw std::invalid_argument("Sales amount cannot be negative");
        }

        // Calculate the base commission
        double baseCommission = BaseCommissionCalculator()(static_cast<double>(sales));

        // Get the commission rate based on sales tiers
        double commissionRate = tiers.getCommissionRate(static_cast<double>(sales));

        // Calculate the commission before bonus
        double commissionBeforeBonus = baseCommission * commissionRate;

        // Get the bonus percentage based on tenure
        double bonusPercentage = bonusPolicy.getBonusPercentage(employeeTenureYears);

        // Apply the bonus
        return commissionBeforeBonus * (1.0 + bonusPercentage);
    }

    // run_tests function
    void run_tests()
    {
        try {
            // Test case 1: Simple sales with a basic commission calculation
            {
                struct SimpleBaseCommission
                {
                    double operator()(double sales) const { return sales * 0.05; } // 5% base commission
                };
                SimpleCommissionTiers tiers{{{
                    {1000.0, 0.02},
                    {5000.0, 0.03},
                    {10000.0, 0.04}
                }}};
                TenureBonusPolicy bonusPolicy;

                double commission = calculateCommission<double, SimpleBaseCommission>(3000.0, tiers, bonusPolicy, 7);
                std::cout << "Test Case 1 Commission: " << commission << std::endl;
            }

            // Test case 2: Different sales data type (integer)
            {
                struct SimpleBaseCommission
                {
                    double operator()(double sales) const { return sales * 0.10; }
                };
                SimpleCommissionTiers tiers{{{
                    {500, 0.05},
                    {2000, 0.07},
                    {5000, 0.10}
                }}};
                TenureBonusPolicy bonusPolicy;

                struct CategoryAdjustedBaseCommission
                {
                    double operator()(double sales) const
                    {
                        // Example logic for category-adjusted base commission
                        return sales * 0.08;
                    }
                };

                double commission = calculateCommission<int, CategoryAdjustedBaseCommission, SimpleCommissionTiers, TenureBonusPolicy>(1500, tiers, bonusPolicy);
                std::cout << "Test Case 2 Commission: " << commission << std::endl;
            }

            // Test case 3: Custom Commission Tiers logic
            {
                struct VolumeBasedTiers
                {
                    double getCommissionRate(double sales) const
                    {
                        if (sales > 20000)
                            return 0.08;
                        if (sales > 10000)
                            return 0.06;
                        return 0.04;
                    }
                };
                struct SimpleBaseCommission
                {
                    double operator()(double sales) const { return sales * 0.03; }
                };
                TenureBonusPolicy bonusPolicy;

                struct TierInfluencedBaseCommission
                {
                    double operator()(double sales) const
                    {
                        double base = sales * 0.02;
                        if (sales > 9500 && sales <= 10000)
                        {
                            base += 10; // Small bump if close to the next tier
                        }
                        return base;
                    }
                };

                double commission = calculateCommission<double, TierInfluencedBaseCommission, VolumeBasedTiers, TenureBonusPolicy>(11000.0, VolumeBasedTiers{}, bonusPolicy);
                std::cout << "Test Case 3 Commission: " << commission << std::endl;
            }

            // Test case 4: Custom Bonus Policy
            {
                struct AggressiveBonusPolicy
                {
                    double getBonusPercentage(int tenureYears) const
                    {
                        if (tenureYears >= 5)
                            return 0.10; // Higher bonus for experienced staff
                        return 0.00;
                    }
                };
                struct SimpleBaseCommission
                {
                    double operator()(double sales) const { return sales * 0.05; }
                };
                SimpleCommissionTiers tiers{{{
                    {1000.0, 0.02},
                    {5000.0, 0.03},
                    {10000.0, 0.04}
                }}};

                struct PerformanceLinkedBaseCommission
                {
                    double operator()(double sales) const
                    {
                        double baseRate = 0.04;
                        if (sales > 8000)
                        {
                            baseRate += 0.01; // Reward current high performers with a better base
                        }
                        return sales * baseRate;
                    }
                };

                double commission = calculateCommission<double, PerformanceLinkedBaseCommission, SimpleCommissionTiers, AggressiveBonusPolicy>(6000.0, tiers, AggressiveBonusPolicy{});
                std::cout << "Test Case 4 Commission: " << commission << std::endl;
            }

            // Additional test case for error handling
            {
                SimpleCommissionTiers tiers{{{
                    {1000.0, 0.02},
                    {5000.0, 0.03},
                    {10000.0, 0.04}
                }}};
                TenureBonusPolicy bonusPolicy;
                struct SimpleBaseCommission {
                    double operator()(double sales) const { return sales * 0.05; }
                };

                try {
                    calculateCommission<double, SimpleBaseCommission>(
                        -1000.0, tiers, bonusPolicy, 7);
                } catch (const std::invalid_argument& e) {
                    std::cout << "Successfully caught negative sales: " << e.what() << std::endl;
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error in tests: " << e.what() << std::endl;
        }
    }
}

int main()
{
    n302::run_tests();
    return 0;
}