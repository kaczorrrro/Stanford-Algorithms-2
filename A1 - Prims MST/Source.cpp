#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>
#include "Graph.h"


using std::vector;
using std::stringstream;
using std::string;
using std::cin;
using std::cout;



int main(int argc, char* argv[]) {
	if (!freopen("input/trueInput.txt", "r", stdin)) {
		cout << "Problem z plikiem" << endl;
		return -1;
	}

	int vertCount;
	cin >> vertCount;
	int edgesCount;
	cin >> edgesCount;

	Graph g;
	vector<int> inputVerts;
	for (int i = 1; i <= vertCount; i++) {
		inputVerts.push_back(i);
	}
	g.batchAddVertex(inputVerts);
	cout << vertCount << " verts added" << endl;
	
	int from, to, len;
	while (cin >> from >> to >> len) {
		//graph in this problem in undirected
		g.addEdge(from, to, len);
		g.addEdge(to, from, len);
	}

	g.calculateMST();
	g.printSumOfMstDistances();



}