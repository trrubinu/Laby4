#include "CargoCompany.h"
#include "DiscountStrategy.h"
#include <stdexcept>
#include <limits>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <vector>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

void CargoCompany::addTariff(const std::wstring& name, double price, IDiscountStrategy* strategy)
{
    if (name.empty() || name.length() > 100)
        throw std::invalid_argument("Название тарифа должно быть от 1 до 100 символов");

    if (price <= 0 || price > std::numeric_limits<double>::max() / 2)
        throw std::invalid_argument("Цена должна быть положительным числом и не превышать допустимый диапазон");

    for (const auto& t : tariffs)
        if (t.getName() == name)
            throw std::runtime_error("Тариф с таким названием уже существует");

    tariffs.emplace_back(name, price, strategy);
}

void CargoCompany::addClient(const std::wstring& name, const std::wstring& passport)
{
    if (name.empty() || name.length() > 100)
        throw std::invalid_argument("ФИО должно быть от 1 до 100 символов");

    if (passport.empty() || passport.length() > 20)
        throw std::invalid_argument("Паспорт должен быть от 1 до 20 символов");

    for (const auto& c : clients)
        if (c.getPassport() == passport)
            throw std::runtime_error("Клиент с таким паспортом уже существует");

    clients.emplace_back(name, passport);
}

void CargoCompany::createOrder(const std::wstring& passport, const std::wstring& tariffName, double volume)
{
    if (passport.empty() || passport.length() > 20)
        throw std::invalid_argument("Паспорт должен быть от 1 до 20 символов");

    if (volume <= 0 || volume > 1000000)
        throw std::invalid_argument("Объём должен быть положительным числом и не превышать 1,000,000");

    // Проверка существования клиента
    bool clientExists = false;
    for (const auto& c : clients)
    {
        if (c.getPassport() == passport)
        {
            clientExists = true;
            break;
        }
    }
    if (!clientExists)
        throw std::runtime_error("Клиент с таким паспортом не найден");

    // Проверка существования тарифа
    bool tariffExists = false;
    double price = 0.0;
    for (const auto& t : tariffs)
    {
        if (t.getName() == tariffName)
        {
            tariffExists = true;
            price = t.getFinalPrice();
            break;
        }
    }
    if (!tariffExists)
        throw std::runtime_error("Тариф не найден");

    const double MAX_ORDER_COST = 100000000.0;
    double totalCost = price * volume;
    if (totalCost > MAX_ORDER_COST)
        throw std::runtime_error("Стоимость заказа превышает максимально допустимую (100 000 000 RUB)");

    orders.emplace_back(passport, tariffName, volume, price);
}

void CargoCompany::removeTariff(int index)
{
    if (index < 0 || index >= (int)tariffs.size())
        throw std::out_of_range("Неверный индекс тарифа");

    std::wstring tariffName = tariffs[index].getName();
    if (isTariffUsedInOrders(tariffName))
        throw std::runtime_error("Невозможно удалить тариф: он используется в заказах");

    tariffs.erase(tariffs.begin() + index);
}

void CargoCompany::removeClient(int index)
{
    if (index < 0 || index >= (int)clients.size())
        throw std::out_of_range("Неверный индекс клиента");

    std::wstring passport = clients[index].getPassport();
    if (hasOrdersForClient(passport))
        throw std::runtime_error("Невозможно удалить клиента: у него есть заказы");

    clients.erase(clients.begin() + index);
}

void CargoCompany::removeOrder(int index)
{
    if (index < 0 || index >= (int)orders.size())
        throw std::out_of_range("Неверный индекс заказа");

    orders.erase(orders.begin() + index);
}

void CargoCompany::updateTariff(int index, const std::wstring& name, double price, IDiscountStrategy* strategy)
{
    if (index < 0 || index >= (int)tariffs.size())
        throw std::out_of_range("Неверный индекс тарифа");

    if (name.empty() || name.length() > 100)
        throw std::invalid_argument("Название тарифа должно быть от 1 до 100 символов");

    if (price <= 0 || price > std::numeric_limits<double>::max() / 2)
        throw std::invalid_argument("Цена должна быть положительным числом и не превышать допустимый диапазон");

    for (size_t i = 0; i < tariffs.size(); ++i)
    {
        if (i != static_cast<size_t>(index) && tariffs[i].getName() == name)
            throw std::runtime_error("Тариф с таким названием уже существует");
    }

    tariffs[index] = Tariff(name, price, strategy);
}

