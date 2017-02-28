// Clustering1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Graph.h"
#include <vector>

using namespace std;

int main()
{
	if (!freopen("input/trueInput.txt", "r", stdin)) {
		cout << "Problem z plikiem" << endl;
		return (1);
	}

	Graph g;
	int vertsNum;
	cin >> vertsNum;

	vector<int> verts;
	for (int i = 1; i <= vertsNum; i++)
		verts.push_back(i);
	g.batchAddVertex(verts);

	int from, to, len;
	while (cin >> from >> to >> len) {
		g.addEdge(from, to, len);
	}

	g.cluster(4);
	


    return 0;
}

