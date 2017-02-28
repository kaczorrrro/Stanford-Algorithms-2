#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "Heap.h"

using namespace std;

class Edge;


class Vertex {
public:
	int id;
	bool visited = false;
	int mstDistance = INT_MAX;
	vector<Edge*> inputs, outputs;
};

class Edge {
public:
	int length;
	Vertex *from, *to;
};


class Graph {
public:
	void addVertex(int i) {
		batchAddVertex(vector<int> { i });
	}

	void batchAddVertex(const vector<int> & ids) {
		int size = ids.size();
		if (size <= 0)
			return;
		Vertex* verts = new Vertex[size];
		memoryPointersToVerts.push_back(verts);

		for (int i = 0; i < size; i++) {
			Vertex* v = verts + i;
			v->id = ids[i];
			vertices.push_back(v);
		}
		numVertices += size;
	}

	void addEdge(int fromId, int toId, int len) {
		Vertex * from = vertices[fromId - 1];
		Vertex * to = vertices[toId - 1];
		Edge * e = new Edge{ len, from, to };
		memoryPointersToEdges.insert(e);
		from->outputs.push_back(e);
		to->inputs.push_back(e);
	}

	void printMe() {
		for (Vertex* v : vertices) {
			cout << "Vertex " << v->id << endl;
			cout << "Inputs: ";
			for (Edge* e : v->inputs)
				cout << e->from->id << " ";
			cout << endl;

			cout << "Outputs: ";
			for (Edge* e : v->outputs)
				cout << e->to->id << " " << " len: " << e->length << "\t";
			cout << endl << endl;
		}
	}

	Graph() {};

	~Graph() {
		for (Edge* e : memoryPointersToEdges) delete e;
		for (Vertex* v : memoryPointersToVerts) delete[] v;
	}

	void calculateMST() {
		Heap<Vertex*> vertsHeap;
		vertsHeap.insert(Heap<Vertex*>::Entry{ 0, vertices[0] });

		while (vertsHeap.size()) {
			auto heapEntry = vertsHeap.extractMin();
			vertsHeap.popMin();
			Vertex* v = heapEntry.obj;
			if (v->visited)
				continue;

			v->visited = true;
			v->mstDistance = heapEntry.key;
			for (Edge * e : v->outputs) {
				if (!e->to->visited)
					vertsHeap.insert(Heap<Vertex*>::Entry{e->length, e->to});
			}
		}
	
	}
	
	void printSumOfMstDistances() {
		long long sumOfLengths = 0;
		for (Vertex * v : vertices)
			sumOfLengths += v->mstDistance;
		cout << sumOfLengths << endl;
	}



private:
	vector<Vertex*> vertices;
	vector<Vertex*> memoryPointersToVerts;
	unordered_set<Edge*> memoryPointersToEdges;
	int numVertices = 0;

};



//int main(int argc, char* argv[]) {
//	int vertCount = 875714;
//	if (argc < 2) {
//		cout << "Assuming input size " << vertCount << endl;
//	}
//	else {
//		vertCount = atoi(argv[1]);
//	}
//
//
//	Graph g;
//
//	//std::ios_base::sync_with_stdio(false);
//
//	if (!freopen("trueInput.txt", "r", stdin)) {
//		cout << "Problem z plikiem" << endl;
//		return -1;
//	}
//
//
//	vector<int> inputVerts;
//	for (int i = 1; i <= vertCount; i++) {
//		inputVerts.push_back(i);
//	}
//	g.batchAddVertex(inputVerts);
//	cout << "Verts added" << endl;
//
//
//	long t1 = GetTickCount();
//	vector<pair<int, int>> inputEdges;
//	int from, to;
//	while (cin >> from && cin >> to) {
//		inputEdges.push_back(make_pair(from, to));
//		//g.addEdge(from, to); 3 seconds slower (33%)
//	}
//	cout << "Edges read" << endl;
//	g.batchAddEdge(inputEdges);
//	cout << "Edges added" << endl;
//	long t2 = GetTickCount();
//	cout << "Time " << t2 - t1 << endl;
//
//
//	g.DFS_loop(true);
//	cout << "First loop finished" << endl;
//	g.unvisitAll();
//	//g.printMe();
//	g.sortVerticesByFinishingTime();
//	cout << "Sorted" << endl;
//	//g.printMe();
//	g.DFS_loop(false);
//
//	auto sizes = g.getGroupSizes();
//	cout << endl;
//	for (int i = 0; i < 5; i++)
//		cout << sizes[i] << endl;
//
//
//
//}

//void batchAddEdge(vector<pair<int, int>> fromToPairs) {
//	int size = fromToPairs.size();
//	if (size == 0)
//		return;

//	Edge * edgeObjects = new Edge[size];
//	for (int i = 0; i < size; i++) {
//		Vertex * from = vertices[fromToPairs[i].first - 1];
//		Vertex * to = vertices[fromToPairs[i].second - 1];
//		Edge* e = edgeObjects + i;
//		e->from = from;
//		e->to = to;
//		from->outputs.push_back(e);
//		to->inputs.push_back(e);
//	}
//	memoryPointersToEdges.insert(edgeObjects);
//}