void CargoCompany::updateClient(int index, const std::wstring& name, const std::wstring& passport)
{
    if (index < 0 || index >= (int)clients.size())
        throw std::out_of_range("Неверный индекс клиента");

    if (name.empty() || name.length() > 100)
        throw std::invalid_argument("ФИО должно быть от 1 до 100 символов");

    if (passport.empty() || passport.length() > 20)
        throw std::invalid_argument("Паспорт должен быть от 1 до 20 символов");

    for (size_t i = 0; i < clients.size(); ++i)
    {
        if (i != static_cast<size_t>(index) && clients[i].getPassport() == passport)
            throw std::runtime_error("Клиент с таким паспортом уже существует");
    }

    clients[index] = Client(name, passport);
}

void CargoCompany::updateOrder(int index, const std::wstring& passport, const std::wstring& tariff, double volume)
{
    if (index < 0 || index >= (int)orders.size())
        throw std::out_of_range("Неверный индекс заказа");

    if (passport.empty() || passport.length() > 20)
        throw std::invalid_argument("Паспорт должен быть от 1 до 20 символов");

    if (volume <= 0 || volume > 1000000)
        throw std::invalid_argument("Объём должен быть положительным числом и не превышать 1,000,000");

    // Проверка существования клиента
    bool clientExists = false;
    for (const auto& c : clients)
    {
        if (c.getPassport() == passport)
        {
            clientExists = true;
            break;
        }
    }
    if (!clientExists)
        throw std::runtime_error("Клиент с таким паспортом не найден");

    // Проверка существования тарифа
    bool tariffExists = false;
    double price = 0.0;
    for (const auto& t : tariffs)
    {
        if (t.getName() == tariff)
        {
            tariffExists = true;
            price = t.getFinalPrice();
            break;
        }
    }
    if (!tariffExists)
        throw std::runtime_error("Тариф не найден");

    orders[index] = Order(passport, tariff, volume, price);
}

// ===== СТАТИСТИКА И ПОИСК (из старой программы) =====
const Tariff* CargoCompany::findMinPriceTariff() const
{
    if (tariffs.empty()) return nullptr;
    const Tariff* minTariff = &tariffs[0];
    double minPrice = minTariff->getFinalPrice();
    for (const auto& t : tariffs)
    {
        double p = t.getFinalPrice();
        if (p < minPrice) { minPrice = p; minTariff = &t; }
    }
    return minTariff;
}

const Tariff* CargoCompany::findTariffByName(const std::wstring& name) const
{
    for (const auto& t : tariffs)
        if (t.getName() == name) return &t;
    return nullptr;
}

bool CargoCompany::clientExists(const std::wstring& passport) const
{
    for (const auto& c : clients)
        if (c.getPassport() == passport) return true;
    return false;
}

bool CargoCompany::hasOrdersForClient(const std::wstring& passport) const
{
    for (const auto& o : orders)
        if (o.getClientPassport() == passport) return true;
    return false;
}

bool CargoCompany::isTariffUsedInOrders(const std::wstring& tariffName) const
{
    for (const auto& o : orders)
        if (o.getTariffName() == tariffName) return true;
    return false;
}

const Client* CargoCompany::findClientByPassport(const std::wstring& passport) const
{
    for (const auto& c : clients)
        if (c.getPassport() == passport) return &c;
    return nullptr;
}

double CargoCompany::getTotalForClient(const std::wstring& passport) const
{
    double sum = 0;
    for (const auto& o : orders)
        if (o.getClientPassport() == passport)
            sum += o.getCost();
    return sum;
}

double CargoCompany::getTotalAll() const
{
    double sum = 0;
    for (const auto& o : orders)
        sum += o.getCost();
    return sum;
}

