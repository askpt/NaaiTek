// Simple Maze Generator in C++ by Jakub Debski '2006 
// http://www.roguebasin.com/index.php?title=Simple_maze

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <vector>
#include <list>
#include <iostream>
#include <SWI-cpp.h>
using namespace std;

vector<vector<int>> GenerateMaze(int x, int y)
{
	srand(time(0));

	const int maze_size_x = x;
	const int maze_size_y = y;
	vector < vector < int > > maze;
	list < pair < int, int> > drillers;

	maze.resize(maze_size_y);
	for (size_t y = 0; y < maze_size_y; y++)
		maze[y].resize(maze_size_x);

	for (size_t x = 0; x < maze_size_x; x++)
	for (size_t y = 0; y<maze_size_y; y++)
		maze[y][x] = 0;

	drillers.push_back(make_pair(maze_size_x / 2, maze_size_y / 2));
	while (drillers.size()>0)
	{
		list < pair < int, int> >::iterator m, _m, temp;
		m = drillers.begin();
		_m = drillers.end();
		while (m != _m)
		{
			bool remove_driller = false;
			switch (rand() % 4)
			{
			case 0:
				(*m).second -= 2;
				if ((*m).second < 0 || maze[(*m).second][(*m).first])
				{
					remove_driller = true;
					break;
				}
				maze[(*m).second + 1][(*m).first] = 1;
				break;
			case 1:
				(*m).second += 2;
				if ((*m).second >= maze_size_y || maze[(*m).second][(*m).first])
				{
					remove_driller = true;
					break;
				}
				maze[(*m).second - 1][(*m).first] = 1;
				break;
			case 2:
				(*m).first -= 2;
				if ((*m).first < 0 || maze[(*m).second][(*m).first])
				{
					remove_driller = true;
					break;
				}
				maze[(*m).second][(*m).first + 1] = 1;
				break;
			case 3:
				(*m).first += 2;
				if ((*m).first >= maze_size_x || maze[(*m).second][(*m).first])
				{
					remove_driller = true;
					break;
				}
				maze[(*m).second][(*m).first - 1] = 1;
				break;
			}
			if (remove_driller)
				m = drillers.erase(m);
			else
			{
				drillers.push_back(make_pair((*m).first, (*m).second));
				// uncomment the line below to make the maze easier 
				// if (rand()%2) 
				drillers.push_back(make_pair((*m).first, (*m).second));

				maze[(*m).second][(*m).first] = 1;
				++m;
			}
		}
	}

	return maze;
}

vector<vector<int>> GetPath(vector<vector<int>> maze, int size_x, int size_y, double x_ini, double y_ini, double x_end, double y_end)
{
	char* argv[] = { "swipl.dll", "-s", "mz_off.pl", NULL };

	PlEngine e(3, argv);

	for (size_t x = 0; x < size_x; x++)
	{
		for (size_t y = 0; y < size_y; y++)
		{
			if (maze[x][y] == 1)
			{
				//PlFrame fr;
				PlTermv av(1);

				double cx = x, cy = y;

				av[0] = PlCompound("pos", PlTermv(PlTerm(cx), PlTerm(cy)));
				PlQuery q("assert", av);
				q.next_solution();
			}
		}
	}
	
	PlTermv av1(6);

	av1[2] = PlTerm(x_ini);
	av1[3] = PlTerm(y_ini);
	av1[4] = PlTerm(x_end);
	av1[5] = PlTerm(y_end);
	
	PlQuery qq("eachPosMazeSol", av1);
	while (qq.next_solution())
	{
		
		int x = atoi((char*)av1[0]);
		int y = atoi((char*)av1[1]);

		maze[x][y] = 2;
	}

	/*
	char *path;
	PlTermv av2(1);
	PlQuery qq1("findPath", av2);
	while (qq1.next_solution())
	{
	cout << ((char*)av2[0]) << endl;
	}
	*/

	//cout << path << endl;

	return maze;
}

void PrintMaze(vector<vector<int>> maze, int maze_size_x, int maze_size_y)
{
	for (size_t x = 0; x < maze_size_y; x++)
	{
		for (size_t y = 0; y < maze_size_x; y++)
		{
			if (maze[x][y] == 1)
				printf(" ");
			else if (maze[x][y] == 0)
				printf("#");
			else if (maze[x][y] == 2)
				printf(".");
		}
		printf("\n");
	}
}

int main()
{
	int maze_size_x = 20;
	int maze_size_y = 20;
	vector<vector<int>> maze = GenerateMaze(maze_size_x, maze_size_y);

	PrintMaze(maze, maze_size_x, maze_size_y);
	cin.get();

	double x_ini, y_ini, x_end, y_end;

	cout << "X ini" << endl;
	cin >> x_ini;
	cout << "Y ini" << endl;
	cin >> y_ini;
	cout << "X end" << endl;
	cin >> x_end;
	cout << "y end" << endl;
	cin >> y_end;

	maze = GetPath(maze, maze_size_x, maze_size_y, x_ini, y_ini, x_end, y_end);

	PrintMaze(maze, maze_size_x, maze_size_y);
	
	cin.get();
	cin.get();

	return 0;
}