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
		return { { -1, -1 }, dest };
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
	/*Print();
	cout << endl;*/
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

void Lee::GenerateMaze(Coord left_upper, Coord right_down)
{
	std::random_device rd;
	std::mt19937 rand(rd());
	int width = right_down.first - left_upper.first + 1;
	int length = right_down.second - left_upper.second + 1;

	if (width < 3 || length < 3)
		return;

	// точка пересечения линий
	Coord intersec = { left_upper.first + width / 2, left_upper.second + length / 2 };


	int widthMid = width / 2;
	int lengthMid = length / 2;

	// делим поле на 4 части
	for (int i = left_upper.first; i < right_down.first + 1; ++i)
		map[i][intersec.second] = -1;
	for (int i = left_upper.second; i < right_down.second + 1; ++i)
		map[intersec.first][i] = -1;
	enum Side
	{
		LEFT = 1,
		DOWN,
		RIGHT,
		UP
	};
	vector<int> sides = { LEFT, DOWN, RIGHT, UP };
	// выберем произвольные стороны
	shuffle(sides.begin(), sides.end(), rand);

	// делаем дыры
	for (int side : { sides[0], sides[1], sides[2] })
	{
		Coord hole;
		switch (side)
		{
		case LEFT:
			hole.first = intersec.first;
			hole.second = left_upper.second + rand() % lengthMid;

			/*do
			{
				hole.second = left_upper.second + rand() % lengthMid;
			} while (hole.second == left_upper.second + (intersec.second - left_upper.second - 1) / 2 && length != 3);*/
			break;
		case DOWN:
			hole.first = intersec.first + 1 + rand() % (right_down.first - intersec.first);
			/*do
			{
				hole.first = intersec.first + 1 + rand() % (right_down.first - intersec.first);
			} while (hole.first == intersec.first + 1 + (right_down.first - intersec.first - 1) / 2 && width != 3);*/
			hole.second = intersec.second;
			break;
		case RIGHT:
			hole.first = intersec.first;
			hole.second = intersec.second + 1 + rand() % (right_down.second - intersec.second);

			/*do
			{
				hole.second = intersec.second + 1 + rand() % (right_down.second - intersec.second);
			} while (hole.second == intersec.second + 1 + (right_down.second - intersec.second - 1) / 2 && length != 3);*/
			break;
		case UP:
			hole.first = left_upper.first + rand() % widthMid;
			/*do
			{
				hole.first = left_upper.first + rand() % widthMid;
			} while (hole.first == left_upper.first + (intersec.first - left_upper.first - 1) / 2 && width != 3);*/
			hole.second = intersec.second;
			break;
		}
		map[hole.first][hole.second] = 0;
	}
	// Рекурсивно строим в каждой ячейке
	GenerateMaze(left_upper, { intersec.first - 1, intersec.second - 1 });
	GenerateMaze({ left_upper.first, intersec.second + 1 }, { intersec.first - 1, right_down.second });
	GenerateMaze({ intersec.first + 1, left_upper.second }, { right_down.first, intersec.second - 1 });
	GenerateMaze({ intersec.first + 1, intersec.second + 1 }, right_down);
}

bool Lee::DeadEnd(int x, int y) const
{
	int a = 0;
	int pass = 0;

	if (x != 1) {
		if (map[y][x - 2] == pass)
			a += 1;
	}
	else a += 1;

	if (y != 1) {
		if (map[y - 2][x] == pass)
			a += 1;
	}
	else a += 1;

	if (x != _length - 2) {
		if (map[y][x + 2] == pass)
			a += 1;
	}
	else a += 1;

	if (y != _width - 2) {
		if (map[y + 2][x] == pass)
			a += 1;
	}
	else a += 1;

	if (a == 4)
		return 1;
	else
		return 0;
}

void Lee::GenerateMazeNonRec()
{
	std::random_device rd;
	std::mt19937 rand(rd());
	int x, y, side, count;
	int wall = -1, pass = 0;

	for (int i = 0; i < _width; i++)
		for (int j = 0; j < _length; j++)
			map[i][j] = wall;

	x = 3; y = 3; count = 0; 
	while (count < 10000) { 
		map[y][x] = pass; count++;
		while (true) { 
			side = rand() % 4; 
			switch (side) {  
			case 0: if (y != 1)
				if (map[y - 2][x] == wall) 
				{ // Вверх
					map[y - 1][x] = pass;
					map[y - 2][x] = pass;
					y -= 2;
				}
			case 1: if (y != _width - 2)
				if (map[y + 2][x] == wall) 
				{ // Вниз
					map[y + 1][x] = pass;
					map[y + 2][x] = pass;
					y += 2;
				}
			case 2: if (x != 1)
				if (map[y][x - 2] == wall) 
				{ // Налево
					map[y][x - 1] = pass;
					map[y][x - 2] = pass;
					x -= 2;
				}
			case 3: if (x != _length - 2)
				if (map[y][x + 2] == wall) 
				{ // Направо
					map[y][x + 1] = pass;
					map[y][x + 2] = pass;
					x += 2;
				}
			}
			if (DeadEnd(x, y))
				break;
		}

		if (DeadEnd(x, y)) // Вытаскиваем крота из тупика
			do 
			{
				x = 2 * (rand() % ((_length - 1) / 2)) + 1;
				y = 2 * (rand() % ((_width - 1) / 2)) + 1;
			} while (map[y][x] != pass);
	} // На этом и все.
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
	//Print();
	//double rto = 0.6 + rand() % 11 * 1.0 / 100;
	//double rto = 0.7;
	for (int i = 0; i < num; ++i)
	{

		cout << "Test number " << i + 1 << endl;
		//cout << "Ratio: " << rto << endl;
		//GenerateObstacles(rto);

		pair<Coord, Coord> coords = { { 0, 0 }, { _width - 1, _length - 1 } };
		//pair<Coord, Coord> coords = { { 1, 1 }, { _width - 2, _length - 2 } };
		//GenerateMazeNonRec();
		GenerateMaze({ 0, 0 }, { _width - 1, _length - 1 });
		//auto coords = GenerateCoords();
		/*cout << "Start: (" << coords.first.first << ", " << coords.first.second << ") Finish: (" <<
			coords.second.first << ", " << coords.second.second << ")" << endl;*/
		map[coords.first.first][coords.first.second] = 0;
		map[coords.second.first][coords.second.second] = 0;
		pair<vector<Coord>, int> res = Findpath(coords.first, coords.second);
		cout << res.second << "\n\n";
		CleanUp(true);
		if (onlyExisting && (res.first[0].first == -1))
		{
			--i;
			continue;
		}
		//rto = 0.6 + rand() % 11 * 1.0 / 100;
		results.push_back(res.second);
	}
	return results;
}