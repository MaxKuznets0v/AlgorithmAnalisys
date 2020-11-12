#include "lee.h"
#include <queue>
#include <algorithm>

using namespace std;

Lee::Lee(int width, int length) : _width(width), _length(length)
{
	for (int i = 0; i < width; ++i)
		map.push_back(vector<int>(length));
}

vector<Coord> Lee::GetNeighbors(Coord cell) const
{
	vector<Coord> res;

	// Проверяем, что соседние вершины не имеют метки
	if (cell.second - 1 >= 0 && !map[cell.first][cell.second - 1])
		res.push_back({ cell.first, cell.second - 1 });
	if (cell.second + 1 < _length && !map[cell.first][cell.second + 1])
		res.push_back({ cell.first, cell.second + 1 });
	if (cell.first - 1 >= 0 && !map[cell.first - 1][cell.second])
		res.push_back({ cell.first - 1, cell.second });
	if (cell.first + 1 < _width && !map[cell.first + 1][cell.second])
		res.push_back({ cell.first + 1, cell.second });
	
	return res;
}

int Lee::Forward(Coord start, Coord dest)
{
	// количество элементарных операций - посещенных вершин
	int operation = 0;
	// Очередь для обхода
	queue<Coord> curCoords;
	curCoords.push(start);

	// Расстояние от стратовой точки
	int distance = 0;
	map[start.first][start.second] = -1;
	while (!map[dest.first][dest.second] && !curCoords.empty())
	{
		Coord cur = curCoords.front();
		if (cur != start && map[cur.first][cur.second] != distance)
			distance++;
		
		// Получим соседние непосещенные вершины
		vector<Coord> neighbors = GetNeighbors(cur);

		for (Coord& neighbor : neighbors)
		{
			map[neighbor.first][neighbor.second] = distance + 1;
			curCoords.push(neighbor);
			operation++;
		}

		curCoords.pop();
	}
	map[start.first][start.second] = 0;
	return operation;
}

vector<Coord> Lee::Backward(Coord start, Coord dest) const
{
	int distance = map[dest.first][dest.second];
	vector<Coord> res(distance + 1);

	Coord cur = dest;
	while (distance)
	{
		res[distance] = cur;
		cur = NextNeighbor(cur);
		if (cur.first == -1)
			break;
		distance--;
	}
	if (distance == 1)
		res[0] = start;
	return res;
}

Coord Lee::NextNeighbor(Coord cur) const
{
	int dist = map[cur.first][cur.second];
	Coord res;
	if (cur.second - 1 >= 0 && map[cur.first][cur.second - 1] && map[cur.first][cur.second - 1] < dist)
		res = { cur.first, cur.second - 1 };
	else if (cur.second + 1 < _length && map[cur.first][cur.second + 1] && map[cur.first][cur.second + 1] < dist)
		res = { cur.first, cur.second + 1 };
	else if (cur.first - 1 >= 0 && map[cur.first - 1][cur.second] && map[cur.first - 1][cur.second] < dist)
		res = { cur.first - 1, cur.second };
	else if (cur.first + 1 < _width && map[cur.first + 1][cur.second] && map[cur.first + 1][cur.second] < dist)
		res = { cur.first + 1, cur.second };
	else
		res = { -1, 0 };
	return res;
}

pair<vector<Coord>, int> Lee::Findpath(Coord start, Coord dest)
{
	CleanUp();
	int op = Forward(start, dest);
	Print();
	vector<Coord> path = Backward(start, dest);
	CleanUp();

	// Возвращает сам путь и трудоемкость
	return { path, op + path.size() };
}

void Lee::CleanUp()
{
	// удалим метки расстояний со всех ячеек кроме препятствий
	for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _length; ++j)
			if (map[i][j] != -1)
				map[i][j] = 0;
}