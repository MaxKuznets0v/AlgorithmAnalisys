#include "lee.h"
#include <iostream>
using namespace std;

int main()
{
	Lee task = Lee(10, 5);
	auto res = task.Findpath({ 7, 0 }, { 5, 4 });
	cout << res.second << endl;
	system("pause");
	return 0;
}