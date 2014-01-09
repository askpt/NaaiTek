#include "User.h"

User::User()
{

}

User::User(char* name, char* email, char* country, char* city, int number, int day, int month, int year)
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