// ===== Вспомогательные для файла =====
std::wstring CargoCompany::trim(const std::wstring& s)
{
    size_t start = s.find_first_not_of(L" \t\r\n");
    if (start == std::wstring::npos) return L"";
    size_t end = s.find_last_not_of(L" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::vector<std::wstring> CargoCompany::split(const std::wstring& s, wchar_t delim)
{
    std::vector<std::wstring> parts;
    std::wstringstream ss(s);
    std::wstring item;
    while (std::getline(ss, item, delim))
        parts.push_back(trim(item));
    return parts;
}

static double parseDouble(const std::wstring& s)
{
    std::wstring normalized = s;
    for (auto& c : normalized)
        if (c == L',') c = L'.';
    return std::stod(normalized);
}

// ===== СОРТИРОВКА =====
void CargoCompany::sortTariffs(int column, bool ascending)
{
    if (column == 0)
    {
        if (ascending)
            std::sort(tariffs.begin(), tariffs.end(), [](const Tariff& a, const Tariff& b) { return a.getName() < b.getName(); });
        else
            std::sort(tariffs.begin(), tariffs.end(), [](const Tariff& a, const Tariff& b) { return a.getName() > b.getName(); });
    }
    else if (column == 1)
    {
        if (ascending)
            std::sort(tariffs.begin(), tariffs.end(), [](const Tariff& a, const Tariff& b) { return a.getBasePrice() < b.getBasePrice(); });
        else
            std::sort(tariffs.begin(), tariffs.end(), [](const Tariff& a, const Tariff& b) { return a.getBasePrice() > b.getBasePrice(); });
    }
    else if (column == 2)
    {
        if (ascending)
            std::sort(tariffs.begin(), tariffs.end(), [](const Tariff& a, const Tariff& b) { return a.getFinalPrice() < b.getFinalPrice(); });
        else
            std::sort(tariffs.begin(), tariffs.end(), [](const Tariff& a, const Tariff& b) { return a.getFinalPrice() > b.getFinalPrice(); });
    }
    else if (column == 3)
    {
        if (ascending)
            std::sort(tariffs.begin(), tariffs.end(), [](const Tariff& a, const Tariff& b) {
                if (a.getDiscountType() != b.getDiscountType()) return a.getDiscountType() < b.getDiscountType();
                return a.getDiscountValue() < b.getDiscountValue();
            });
        else
            std::sort(tariffs.begin(), tariffs.end(), [](const Tariff& a, const Tariff& b) {
                if (a.getDiscountType() != b.getDiscountType()) return a.getDiscountType() > b.getDiscountType();
                return a.getDiscountValue() > b.getDiscountValue();
            });
    }
}

void CargoCompany::sortClients(int column, bool ascending)
{
    if (column == 0)
    {
        if (ascending)
            std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) { return a.getName() < b.getName(); });
        else
            std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) { return a.getName() > b.getName(); });
    }
    else if (column == 1)
    {
        if (ascending)
            std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) { return a.getPassport() < b.getPassport(); });
        else
            std::sort(clients.begin(), clients.end(), [](const Client& a, const Client& b) { return a.getPassport() > b.getPassport(); });
    }
}

void CargoCompany::sortOrders(int column, bool ascending)
{
    if (column == 0)
    {
        if (ascending)
            std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) { return a.getClientPassport() < b.getClientPassport(); });
        else
            std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) { return a.getClientPassport() > b.getClientPassport(); });
    }
    else if (column == 1)
    {
        if (ascending)
            std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) { return a.getTariffName() < b.getTariffName(); });
        else
            std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) { return a.getTariffName() > b.getTariffName(); });
    }
    else if (column == 2)
    {
        if (ascending)
            std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) { return a.getVolume() < b.getVolume(); });
        else
            std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) { return a.getVolume() > b.getVolume(); });
    }
    else if (column == 3)
    {
        if (ascending)
            std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) { return a.getCost() < b.getCost(); });
        else
            std::sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) { return a.getCost() > b.getCost(); });
    }
}

// ===== ФАЙЛ (UTF-8 с BOM) =====
#ifdef _WIN32
static std::string wstringToUtf8(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    if (size <= 0) return std::string();
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &result[0], size, nullptr, nullptr);
    return result;
}
static std::wstring utf8ToWstring(const std::string& str)
{
    if (str.empty()) return std::wstring();
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    if (size <= 0) return std::wstring();
    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &result[0], size);
    return result;
}
#endif

