#include "money.h"



money::money()
{
}


money::~money()
{
}

std::string money::getMoney()
{
    return to_string(this->userMoney);
}

void money::setMoney(unsigned long long newMoney)
{
    this->userMoney = newMoney;
}