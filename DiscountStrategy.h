#pragma once
#include <string>
#include <stdexcept>

// ==================== Интерфейс ====================
class IDiscountStrategy {
public:
    virtual ~IDiscountStrategy() = default;

    virtual double applyDiscount(double price) const = 0;
    virtual std::wstring getStrategyName() const = 0;
    virtual IDiscountStrategy* clone() const = 0;
    virtual int getType() const = 0;   // 0=none, 1=percent, 2=fixed
    virtual double getValue() const = 0;

    virtual void validateDiscount(double basePrice) const {}
};

// ==================== Без скидки ====================
class NoDiscountStrategy : public IDiscountStrategy {
public:
    double applyDiscount(double price) const override;
    std::wstring getStrategyName() const override;
    IDiscountStrategy* clone() const override;
    int getType() const override { return 0; }
    double getValue() const override { return 0.0; }
};

// ==================== Процентная ====================
class PercentageDiscountStrategy : public IDiscountStrategy {
private:
    double percent;

public:
    PercentageDiscountStrategy(double p);

    double applyDiscount(double price) const override;
    std::wstring getStrategyName() const override;
    IDiscountStrategy* clone() const override;
    int getType() const override { return 1; }
    double getValue() const override { return percent; }
    void validateDiscount(double basePrice) const override;
};

// ==================== Фиксированная ====================
class FixedDiscountStrategy : public IDiscountStrategy {
private:
    double discount;

public:
    FixedDiscountStrategy(double d);

    double applyDiscount(double price) const override;
    std::wstring getStrategyName() const override;
    IDiscountStrategy* clone() const override;
    int getType() const override { return 2; }
    double getValue() const override { return discount; }
    void validateDiscount(double basePrice) const override;
};
