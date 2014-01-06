// Simple Maze Generator in C++ by Jakub Debski '2006
// http://www.roguebasin.com/index.php?title=Simple_maze

#include <time.h>
#include <vector>
#include <list>
#include <iostream>
using namespace std;


int maze()
{
	srand(time(0));
    
	const int maze_size_x = 20;
	const int maze_size_y = 20;
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
    
	// Done
	for (size_t y = 0; y < maze_size_y; y++)
	{
		for (size_t x = 0; x < maze_size_x; x++)
		{
			if (maze[y][x] == 1)
				printf(" ");
			else
				printf("#");
		}
		printf("\n");
	}
    
	cin.get();
    
	return 0;
}
