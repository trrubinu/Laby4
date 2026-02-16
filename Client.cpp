#include "Client.h"

Client::Client(const std::wstring& name, const std::wstring& passport)
    : name(name), passport(passport)
{
}

const std::wstring& Client::getName() const
{
    return name;
}

const std::wstring& Client::getPassport() const
{
    return passport;
}
