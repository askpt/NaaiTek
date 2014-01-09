#include "User.h"
#include <string>

using namespace std;

User::User()
{

}

User::User(wstring name, wstring email, wstring country, wstring city, int number, int day, int month, int year)
{
	
	this->name = name;
	this->email = email;
	this->country = country;
	this->city = city;
	this->number = number;
	this->day = day;
	this->month = month;
	this->year = year;
}

User::~User()
{}

User::User(const User &u)
{
	name = u.name;
	email = u.email;
	country = u.country;
	city = u.city;
	number = u.number;
	day = u.day;
	month = u.month;
	year = u.year;
}

User* User::clone() const{
	return new User(*this);
}
