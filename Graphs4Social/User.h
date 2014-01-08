#ifndef User_
#define User_

class User
{
public:
	User();
	User(int iduser, char* name, int conStrength);
	~User();

	int iduser;
	char* name;
	int conStrength;
private:

};

#endif // !User_
