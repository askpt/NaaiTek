#ifndef User_
#define User_

#include <string>

using namespace std;

class User
{
public:
	User();
	User(wstring name, wstring email, wstring country, wstring city, int number, int day, int month, int year);
	User(const User &u);
	~User();
	User* clone() const;

	wstring name, email, country, city;
	int number, day, month, year;

private:

};

#endif // !User_
