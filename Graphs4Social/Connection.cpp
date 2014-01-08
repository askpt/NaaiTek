#include "Connection.h"

Connection::Connection()
{
}

Connection::Connection(int nodei, int nodef, int strength)
{
	this->nodei = nodei;
	this->nodef = nodef;
	this->strength = strength;
}

Connection::~Connection()
{
}