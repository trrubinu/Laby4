#pragma once
#include <exception>
#include <string>

class CompanyException : public std::exception {
protected:
    std::string message;
public:
    explicit CompanyException(const std::string& msg);
    const char* what() const noexcept override;
};

class DuplicateDirectionException : public CompanyException {
public:
    explicit DuplicateDirectionException(const std::string& dir);
};

class DirectionNotFoundException : public CompanyException {
public:
    explicit DirectionNotFoundException(const std::string& dir);
};

class ClientNotFoundException : public CompanyException {
public:
    explicit ClientNotFoundException(const std::string& passport);
};

class InvalidPriceException : public CompanyException {
public:
    InvalidPriceException();
};

class InvalidVolumeException : public CompanyException {
public:
    InvalidVolumeException();
};

class InvalidDiscountException : public CompanyException {
public:
    explicit InvalidDiscountException(const std::string& msg);
};
