#include "graphs.h"
#include <iostream>
#include <fstream>
#include "User.h"


#define __GRAFO__FILE__ "example.graph"

Node nodes[_MAX_NODES_GRAPH];
Path paths[_MAX_PATHS_GRAPH];
int numNodes = 0, numPaths = 0;
User users[_MAX_NODES_GRAPH];

using namespace std;

//Nodes

void addNode(Node node){
	if (numNodes<_MAX_NODES_GRAPH){
		nodes[numNodes] = node;
		numNodes++;
	}
	else{
		cout << "Number of nodes reached limits!" << endl;
	}
}

void deleteNode(int indNode){
	if (indNode >= 0 && indNode<numNodes){
		for (int i = indNode; i<numNodes; nodes[i++] = nodes[i + i]);
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
	for (int i = 0; i<numNodes; printNode(nodes[i++]));
}

Node createNode(float x, float y, float z, int user){
	Node node;
	node.x = x;
	node.y = y;
	node.z = z;
	node.user = user;
	return node;
}

void addPath(Path path){
	if (numPaths<_MAX_PATHS_GRAPH){
		paths[numPaths] = path;
		numPaths++;
	}
	else{
		cout << "Number of paths reached limit!" << endl;
	}
}

//Path

void deletePath(int indPath){
	if (indPath >= 0 && indPath<numPaths){
		for (int i = indPath; i<numPaths; paths[i++] = paths[i + i]);
		numPaths--;
	}
	else{
		cout << "Invalid path index." << endl;
	}
}

void printPath(Path path){
	cout << "Start node:" << path.nodei << "End node:" << path.nodef << "Weight:" << path.weight << "Width:" << path.width << endl;
}

void listPaths(){
	for (int i = 0; i<numPaths; printPath(paths[i++]));
}

Path createPath(int nodei, int nodef, float weight, float width){
	Path path;
	path.nodei = nodei;
	path.nodef = nodef;
	path.weight = weight;
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
	for (int i = 0; i<numNodes; i++)
		myfile << nodes[i].x << " " << nodes[i].y << " " << nodes[i].z << endl;
	myfile << numPaths << endl;
	for (int i = 0; i<numPaths; i++)
		myfile << paths[i].nodei << " " << paths[i].nodef << " " << paths[i].weight << " " << paths[i].width << endl;
	myfile.close();
}

void readGraph(){
	ifstream myfile;

	myfile.open(__GRAFO__FILE__, ios::in);
	if (!myfile.is_open()) {
		cout << "Error oppening " << __GRAFO__FILE__ << " to write" << endl;
		exit(1);
	}
	myfile >> numNodes;
	for (int i = 0; i<numNodes; i++)
		myfile >> nodes[i].x >> nodes[i].y >> nodes[i].z;
	myfile >> numPaths;
	for (int i = 0; i<numPaths; i++)
		myfile >> paths[i].nodei >> paths[i].nodef >> paths[i].weight >> paths[i].width;
	myfile.close();
	// calculates the width of each node = bigger width of the paths that diverge/converge from/to that node	
	for (int i = 0; i<numNodes; i++){
		nodes[i].width = 0;
		for (int j = 0; j<numPaths; j++)
		if ((paths[j].nodei == i || paths[j].nodef == i) && nodes[i].width < paths[j].width)
			nodes[i].width = paths[j].width;
	}
}