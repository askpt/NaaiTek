#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include "graphs.h"
#include <iostream>
#include <fstream>
#include "User.h"
#include "Json.h"
#include <math.h>

#define __GRAFO__FILE__ "example.graph"
#define __MINIGAMES_CONFIG_FILE__ "minigames.config"

Node nodes[_MAX_NODES_GRAPH];
Path paths[_MAX_PATHS_GRAPH];
int numNodes = 0, numPaths = 0;
User users[_MAX_NODES_GRAPH];
Connection connections[_MAX_PATHS_GRAPH];

using namespace std;

/* Adds the node to the nodes array */
void addNode(Node node){
	if (numNodes < _MAX_NODES_GRAPH){
		nodes[numNodes] = node;
		numNodes++;
	}
	else{
		cout << "Number of nodes reached limits!" << endl;
	}
}

/* Deletes the node from the nodes array */
void deleteNode(int indNode){
	if (indNode >= 0 && indNode < numNodes){
		for (int i = indNode; i < numNodes; nodes[i++] = nodes[i + i]);
		numNodes--;
	}
	else{
		cout << "Invalid node index." << endl;
	}
}

/* Prints the node coordinates */
void printNode(Node node){
	cout << "X:" << node.x << "Y:" << node.y << "Z:" << node.z << endl;
}

/* Prints all the nodes from the nodes list */
void printNodes(){
	for (int i = 0; i < numNodes; printNode(nodes[i++]));
}

/* Creates a node */
Node createNode(float x, float y, float z, User *user){
	Node node;
	node.x = x;
	node.y = y;
	node.z = z;
	node.user = user;
	return node;
}

/* Adds the path to the paths array */
void addPath(Path path){
	if (numPaths < _MAX_PATHS_GRAPH){
		paths[numPaths] = path;
		numPaths++;
	}
	else{
		cout << "Number of paths reached limit!" << endl;
	}
}

/* Deletes the path from the paths array */
void deletePath(int indPath){
	if (indPath >= 0 && indPath < numPaths){
		for (int i = indPath; i < numPaths; paths[i++] = paths[i + i]);
		numPaths--;
	}
	else{
		cout << "Invalid path index." << endl;
	}
}

/* Prints the path corresponding nodes, strenght and width */
void printPath(Path path){
	cout << "Start node:" << path.connection.nodei << "End node:" << path.connection.nodef
		<< "Strength:" << path.connection.strength << "Width:" << path.width << endl;
}

/* Prints all the paths from the paths list */
void listPaths(){
	for (int i = 0; i < numPaths; printPath(paths[i++]));
}

/* Creates a path */
Path createPath(float width, Connection connection){
	Path path;
	path.connection = connection;
	path.width = width;
	return path;
}

/* Saves the graph to a file */
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

/* Saves the graph from a file */
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

/* Calculates the node width depending on the number of tags */
float calcNodeWidth(int param)
{
	return (param + 2.5) * 0.3;
}

/* Calculates the path width depending on the connection strength */
float calcPathWidth(int param)
{
	return (param + 1) * 0.25;
}

/* Reads the graph from the PROLOG WS depending on the Registered User */
void readGraphUser(std::string user)
{
	wstring userWs(user.begin(), user.end());

	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/get_graph?user=" + userWs;

	utility::string_t urlDim = L"http://uvm061.dei.isep.ipp.pt:5000/get_users_dimension";


	json::value graph = RequestJSONValueAsync(url).get();
	json::value usersDim = RequestJSONValueAsync(urlDim).get();

	buildGraph(graph, usersDim, userWs);
}

/* Reads the graph from the PROLOG WS depending on the Registered User and a Friend to the 3rd degree */
void readCommonGraph(wstring user, wstring friendUser)
{
	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/get_common_graph?personA=" + user + L"&personB=" + friendUser;

	utility::string_t urlDim = L"http://uvm061.dei.isep.ipp.pt:5000/get_users_dimension";


	json::value graph = RequestJSONValueAsync(url).get();
	json::value usersDim = RequestJSONValueAsync(urlDim).get();

	buildGraph(graph, usersDim, user);
}

/* Actually fills the "nodes" and "paths" using:
 * the graph of the registered user to the 2nd degree friends
 * the userDim that is the number of users in the registered user graph
 * the userWs that is the registered user converted to wide string */
