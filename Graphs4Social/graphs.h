#include "User.h"
#include "Connection.h"
#include <string>
#include "Json.h"

#ifndef _GRAPH_INCLUDE
#define _GRAPH_INCLUDE

#define _MAX_NODES_GRAPH 100
#define _MAX_PATHS_GRAPH 200

typedef struct Node{
	float x, y, z, width;
	User *user;
}Node;

typedef struct Path{
	float width;
	Connection connection;                                                    
}Path;

extern Node nodes[];
extern Path paths[];
extern int numNodes, numPaths;

void addNode(Node);
void deleteNode(int);
void printNode(Node);
void listNodes();
Node createNode(float, float, float, User);

void addPath(Path);
void deletePath(int);
void printPath(Path);
void listPaths();
Path createPath(int, int, float, float);

void saveGraph();
void readGraph();
void readCommonGraph(wstring, wstring);
void readGraphUser(std::string);

bool TryAuth(std::string, std::string);

void buildGraph(json::value, json::value, wstring);

#endif