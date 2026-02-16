#pragma once
#include <string>

class Client
{
private:
    std::wstring name;
    std::wstring passport;

public:
    Client(const std::wstring& name, const std::wstring& passport);

    const std::wstring& getName() const;
    const std::wstring& getPassport() const;
};
