#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

int main()
{
	map<int, int> map;

	map[0] = map.size();
	map[1] = map.size();

	cout << map[01] << endl;

	return 0;
}
