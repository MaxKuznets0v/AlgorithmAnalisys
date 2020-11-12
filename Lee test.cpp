#include "lee.h"
#include <iostream>
using namespace std;

int main()
{
	Lee test;
	int testNum, width, length;
	width = 10;
	length = 15;
	testNum = 10;
	vector<int> res = test.GenerateTask(width, length, testNum);

	for (auto& elem : res)
		cout << elem << " ";

	cout << endl;
	return 0;
}