#include "Exceptions.h"

CompanyException::CompanyException(const std::string& msg) : message(msg) {}
const char* CompanyException::what() const noexcept { return message.c_str(); }

DuplicateDirectionException::DuplicateDirectionException(const std::string& dir)
    : CompanyException("Ошибка: тариф '" + dir + "' уже существует") {
}

DirectionNotFoundException::DirectionNotFoundException(const std::string& dir)
    : CompanyException("Ошибка: направление '" + dir + "' не найдено") {
}

ClientNotFoundException::ClientNotFoundException(const std::string& passport)
    : CompanyException("Ошибка: клиент с паспортом '" + passport + "' не найден") {
}

InvalidPriceException::InvalidPriceException()
    : CompanyException("Ошибка: цена должна быть положительной") {
}

InvalidVolumeException::InvalidVolumeException()
    : CompanyException("Ошибка: объем должен быть положительным") {
}

InvalidDiscountException::InvalidDiscountException(const std::string& msg)
    : CompanyException("Ошибка: " + msg) {
}
