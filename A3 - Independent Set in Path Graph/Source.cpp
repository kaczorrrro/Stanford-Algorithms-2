#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
	int numVertices;
	cout << "Enter num vertices" << endl;
	cin >> numVertices;

	cout << "Enter weights" << endl;
	vector<int> weights;
	weights.push_back(0);
	int temp;
	for (int i = 0; i < numVertices; i++) {
		cin >> temp;
		weights.push_back(temp);
	}

	vector<int> optimalSolutions;
	optimalSolutions.resize(numVertices+1);
	optimalSolutions[0] = 0;
	optimalSolutions[1] = weights[1];
	for (int i = 2; i <= numVertices; i++) {
		optimalSolutions[i] = max(optimalSolutions[i - 1], optimalSolutions[i - 2] + weights[i]);
	}
	cout << optimalSolutions[numVertices] << endl;

	for(int vertexTraverse = numVertices; vertexTraverse > 0; vertexTraverse--){
		int tempOptimal = optimalSolutions[vertexTraverse];
		if (tempOptimal == optimalSolutions[vertexTraverse - 1])
			continue;
		cout << vertexTraverse << " ";
		vertexTraverse--;
	}

	cout << endl;

	return 0;
}