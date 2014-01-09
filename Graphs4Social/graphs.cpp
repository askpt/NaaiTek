#define _CRT_SECURE_NO_WARNINGS

#include "graphs.h"
#include <iostream>
#include <fstream>
#include "User.h"
#include "Json.h"

#define __GRAFO__FILE__ "example.graph"

Node nodes[_MAX_NODES_GRAPH];
Path paths[_MAX_PATHS_GRAPH];
int numNodes = 0, numPaths = 0;
User users[_MAX_NODES_GRAPH];
Connection connections[_MAX_PATHS_GRAPH];

using namespace std;

//Node

void addNode(Node node){
	if (numNodes < _MAX_NODES_GRAPH){
		nodes[numNodes] = node;
		numNodes++;
	}
	else{
		cout << "Number of nodes reached limits!" << endl;
	}
}

void deleteNode(int indNode){
	if (indNode >= 0 && indNode < numNodes){
		for (int i = indNode; i < numNodes; nodes[i++] = nodes[i + i]);
		numNodes--;
	}
	else{
		cout << "Invalid node index." << endl;
	}
}

void printNode(Node node){
	cout << "X:" << node.x << "Y:" << node.y << "Z:" << node.z << endl;
}

void printNodes(){
	for (int i = 0; i < numNodes; printNode(nodes[i++]));
}

Node createNode(float x, float y, float z, User *user){
	Node node;
	node.x = x;
	node.y = y;
	node.z = z;
	node.user = user;
	return node;
}

void addPath(Path path){
	if (numPaths < _MAX_PATHS_GRAPH){
		paths[numPaths] = path;
		numPaths++;
	}
	else{
		cout << "Number of paths reached limit!" << endl;
	}
}

//Path

void deletePath(int indPath){
	if (indPath >= 0 && indPath < numPaths){
		for (int i = indPath; i < numPaths; paths[i++] = paths[i + i]);
		numPaths--;
	}
	else{
		cout << "Invalid path index." << endl;
	}
}

void printPath(Path path){
	cout << "Start node:" << path.connection.nodei << "End node:" << path.connection.nodef
		<< "Strength:" << path.connection.strength << "Width:" << path.width << endl;
}

void listPaths(){
	for (int i = 0; i < numPaths; printPath(paths[i++]));
}

Path createPath(float width, Connection connection){
	Path path;
	path.connection = connection;
	path.width = width;
	return path;
}

//Graph

void saveGraph(){
	ofstream myfile;

	myfile.open(__GRAFO__FILE__, ios::out);
	if (!myfile.is_open()) {
		cout << "Error opening " << __GRAFO__FILE__ << "to write" << endl;
		exit(1);
	}
	myfile << numNodes << endl;
	for (int i = 0; i < numNodes; i++)
		myfile << nodes[i].x << " " << nodes[i].y << " " << nodes[i].z << endl;
	myfile << numPaths << endl;
	for (int i = 0; i < numPaths; i++)
		myfile << paths[i].connection.nodei << " " << paths[i].connection.nodef << " " << paths[i].width << endl;
	myfile.close();
}

void readGraph(){

	/*
	utility::string_t uri = L"http://uvm061.dei.isep.ipp.pt:9000/branch_and_bound?personA=Joao&personB=JoseCid";
	IterateJSONValue(uri);*/
	ifstream myfile;

	myfile.open(__GRAFO__FILE__, ios::in);
	if (!myfile.is_open()) {
		cout << "Error oppening " << __GRAFO__FILE__ << " to write" << endl;
		exit(1);
	}
	myfile >> numNodes;
	for (int i = 0; i < numNodes; i++)
		myfile >> nodes[i].x >> nodes[i].y >> nodes[i].z;
	myfile >> numPaths;
	for (int i = 0; i < numPaths; i++)
		myfile >> paths[i].connection.nodei >> paths[i].connection.nodef >> paths[i].width;
	myfile.close();
	// calculates the width of each node = bigger width of the paths that diverge/converge from/to that node	
	for (int i = 0; i < numNodes; i++){
		nodes[i].width = 0;
		for (int j = 0; j < numPaths; j++)
		if ((paths[j].connection.nodei == i || paths[j].connection.nodef == i) && nodes[i].width < paths[j].width)
			nodes[i].width = paths[j].width;
	}
}

