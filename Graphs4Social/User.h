#ifndef User_
#define User_

class User
{
public:
	User();
	User(char* name, char* email, char* country, char* city, int number, int day, int month, int year);
	~User();

	char* name, *email, *country, *city;
	int number, day, month, year;

private:

};

#endif // !User_
