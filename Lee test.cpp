#include "lee.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	ifstream results("results.txt");
	ofstream frequency("frequency.txt", ios_base::trunc);
	vector < double > res;
	double num;
	while (results >> num)
		res.push_back(num);


	int interval = 50;
	double min = 0, max = 1;
	double step = 1.0 * (max - min) / interval;
	cout << step << endl;
	vector<int> count(interval);
	for (auto& r : res)
	{
		int ratio = (r - min) / step;
		if (ratio != interval)
			count[ratio]++;
		else
			count[ratio - 1]++;
	}
	for (int i = 0; i < count.size(); ++i)
	{
		frequency << 1. * count[i] / res.size() << '\n';
	}
	frequency << "---\n";
	for (int i = 0; i < count.size(); ++i)
		frequency << i * step << '\n';

	/*Lee test;
	int width = 80;
	int length = 90;
	int testNum = 10000;
	ofstream results("results.txt", ios_base::trunc);

	vector<int> res = test.GenerateTask(width, length, testNum, true);

	for (auto& elem : res)
		results << elem << "\n";*/
	results.close();
	return 0;
}