void readGraphUser(std::string user)
{
	wstring userWs(user.begin(), user.end());

	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/get_graph?user=" + userWs;

	utility::string_t urlDim = L"http://uvm061.dei.isep.ipp.pt:5000/get_users_dimension";


	json::value graph = RequestJSONValueAsync(url).get();
	json::value usersDim = RequestJSONValueAsync(urlDim).get();

	numNodes = graph[L"nodes"].size();
	//numNodes = 2;
	wstring usersNodes[100];

	userWs = L'\"' + userWs + L'\"';
	int nodePos = 1;

	for (auto iter = graph[L"nodes"].begin(); iter != graph[L"nodes"].end(); ++iter)
	{
		for (auto iterDim = usersDim[L"users"].begin(); iterDim != usersDim[L"users"].end(); ++iterDim)
		{
			if (iterDim->second[L"user"] == iter->second)
			{

				utility::string_t urlUserInfo = L"http://wvm061.dei.isep.ipp.pt/databaseWS/Database.svc/user/" + iter->second.as_string();
				json::value userInfo = RequestJSONValueAsync(urlUserInfo).get();

				wstring userName = userInfo[L"Username"].as_string();
				wstring email = userInfo[L"Email"].as_string();
				wstring country = userInfo[L"Country"].as_string();
				wstring city = userInfo[L"City"].as_string();
				int day = userInfo[L"Day"].as_integer();
				int month = userInfo[L"Month"].as_integer();
				int year = userInfo[L"Year"].as_integer();
				int number = userInfo[L"Number"].as_integer();

				if (iterDim->second[L"user"].to_string().compare(userWs) == 0)
				{
					//pos i = 0 is authenticated user

					nodes[0].x = 0;
					nodes[0].y = 0;
					nodes[0].z = iterDim->second[L"dimension"].as_integer() * 2;
					nodes[0].width = userInfo[L"numTags"].as_integer() + 1;

					nodes[0].user = new User(userName, email, country, city, number, day, month, year);

					usersNodes[0] = userName;
				}
				else
				{
					//TODO radial function
					nodes[nodePos].x = 20;
					nodes[nodePos].y = 30;
					nodes[nodePos].z = iterDim->second[L"dimension"].as_integer() * 2;
					nodes[nodePos].width = userInfo[L"numTags"].as_integer() + 1;

					nodes[nodePos].user = new User(userName, email, country, city, number, day, month, year);
					usersNodes[nodePos] = userName;

					nodePos++;
					//wcout << iter->first; //Pos
					//wcout << iter->second; // Nome
					//wcout << iterDim->second[L"dimension"]; //dimensao

					//utility::string_t urlUserInfo = L"http://wvm061.dei.isep.ipp.pt/databaseWS/Database.svc/user/" + iter->second.as_string();

					//json::value userInfo = RequestJSONValueAsync(urlUserInfo).get();
					//wcout << userInfo[L"Username"];
					//wcout << userInfo[L"City"];
					//wcout << userInfo[L"Country"];
					//wcout << userInfo[L"Day"];
					//wcout << userInfo[L"Email"];
					//cin.get();
				}
			}
		}
	}
	int p = 0;
	for (auto iter = graph[L"paths"].begin(); iter != graph[L"paths"].end(); ++iter)
	{
		int user1Pos = 0;
		int user2Pos = 0;
		wstring user1Name = iter->second[L"user1"].as_string();
		wstring user2Name = iter->second[L"user2"].as_string();


		for (size_t i = 0; i < numNodes; i++)
		{
			if (user1Name.compare(usersNodes[i]) == 0)
			{
				user1Pos = i;
			}
			if (user2Name.compare(usersNodes[i]) == 0)
			{
				user2Pos = i;
			}
		}

		paths[p].width = iter->second[L"connection"].as_integer();
		paths[p].connection.nodei = user1Pos;
		paths[p].connection.nodef = user2Pos;


		/*wcout << iter->second[L"user1"];
		wcout << iter->second[L"user2"];
		wcout << iter->second[L"connection"];
		wcout << iter->second[L"tags"] << endl;*/

		p++;
	}

	numPaths = graph[L"paths"].size();
}

bool TryAuth(string username, string password)
{
	wstring userWs(username.begin(), username.end());
	wstring passWs(password.begin(), password.end());

	utility::string_t url = L"http://wvm061.dei.isep.ipp.pt/databaseWS/Database.svc/auth?user=" + userWs;
	url += L"&pass=" + passWs;

	json::value obj = RequestJSONValueAsync(url).get();

	wstring status = obj[L"Status"].to_string();

	int retBool = true;

	char c_mb_str[80];
	wcstombs(c_mb_str, &status[0], 80);
	string s(c_mb_str);
	s[4] = 0;

	char test[] = { '"', 'o', 'k', '"' };

	for (size_t i = 0; i < 4; i++)
	{
		if (s[i] != test[i])
		{
			retBool = false;
		}
	}

	return (retBool);

}