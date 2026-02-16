#include "DiscountStrategy.h"
#include <algorithm>

// ==================== NoDiscountStrategy ====================
double NoDiscountStrategy::applyDiscount(double price) const {
    return price;
}

std::wstring NoDiscountStrategy::getStrategyName() const {
    return L"Без скидки";
}

IDiscountStrategy* NoDiscountStrategy::clone() const {
    return new NoDiscountStrategy(*this);
}

// ==================== PercentageDiscountStrategy ====================
PercentageDiscountStrategy::PercentageDiscountStrategy(double p)
    : percent(p)
{
    if (p < 0.0 || p > 100.0)
        throw std::logic_error("Процент скидки должен быть 0..100");
}

void PercentageDiscountStrategy::validateDiscount(double basePrice) const {
    if (percent >= 100.0)
        throw std::logic_error("Скидка 100% недопустима");
}

double PercentageDiscountStrategy::applyDiscount(double price) const {
    double result = price * (1.0 - percent / 100.0);

    // защита от нуля/минуса
    return std::max(1.0, result);
}

std::wstring PercentageDiscountStrategy::getStrategyName() const {
    return L"Скидка " + std::to_wstring((int)percent) + L"%";
}

IDiscountStrategy* PercentageDiscountStrategy::clone() const {
    return new PercentageDiscountStrategy(*this);
}

// ==================== FixedDiscountStrategy ====================
FixedDiscountStrategy::FixedDiscountStrategy(double d)
    : discount(d)
{
    if (d < 0.0)
        throw std::logic_error("Фиксированная скидка должна быть ≥ 0");
}

void FixedDiscountStrategy::validateDiscount(double basePrice) const {
    if (discount >= basePrice)
        throw std::logic_error("Скидка ≥ цены недопустима");
}

double FixedDiscountStrategy::applyDiscount(double price) const {
    double result = price - discount;

    // защита от нуля/минуса
    return std::max(1.0, result);
}

std::wstring FixedDiscountStrategy::getStrategyName() const {
    return L"Фиксированная скидка " + std::to_wstring(discount);
}

IDiscountStrategy* FixedDiscountStrategy::clone() const {
    return new FixedDiscountStrategy(*this);
}
