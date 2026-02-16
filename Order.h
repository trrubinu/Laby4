#pragma once
#include <string>

class Order
{
private:
    std::wstring clientPassport;
    std::wstring tariffName;
    double volume;
    double pricePerUnit;

public:
    Order(const std::wstring& passport,
        const std::wstring& tariff,
        double volume,
        double price);

    const std::wstring& getClientPassport() const { return clientPassport; }
    const std::wstring& getTariffName() const { return tariffName; }
    double getVolume() const { return volume; }

    double getCost() const;
};
