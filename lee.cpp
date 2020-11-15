#include "lee.h"
#include <queue>
#include <algorithm>
#include <random>
#include <iostream>

using namespace std;

Lee::Lee()
{
	_width = 0; 
	_length = 0;
}

Lee::Lee(int width, int length) : _width(width), _length(length)
{
	MakeMap();
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

	// Расстояние от стартовой точки
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
	// Если пути до конца нет
	if (!distance)
		return { start, dest };
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
	if (distance <= 1)
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
	if (start == dest)
		return { { start }, 1 };
	int op = Forward(start, dest);
	vector<Coord> path = Backward(start, dest);
	CleanUp();

	// Возвращает сам путь и трудоемкость
	return { path, op + path.size() - 1 };
}

void Lee::CleanUp(bool full)
{
	// удалим метки расстояний со всех ячеек кроме препятствий
	// full - удаляем и препятствия
	for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _length; ++j)
			if (map[i][j] == -1 || full)
				map[i][j] = 0;
}

void Lee::GenerateObstacles(double ratio)
{
	// Генерация преград (отмечаются -1 на карте)
	random_device rd;
	std::mt19937 rand(rd());
	for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _length; ++j)
		{
			double num = (rand() % 100) * 1. / 100;
			if (num > ratio)
				map[i][j] = -1;
		}
}

pair<Coord, Coord> Lee::GenerateCoords() const
{
	random_device rd;
	std::mt19937 rand(rd());
	int first = 0;
	int second = 0;

	do
	{
		first = rand() % _width;
		second = rand() % _length;
	} while (map[first][second]);

	Coord start = { first, second };

	do
	{
		first = rand() % _width;
		second = rand() % _length;
	} while (map[first][second]);

	Coord dest = { first, second };

	return { start, dest };
}

void Lee::MakeMap()
{
	vector<vector<int>> newMap;
	for (int i = 0; i < _width; ++i)
		newMap.push_back(vector<int>(_length));
	map = move(newMap);
}

vector<int> Lee::GenerateTask(int width, int length, int num, bool onlyExisting)
{
	random_device rd;
	std::mt19937 rand(rd());

	_width = width;
	_length = length;
	MakeMap();
	
	vector<int> results;
	//double rto = 0.6 + rand() % 21 * 1.0 / 100;
	double rto = 0.7;
	GenerateObstacles(rto);
	Print();
	for (int i = 0; i < num; ++i)
	{
		cout << "Test number " << i + 1 << endl;
		//cout << "Ratio: " << rto << endl;
		//pair<Coord, Coord> coords = { { 0, 0 }, { _width - 1, _length - 1 } };
		auto coords = GenerateCoords();
		cout << "Start: (" << coords.first.first << ", " << coords.first.second << ") Finish: (" <<
			coords.second.first << ", " << coords.second.second << ")" << endl;
		map[coords.first.first][coords.first.second] = 0;
		map[coords.second.first][coords.second.second] = 0;
		pair<vector<Coord>, int> res = Findpath(coords.first, coords.second);
		cout << res.second << "\n\n";
		CleanUp(true);
		if (onlyExisting && (res.first[0] != coords.first))
		{
			--i;
			continue;
		}
		//rto = 0.6 + rand() % 21 * 1.0 / 100;
		results.push_back(res.second);
	}
	return results;
}