void buildGraph(json::value graph, json::value usersDim, wstring userWs)
{
	numNodes = graph[L"nodes"].size();
	wstring usersNodes[_MAX_NODES_GRAPH];

	if (numNodes == 0)
	{
		numNodes = 1;
		utility::string_t urlUserInfo = L"http://wvm061.dei.isep.ipp.pt/databaseWS/Database.svc/user/" + userWs;
		json::value userInfo = RequestJSONValueAsync(urlUserInfo).get();

		wstring userName = userInfo[L"Username"].as_string();
		wstring email = userInfo[L"Email"].as_string();
		wstring country = userInfo[L"Country"].as_string();
		wstring city = userInfo[L"City"].as_string();
		int day = userInfo[L"Day"].as_integer();
		int month = userInfo[L"Month"].as_integer();
		int year = userInfo[L"Year"].as_integer();
		int number = userInfo[L"Number"].as_integer();

		nodes[0].x = 0;
		nodes[0].y = 0;
		nodes[0].z = 0;
		nodes[0].width = calcNodeWidth(userInfo[L"numTags"].as_integer());

		nodes[0].user = new User(userName, email, country, city, number, day, month, year);

		usersNodes[0] = userName;
	}
	else{
		//numNodes = 2;

		userWs = L'\"' + userWs + L'\"';
		int nodePos = 1;
		float t = 2.0 * M_PI / numNodes;

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
						nodes[0].z = iterDim->second[L"dimension"].as_integer() * 4;
						nodes[0].width = calcNodeWidth(userInfo[L"numTags"].as_integer());

						nodes[0].user = new User(userName, email, country, city, number, day, month, year);

						usersNodes[0] = userName;

					}
					else
					{
						float r = numNodes * 3.0;

						nodes[nodePos].x = r * cos(t);
						nodes[nodePos].y = r * sin(t);
						nodes[nodePos].z = iterDim->second[L"dimension"].as_integer() * 4;
						nodes[nodePos].width = calcNodeWidth(userInfo[L"numTags"].as_integer());

						nodes[nodePos].user = new User(userName, email, country, city, number, day, month, year);
						usersNodes[nodePos] = userName;

						nodePos++;
						t += 2.0*M_PI / numNodes;
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

			paths[p].width = calcPathWidth(iter->second[L"connection"].as_integer());
			paths[p].connection.nodei = user1Pos;
			paths[p].connection.nodef = user2Pos;
			paths[p].connection.strength = iter->second[L"connection"].as_integer();

			/*wcout << iter->second[L"user1"];
			wcout << iter->second[L"user2"];
			wcout << iter->second[L"connection"];
			wcout << iter->second[L"tags"] << endl;*/

			p++;
		}

		numPaths = graph[L"paths"].size();
	}
}

/* Checks if the user credentials are correct */
bool TryAuth(string username, string password)
{
	wstring userWs(username.begin(), username.end());
	wstring passWs(password.begin(), password.end());

	utility::string_t url = L"http://wvm061.dei.isep.ipp.pt/databaseWS/Database.svc/auth?user=" + userWs;
	url += L"&pass=" + passWs;

	json::value obj = RequestJSONValueAsync(url).get();

	wstring status = obj[L"Status"].to_string();

	return checkIfStatusOk(status);

}

