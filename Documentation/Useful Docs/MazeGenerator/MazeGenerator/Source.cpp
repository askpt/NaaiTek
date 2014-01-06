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

vector<vector<int>> GetPath(vector<vector<int>> maze, int size_x, int size_y)
{
	char* argv[] = { "swipl.dll", "-s", "mz_off.pl", NULL };

	PlEngine e(3, argv);

	for (size_t x = 0; x < size_x; x++)
	{
		for (size_t y = 0; y < size_y; y++)
		{
			if (maze[x][y] == 1)
			{
				PlFrame fr;
				PlTermv av(1);

				char cx[20], cy[20];
				_itoa(x, cx, 10);
				_itoa(y, cy, 10);

				av[0] = PlCompound("pos", PlTermv(PlTerm(cx), PlTerm(cy)));
				PlQuery q("assert", av);
				q.next_solution();
			}
		}
	}
	
	PlTermv av1(1);
	PlQuery qq("findPath", av1);
	while (qq.next_solution())
	{
		cout << (char*)av1[0] << endl;
	}

	cin.get();

	return maze;
}


int main()
{
	int maze_size_x = 20;
	int maze_size_y = 20;
	vector<vector<int>> maze = GenerateMaze(maze_size_x, maze_size_y);

	maze = GetPath(maze, maze_size_x, maze_size_y);
	// Done 
	for (size_t x = 0; x < maze_size_y; x++)
	{
		for (size_t y = 0; y < maze_size_x; y++)
		{
			if (maze[x][y] == 1)
				printf(" ");
			else if (maze[x][y] == 0)
				printf("#");
			else if (maze[x][y] == 1)
				printf(".");
		}
		printf("\n");
	}

	cin.get();
	
	

	return 0;
}