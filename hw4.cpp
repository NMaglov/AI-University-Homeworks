#include <iostream>
const int SIZE = 3;
void print(char grid[SIZE][SIZE])
{
	for(int i = 0; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			std::cout << grid[i][j];
		}
		std::cout << "\n";
	}
}
int winner(char grid[SIZE][SIZE])
{
	for(int i = 0; i < SIZE; i++)
	{
		if(grid[i][0] != '_' && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
		{
			if(grid[i][0] == 'X')
			{
				return 1;
			}
			return -1;
		}
		if(grid[0][i] != '_' && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
		{
			if(grid[0][i] == 'X')
			{
				return 1;
			}
			return -1;
		}
	}
	if(grid[1][1] != '_' && ((grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) || (grid[2][0] == grid[1][1] && grid[1][1] == grid[0][2])))
	{
		if(grid[1][1] == 'X')
		{
			return 1;
		}
		return -1;
	}
	return 0;
}

std::pair<int, int> max(std::pair<int, int> a, std::pair<int, int> b)
{
	if(a.first > b.first)
	{
		return a;
	}
	if(a.first < b.first)
	{
		return b;
	}
	if(a.second < b.second)
	{
		return a;
	}
	return b;
}
std::pair<int, int> min(std::pair<int, int> a, std::pair<int, int> b)
{
	if(a.first < b.first)
	{
		return a;
	}
	if(a.first > b.first)
	{
		return b;
	}
	if(a.second < b.second)
	{
		return a;
	}
	return b;
}
std::pair<int, int> minMaxValue(char grid[SIZE][SIZE], int depth, bool maxi, int alpha, int beta)
{
	int w = winner(grid);
	if(w != 0)
	{
		return {w, depth};
	}
	std::pair<int, int> v = {2, depth};
	if(maxi)
	{
		v.first = -2;
	}
	for(int i = 0; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			if(grid[i][j] == '_')
			{
				if(maxi)
				{
					grid[i][j] = 'X';
					v = max(v, minMaxValue(grid, depth + 1, false, alpha, beta));
					grid[i][j] = '_';
					if(alpha >= beta)
					{
						return v;
					}
					alpha = std::max(alpha, v.first);
				}
				else
				{
					grid[i][j] = 'O';
					v = min(v, minMaxValue(grid, depth + 1, true, alpha, beta));
					grid[i][j] = '_';
					if(alpha >= beta)
					{
						return v;
					}
					beta = std::min(beta, v.first);
				}

			}
		}
	}
	if(std::abs(v.first) > 1)
	{
		v.first = 0;
	}
	return v;
}
void miniMaxDecision(char grid[SIZE][SIZE], bool maxi)
{
	int x, y, v = 2, depth = 1e9;
	if(maxi)
	{
		v = -2;
	}
	for(int i = 0; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			if(grid[i][j] == '_')
			{
				if(maxi)
				{
					grid[i][j] = 'X';
					std::pair<int, int> cur = minMaxValue(grid, 0, false, -2, 2);
					if(cur.first > v || (cur.first == v && cur.second < depth))
					{
						v = cur.first;
						depth = cur.second;
						x = i;
						y = j;
					}
				}
				else
				{
					grid[i][j] = 'O';
					std::pair<int, int> cur = minMaxValue(grid, 0, true, -2, 2);
					if(cur.first < v || (cur.first == v && cur.second < depth))
					{
						v = cur.first;
						depth = cur.second;
						x = i;
						y = j;
					}
				}
				grid[i][j] = '_';
			}
		}
	}
	if(maxi)
	{
		grid[x][y] = 'X';
	}
	else
	{
		grid[x][y] = 'O';
	}
}

int main()
{
	bool first;
	std::cout << "Do you start first?y/n\n";
	char c;
	std::cin >> c;
	if(c == 'y')
	{
		first = true;
	}
	else
	{
		first = false;
	}
	char grid[SIZE][SIZE];
	for(int i = 0; i < SIZE; i++)
	{
		for(int j = 0; j < SIZE; j++)
		{
			grid[i][j] = '_';
		}
	}

	int x, y;
	for(int i = 0; i < SIZE * SIZE; i++)
	{
		if((i % 2 == 0 && first) || (i % 2 == 1 && !first))
		{
			std::cin >> x >> y;
			x--;
			y--;
			if(first)
			{
				grid[x][y] = 'X';
			}
			else
			{
				grid[x][y] = 'O';
			}
		}
		else
		{
			miniMaxDecision(grid, !first);
		}
		print(grid);
		if(winner(grid) != 0)
		{
			break;
		}
	}
	return 0;
}