/* Checks if the correct JSON was returned */
bool checkIfStatusOk(wstring status)
{
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

/* Sends a friend request from the registered user to a possible friend */
bool sendFriendRequest(wstring user, wstring friendUser)
{
	wstring userWs(user.begin(), user.end());
	wstring friendUserWs(friendUser.begin(), friendUser.end());

	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/request_friend?personA=" + userWs + L"&personB=" + friendUserWs;

	json::value request = RequestJSONValueAsync(url).get();
	wstring status = request[L"Status"].to_string();

	return checkIfStatusOk(status);
}

/* Retrieves the shortest path from the registered user to a friend till the 2nd degree */
vector<wstring> GetShortPath(wstring user, wstring friendUser)
{
	vector<wstring> path;

	wstring userWs(user.begin(), user.end());
	wstring friendUserWs(friendUser.begin(), friendUser.end());

	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/breadth_search?personA=" + userWs + L"&personB=" + friendUserWs;

	json::value pathJs = RequestJSONValueAsync(url).get();
	wstring status = pathJs[L"status"].to_string();

	if (checkIfStatusOk(status))
	{
		path.resize(pathJs[L"path"].size());
		int i = 0;
		for (auto iter = pathJs[L"path"].begin(); iter != pathJs[L"path"].end(); ++iter)
		{
			path[i] = iter->second.as_string();
			i++;
		}
	}
	else
	{
		path.resize(0);
	}

	return path;
}

/* Retrieves the strongest path from the registered user to a friend till the 2nd degree */
vector<wstring> GetStrongestPath(wstring user, wstring friendUser)
{
	vector<wstring> path;

	wstring userWs(user.begin(), user.end());
	wstring friendUserWs(friendUser.begin(), friendUser.end());

	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/branch_and_bound?personA=" + userWs + L"&personB=" + friendUserWs;

	json::value pathJs = RequestJSONValueAsync(url).get();
	wstring status = pathJs[L"status"].to_string();

	if (checkIfStatusOk(status))
	{
		path.resize(pathJs[L"path"].size());
		int i = 0;
		for (auto iter = pathJs[L"path"].begin(); iter != pathJs[L"path"].end(); ++iter)
		{
			path[i] = iter->second.as_string();
			i++;
		}
	}
	else
	{
		path.resize(0);
	}

	return path;
}

/* Checks if the connection exists in path list */
bool CheckIfConnectionExistsInPathList(vector<wstring> paths, wstring user1, wstring user2)
{
	bool ret = false;

	for (size_t i = 0; i < paths.size() - 1; i++)
	{
		if ((paths[i] == user1 && paths[i + 1] == user2) || (paths[i] == user2 && paths[i + 1] == user1))
		{
			ret = true;
		}
	}


	return ret;
}

/* Gets the names of the mini-games that can be played */
vector<string> GetMinigamesList()
{
	ifstream myfile;
	int numGames;
	vector<string> minigames;

	myfile.open(__MINIGAMES_CONFIG_FILE__, ios::in);
	if (!myfile.is_open()) {
		cout << "Error oppening " << __MINIGAMES_CONFIG_FILE__ << " to write" << endl;
		exit(1);
	}
	myfile >> numGames;
	minigames.resize(numGames);
	for (int i = 0; i < numGames; i++)
	{
		myfile >> minigames[i];
	}

	return minigames;
}

/* The registered user accepts the 'friendUser' as a friend */
void SendRequest(wstring user, wstring friendUser)
{
	wstring url = L"http://uvm061.dei.isep.ipp.pt:5000/accept_response?personA=" + user + L"&personB=" + friendUser;
	RequestJSONValueAsync(url).get();
}

/* Retrieves the friendships requests from the PROLOG WS */
vector<wstring> GetFriendRequests(wstring user)
{
	vector<wstring> users;

	wstring userWs(user.begin(), user.end());

	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/check_requests?user=" + userWs;

	json::value usersJs = RequestJSONValueAsync(url).get();
	wstring status = usersJs[L"status"].to_string();

	if (checkIfStatusOk(status))
	{
		users.resize(usersJs[L"users"].size());

		int i = 0;
		for (auto iter = usersJs[L"users"].begin(); iter != usersJs[L"users"].end(); ++iter)
		{
			users[i] = iter->second.as_string();
			i++;
		}
	}
	else
	{
		users.resize(0);
	}

	return users;
}

/* Retrieves the game requests from the PROLOG WS */
vector<wstring> GetFriendGameRequests(wstring user)
{
	vector<wstring> users;

	wstring userWs(user.begin(), user.end());

	utility::string_t url = L"http://uvm061.dei.isep.ipp.pt:5000/check_game_requests?user=" + userWs;

	json::value usersJs = RequestJSONValueAsync(url).get();
	wstring status = usersJs[L"status"].to_string();

	if (checkIfStatusOk(status))
	{
		users.resize(usersJs[L"users"].size());

		int i = 0;
		for (auto iter = usersJs[L"users"].begin(); iter != usersJs[L"users"].end(); ++iter)
		{
			users[i] = iter->second.as_string();
			i++;
		}
	}
	else
	{
		users.resize(0);
	}

	return users;
}

/* Checks the current game version in the Windows WS */
bool CheckGameVersion(string gameversion)
{
	wstring gameVersionWS(gameversion.begin(), gameversion.end());

	utility::string_t url = L"http://wvm061.dei.isep.ipp.pt/DatabaseWs/Database.svc/version";

	json::value versionJS = RequestJSONValueAsync(url).get();
	wstring status = versionJS[L"Msg"].to_string();
	gameVersionWS = L"\"" + gameVersionWS + L"\"";

	return status.compare(gameVersionWS) == 0;
}

/* Removes/Denies the friendship request from the PROLOG WS */
void RemoveRequest(wstring user, wstring friendUser)
{
	wstring url = L"http://uvm061.dei.isep.ipp.pt:5000/remove_request?personA=" + user + L"&personB=" + friendUser;
	RequestJSONValueAsync(url).get();
}

/* Accepts the friendship request from the PROLOG WS */
void AcceptRequest(wstring user, wstring friendUser)
{
	wstring url = L"http://uvm061.dei.isep.ipp.pt:5000/accept_response?personA=" + user + L"&personB=" + friendUser;
	RequestJSONValueAsync(url).get();
}

/* Sends the game request to the PROLOG WS */
void SendGameRequest(wstring user, wstring friendUser)
{
	wstring url = L"http://uvm061.dei.isep.ipp.pt:5000/game_response?personA=" + user + L"&personB=" + friendUser;
	RequestJSONValueAsync(url).get();
}