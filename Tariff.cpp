#include "Tariff.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>

// ==================== CTOR ====================
Tariff::Tariff(const std::wstring& n, double price, IDiscountStrategy* strategy)
    : name(n), basePrice(price)
{
    if (price <= 0.0)
        throw std::logic_error("Цена должна быть положительной");

    if (strategy) {
        strategy->validateDiscount(price);   // 🚨 запрет скидки > цены
        discountStrategy.reset(strategy->clone());
    }
    else {
        discountStrategy.reset(new NoDiscountStrategy());
    }
}

// ==================== COPY ====================
Tariff::Tariff(const Tariff& other)
    : name(other.name),
    basePrice(other.basePrice),
    discountStrategy(other.discountStrategy
        ? other.discountStrategy->clone()
        : nullptr)
{
}

// ==================== OPERATOR= ====================
Tariff& Tariff::operator=(const Tariff& other)
{
    if (this != &other) {
        name = other.name;
        basePrice = other.basePrice;
        discountStrategy.reset(
            other.discountStrategy
            ? other.discountStrategy->clone()
            : nullptr
        );
    }
    return *this;
}

// ==================== GETTERS ====================
std::wstring Tariff::getName() const {
    return name;
}

double Tariff::getBasePrice() const {
    return basePrice;
}

// 🚨 ГЛАВНАЯ ЗАЩИТА
double Tariff::getFinalPrice() const
{
    double finalPrice = basePrice;

    if (discountStrategy)
        finalPrice = discountStrategy->applyDiscount(basePrice);

    // защита от нуля/минуса
    finalPrice = std::max(1.0, finalPrice);

    // округление до 2 знаков
    return std::round(finalPrice * 100.0) / 100.0;
}

std::wstring Tariff::getDiscountInfo() const
{
    if (!discountStrategy)
        return L"Без скидки";

    return discountStrategy->getStrategyName();  // 🔥 напрямую Unicode
}

std::wstring Tariff::getDiscountDisplayInfo() const
{
    if (!discountStrategy) return L"\u2014";
    int t = discountStrategy->getType();
    double v = discountStrategy->getValue();
    if (t == 1) return std::to_wstring(static_cast<int>(v)) + L"%";
    if (t == 2) {
        std::wstringstream ss;
        if (v == static_cast<int>(v)) ss << static_cast<int>(v);
        else ss << std::fixed << std::setprecision(2) << v;
        return ss.str() + L" \u0440\u0443\u0431.";
    }
    return L"\u2014";
}

int Tariff::getDiscountType() const
{
    return discountStrategy ? discountStrategy->getType() : 0;
}

double Tariff::getDiscountValue() const
{
    return discountStrategy ? discountStrategy->getValue() : 0.0;
}

