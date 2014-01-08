#include "User.h"

User::User()
{

}

User::User(int iduser, char* name, int conStrength)
{
	this->iduser = iduser;
	this->name = name;
	this->conStrength = conStrength;
}

User::~User()
{}