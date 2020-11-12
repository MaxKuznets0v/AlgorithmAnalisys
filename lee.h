#pragma once
#include <utility>
#include <vector>
#include <iostream>

// Первое значение - строчка, второе - столбец
typedef std::pair<int, int> Coord;

class Lee
{
public:
	Lee(int, int);
	void Print()
	{
		for (int i = 0; i < _width; ++i)
		{
			for (int j = 0; j < _length; ++j)
				std::cout << map[i][j] << " ";
			std::cout << std::endl;
		}
	}
	std::pair<std::vector<Coord>, int> Findpath(Coord start, Coord dest);
private:
	// Длина и ширина дискретной сетки
	mutable int _width;
	mutable int _length;
	// Сетка с целочисленными значениями начало координат - верхний левый угол
	std::vector<std::vector<int>> map;

	int Forward(Coord start, Coord dest);
	std::vector<Coord> Backward(Coord start, Coord dest) const;
	std::vector<Coord> GetNeighbors(Coord) const;
	Coord NextNeighbor(Coord) const;
	void CleanUp();
};