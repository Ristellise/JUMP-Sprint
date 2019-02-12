#ifndef MONEY_H
#define MONEY_H

#include <string>

using namespace std;

// Contains money value
// Returns string
// Called by saveState loader
// Called by Shop

class money
{
private:
	unsigned long long userMoney;
public:
	money();
	~money();
	string getMoney();
	void setMoney(unsigned long long newMoney);
};

#endif
// Yours sincerely by Ryan