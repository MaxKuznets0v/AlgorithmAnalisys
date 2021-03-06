#pragma once
#include <utility>
#include <vector>
#include <iostream>
#include <iomanip>

// ������ �������� - �������, ������ - �������
typedef std::pair<int, int> Coord;

class Lee
{
public:
	Lee();
	Lee(int, int);
	void Print() const
	{
		for (int i = 0; i < _width; ++i)
		{
			for (int j = 0; j < _length; ++j)
			{
				switch (map[i][j])
				{
				case 0:
					std::cout << std::setw(2) << " " << " ";
					break;
				case -1:
					std::cout << std::setw(2) << "#" << " ";
					break;
				default:
					std::cout << std::setw(2) << map[i][j] << " ";
					break;
				}
			}
			std::cout << std::endl;
		}
	}
	std::pair<std::vector<Coord>, int> Findpath(Coord start, Coord dest);
	std::vector<int> GenerateTask(int width, int length, int num, bool onlyExists);
private:
	// ����� � ������ ���������� �����
	int _width;
	int _length;
	// ����� � �������������� ���������� ������ ��������� - ������� ����� ����
	std::vector<std::vector<int>> map;

	void MakeMap();
	int Forward(Coord start, Coord dest);
	std::vector<Coord> Backward(Coord start, Coord dest) const;
	std::vector<Coord> GetNeighbors(Coord) const;
	Coord NextNeighbor(Coord) const;
	bool DeadEnd(int x, int y) const;
	void CleanUp(bool full = false);
	void GenerateObstacles(double ratio);
	void GenerateMaze(Coord left_upper, Coord right_down);
	void GenerateMazeNonRec();
	std::pair<Coord, Coord> GenerateCoords() const;
	
};