void CargoCompany::saveToFile(const std::wstring& path)
{
#ifdef _WIN32
    FILE* f = _wfopen(path.c_str(), L"wb");
#else
    std::string pathNarrow(path.begin(), path.end());
    FILE* f = fopen(pathNarrow.c_str(), "wb");
#endif
    if (!f)
        throw std::runtime_error("Не удалось открыть файл для записи");

    const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    fwrite(bom, 1, 3, f);

#ifdef _WIN32
    auto writeLine = [&f](const std::wstring& wline) {
        std::string u8 = wstringToUtf8(wline);
        fwrite(u8.c_str(), 1, u8.size(), f);
    };
    writeLine(L"[TARIFFS]\n");
    for (const auto& t : tariffs)
    {
        wchar_t buf[64];
        swprintf(buf, 64, L"%.2f", t.getBasePrice());
        std::wstring line = t.getName() + L"\t" + buf;
        swprintf(buf, 64, L"\t%d\t%.2f\n", t.getDiscountType(), t.getDiscountValue());
        line += buf;
        writeLine(line);
    }
    writeLine(L"[CLIENTS]\n");
    for (const auto& c : clients)
    {
        std::wstring line = c.getName() + L"\t" + c.getPassport() + L"\n";
        writeLine(line);
    }
    writeLine(L"[ORDERS]\n");
    for (const auto& o : orders)
    {
        wchar_t buf[64];
        swprintf(buf, 64, L"%.2f", o.getVolume());
        std::wstring line = o.getClientPassport() + L"\t" + o.getTariffName() + L"\t" + buf + L"\n";
        writeLine(line);
    }
#else
    (void)path;
    fclose(f);
    throw std::runtime_error("Сохранение в UTF-8 поддерживается только на Windows");
#endif
    fclose(f);
}

void CargoCompany::loadFromFile(const std::wstring& path)
{
#ifdef _WIN32
    FILE* f = _wfopen(path.c_str(), L"rb");
#else
    std::string pathNarrow(path.begin(), path.end());
    FILE* f = fopen(pathNarrow.c_str(), "rb");
#endif
    if (!f)
        throw std::runtime_error("Не удалось открыть файл для чтения");

    tariffs.clear();
    clients.clear();
    orders.clear();

#ifdef _WIN32
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::vector<char> raw(fsize + 1, 0);
    if (fread(raw.data(), 1, (size_t)fsize, f) != (size_t)fsize)
    {
        fclose(f);
        throw std::runtime_error("Ошибка чтения файла");
    }
    fclose(f);

    size_t pos = 0;
    if (fsize >= 3 && (unsigned char)raw[0] == 0xEF && (unsigned char)raw[1] == 0xBB && (unsigned char)raw[2] == 0xBF)
        pos = 3;

    std::wstring fullText = utf8ToWstring(std::string(raw.data() + pos));
    std::wstringstream ss(fullText);
    std::wstring wline;
    int section = 0;
    while (std::getline(ss, wline))
    {
        std::wstring t = trim(wline);
        if (t.empty()) continue;
        if (t == L"[TARIFFS]") { section = 1; continue; }
        if (t == L"[CLIENTS]") { section = 2; continue; }
        if (t == L"[ORDERS]") { section = 3; continue; }

        auto parts = split(t, L'\t');
        if (section == 1 && parts.size() >= 4)
        {
            std::wstring name = parts[0];
            double basePrice = parseDouble(parts[1]);
            int discType = std::stoi(parts[2]);
            double discVal = parseDouble(parts[3]);
            IDiscountStrategy* strategy = nullptr;
            if (discType == 1) strategy = new PercentageDiscountStrategy(discVal);
            else if (discType == 2) strategy = new FixedDiscountStrategy(discVal);
            else strategy = new NoDiscountStrategy();
            try {
                tariffs.emplace_back(name, basePrice, strategy);
            } catch (...) {
                delete strategy;
                throw;
            }
            delete strategy;
        }
        else if (section == 2 && parts.size() >= 2)
        {
            clients.emplace_back(parts[0], parts[1]);
        }
        else if (section == 3 && parts.size() >= 3)
        {
            createOrder(parts[0], parts[1], parseDouble(parts[2]));
        }
    }
#else
    fclose(f);
    throw std::runtime_error("Загрузка UTF-8 поддерживается только на Windows");
#endif
}
