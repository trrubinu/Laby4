#pragma once
#include <string>
#include <memory>
#include "DiscountStrategy.h"

class Tariff {
private:
    std::wstring name;
    double basePrice;
    std::unique_ptr<IDiscountStrategy> discountStrategy;

public:
    Tariff(const std::wstring& n, double price, IDiscountStrategy* strategy = nullptr);
    Tariff(const Tariff& other);
    Tariff& operator=(const Tariff& other);

    std::wstring getName() const;
    double getBasePrice() const;
    double getFinalPrice() const;
    std::wstring getDiscountInfo() const;
    std::wstring getDiscountDisplayInfo() const;
    int getDiscountType() const;
    double getDiscountValue() const;
};
