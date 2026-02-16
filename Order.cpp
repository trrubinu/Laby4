#include "Order.h"

Order::Order(const std::wstring& passport,
    const std::wstring& tariff,
    double volume,
    double price)
    : clientPassport(passport),
    tariffName(tariff),
    volume(volume),
    pricePerUnit(price)
{
}

double Order::getCost() const
{
    return volume * pricePerUnit;
}
