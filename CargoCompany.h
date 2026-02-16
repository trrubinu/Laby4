#pragma once
#include <vector>
#include <string>
#include "Tariff.h"
#include "Client.h"
#include "Order.h"

class CargoCompany
{
private:
    std::vector<Tariff> tariffs;
    std::vector<Client> clients;
    std::vector<Order> orders;

    static std::wstring trim(const std::wstring& s);
    static std::vector<std::wstring> split(const std::wstring& s, wchar_t delim);

public:
    // ===== ДОБАВЛЕНИЕ =====
    void addTariff(const std::wstring& name, double price, IDiscountStrategy* strategy);
    void addClient(const std::wstring& name, const std::wstring& passport);
    void createOrder(const std::wstring& passport, const std::wstring& tariffName, double volume);

    // ===== УДАЛЕНИЕ =====
    void removeTariff(int index);
    void removeClient(int index);
    void removeOrder(int index);

    // ===== РЕДАКТИРОВАНИЕ =====
    void updateTariff(int index, const std::wstring& name, double price, IDiscountStrategy* strategy);
    void updateClient(int index, const std::wstring& name, const std::wstring& passport);
    void updateOrder(int index, const std::wstring& passport, const std::wstring& tariff, double volume);

    // ===== ГЕТТЕРЫ =====
    const std::vector<Tariff>& getTariffs() const { return tariffs; }
    const std::vector<Client>& getClients() const { return clients; }
    const std::vector<Order>& getOrders() const { return orders; }

    size_t getTariffCount() const { return tariffs.size(); }
    size_t getClientCount() const { return clients.size(); }
    size_t getOrderCount() const { return orders.size(); }

    const Tariff* findMinPriceTariff() const;
    const Tariff* findTariffByName(const std::wstring& name) const;
    bool clientExists(const std::wstring& passport) const;
    bool hasOrdersForClient(const std::wstring& passport) const;
    bool isTariffUsedInOrders(const std::wstring& tariffName) const;
    const Client* findClientByPassport(const std::wstring& passport) const;
    double getTotalForClient(const std::wstring& passport) const;
    double getTotalAll() const;

    // ===== СОРТИРОВКА =====
    void sortTariffs(int column, bool ascending);
    void sortClients(int column, bool ascending);
    void sortOrders(int column, bool ascending);

    // ===== ФАЙЛ =====
    void saveToFile(const std::wstring& path);
    void loadFromFile(const std::wstring& path);
};
