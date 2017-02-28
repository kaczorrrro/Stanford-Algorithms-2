#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Graph.h"
#include "Matrix.h"

using std::vector;
using std::stringstream;
using std::string;
using std::cin;
using std::cout;



int main(int argc, char* argv[]) {
	//if (!freopen("input_g3.txt", "r", stdin)) {
	//	cout << "Problem z plikiem" << endl;
	//	return -1;
	//}

	int vertCount;
	int edgesCount;
	cin >> vertCount >> edgesCount;

	Graph g;

	vector<int> inputVerts;
	for (int i = 1; i <= vertCount; i++) {
		inputVerts.push_back(i);
	}
	g.batchAddVertex(inputVerts);

	cout << vertCount << " verts added" << endl;

	int from, to, len;
	string s;
	while (cin >> from >> to >> len) {
		g.addEdge(from, to, len);
	}

	g.runJohnsons(true);

}