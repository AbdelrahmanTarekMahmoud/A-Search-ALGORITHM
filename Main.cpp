
//CellSort() - sorts the open list according to the sum of h + g
//ExpandNeighbors() - loops through the current node's neighbors and calls appropriate functions to add neighbors to the open list
//CheckValidCell() - ensures that the potential neighbor coordinates are on the grid and that the cell is open
//Heuristic() - computes the distance to the goal
//AddToOpen() - adds the node to the open list and marks the grid cell as closed
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using std::sort;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;

enum class state { kEmpty, kObstacle, kClosed, kPath ,kStart,kFinish};
const int delta[4][2]{ {-1, 0}, {0, -1}, {1, 0}, {0, 1} }; // directional deltas, up,left,down,right

vector<state> parseline(string line)
{
	istringstream stream(line);
	int n;
	char c;
	vector<state> v;
	while (stream >> n >> c && c == ',')
	{
		if (n == 0)
		{
			v.push_back(state::kEmpty);
		}
		else
		{
			v.push_back(state::kObstacle);
		}
	}
	return v;
}
vector<vector<state>> read(string path)
{
	ifstream file(path);
	vector<vector<state>> v;
	if (file)
	{
		string line;
		while (getline(file, line)) 
		{
		vector<state>v2 = parseline(line);
		v.push_back(v2);
		}
	}
return v;
}

bool Compare(const vector<int> a, const vector<int> b)
{
	int f1 = a[2] + a[3]; // f1 = g1 + h1
	int f2 = b[2] + b[3]; // f2 = g2 + h2
	return f1 > f2;
}

void cellSort(vector<vector<int>>* v)
{
	sort(v->begin(), v->end(), Compare);
}


int hueristic(int x1, int y1, int x2, int y2)
{
	return (abs(x2 - x1) + abs(y2 - y1));
}

bool CheckValidCell(int x, int y, vector<vector<state>>& grid)
{
  bool on_grid_x = (x >= 0 && x < grid.size());
  bool on_grid_y = (y >= 0 && y < grid[0].size());
  if (on_grid_x && on_grid_y)
    return grid[x][y] == state::kEmpty;
  return false;
}


void AddToOpen(int x, int y, int g, int h , vector<vector<int>> &openlist , vector<vector<state>>&grid)
{
	openlist.push_back(vector<int>{x, y, g, h});
	grid[x][y] = state::kClosed;


}

void ExpandNeighbors(vector<int> &currnode ,int *goal, vector<vector<int>> &open ,vector<vector<state>> &grid)
{
	int x = currnode[0];
	int y = currnode[1];
	int g = currnode[2];
	int h = currnode[3];
	for (int i = 0;i < 4;i++)
	{
		int potential_x = x + delta[i][0];
		int potential_y = y + delta[i][1];
		if (CheckValidCell(potential_x, potential_y, grid))
		{
			int new_h = hueristic(potential_x, potential_y, goal[0], goal[1]);
			int g2 = g + 1;
			AddToOpen(potential_x, potential_y, g2, new_h, open, grid);
		}
	}
}

vector<vector<state>> search(vector<vector<state>> grid, int init[2], int goal[2])
{

		vector<vector<int>> open{}; //empty vector
		int x = init[0];
		int y = init[1];
		int g = 0;
		int h = hueristic(x, y, goal[0], goal[1]);
		AddToOpen(x, y, g, h, open, grid);
		while (open.size() > 0)
		{
			cellSort(&open);
			auto current = open.back();
			open.pop_back();
			x = current[0];
			y = current[1];
			grid[x][y] = state::kPath;
			if (x == goal[0] && y == goal[1])
			{
				grid[init[0]][init[1]] = state::kStart;
				grid[goal[0]][goal[1]] = state::kFinish;
				return grid;
			}
			ExpandNeighbors(current, goal, open, grid);
		}
		cout << "No Path Found" << "\n";
		return vector<vector<state>>{};
}
string CellString(state cell) {
	switch (cell) {
	case state::kObstacle: return "O    ";
	case state::kPath: return "P    ";
	case state::kStart: return "S    ";
	case state::kFinish: return "F    ";
	default: return "0    ";
	}
}


void PrintBoard(const vector<vector<state>> board) 
{
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {
			cout<< CellString(board[i][j]);
		}
		cout << "\n";
	}
}



int main()
{
	
	int init_point[2]={ 0,0 };
	int goal_point[2]={ 4,5 };
	auto board = read("1.board.txt");
	auto solution = search(board, init_point, goal_point);
	PrintBoard(solution);
}

