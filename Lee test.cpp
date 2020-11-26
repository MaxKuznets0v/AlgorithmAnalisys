#include "lee.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void CountFreq(int interval)
{
	ifstream results("results.txt");
	ofstream frequency("frequency.txt", ios_base::trunc);
	vector < double > res;
	double num;
	while (results >> num)
		res.push_back(num);

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
	results.close();
	frequency.close();
}

int main()
{
	/*CountFreq(40);
	return 0;*/
	Lee test;
	int width = 10;
	int length = 10;
	int step = 2;
	int testNum = 3000;
	ofstream results("results.txt", ios_base::app);

	vector<int> res;
	for (int i = 21; i < 30; ++i)
	{
		res = test.GenerateTask(width + i * step, length + i * step, testNum, true);
		results << "Size: " << width + i * step << "x" << length + i * step << endl;
		for (auto& elem : res)
			results << elem << "\n";
		results << "------\n";
	}

	return 0;
}