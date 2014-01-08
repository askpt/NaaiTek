#ifndef Connection_
#define Connection_

class Connection
{
public:
	Connection();
	Connection(int nodei, int nodef, int strength);
	~Connection();

	int nodei;
	int nodef;
	int strength;
private:

};



#endif // !Connection_
