#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "Heap.h"
#include "Matrix.h"

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::unordered_set;

class Edge;

class Vertex {
public:
	int id;
	bool visited = false;
	int minDistance = INT_MAX;
	int pValue = INT_MAX;
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

	void calculateDistancesUsingDijkstra(int startIndex) {
		Heap<Vertex*> unvisitedVertices;
		Vertex * vStart = vertices[startIndex - 1];
		vStart->minDistance = 0;
		unvisitedVertices.insert(vStart, vStart->minDistance);

		while (unvisitedVertices.size()) {
			Vertex * current = unvisitedVertices.extractMin().obj;
			unvisitedVertices.popMin();
			if (current->visited)
				continue;

			current->visited = true;

			for (Edge * e : current->outputs) {
				Vertex* vTarget = e->to;
				if (!vTarget->visited) {
					int newDist = current->minDistance + e->length;
					if (newDist < vTarget->minDistance) {
						vTarget->minDistance = newDist;
						unvisitedVertices.insert(vTarget, newDist);
					}
				}
			}

		}
	}

	void resetDistances() {
		for (Vertex* v : vertices) {
			v->minDistance = INT_MAX;
			v->visited = false;
		}
	}

	void printDistances() {
		for (Vertex* v : vertices)
			cout << v->id << ": " << v->minDistance << endl;
		cout << endl;
	}

	void runJohnsons(bool fullOutput) {
		preparePValues();
		reweightEdges();
		
		long long minimalDist = LONG_MAX;

		for (int i = 1; i <= numVertices; i++) {
			resetDistances();
			calculateDistancesUsingDijkstra(i);
			if (fullOutput)
				cout << "Distances from " << i << ":" << endl;

			for (Vertex* v : vertices) {
				long long realDist = v->minDistance == INT_MAX? INT_MAX : v->minDistance - vertices[i - 1]->pValue + v->pValue;
				if (i == v->id && realDist < 0) {
					cout << "Negative cycle detected";
				}
				if (realDist < minimalDist){
					minimalDist = realDist;
				}

				if (fullOutput)
					cout << "to " << v->id << ": " << realDist << endl;
			}
		}
		cout << "Minimal dist: " << minimalDist << endl;

	}


private:
	vector<Vertex*> vertices;
	vector<Vertex*> memoryPointersToVerts;
	unordered_set<Edge*> memoryPointersToEdges;
	int numVertices = 0;

	void preparePValues() {
		int n = numVertices;
		Matrix<long long> dist(n+2,n);

		//base case, all vertices are connected to S via 0 cost path
		for (int i = 0; i < n; i++)
			dist.at(0, i) = 0;

		for (int k = 1; k <= n+1; k++) {
			for (int i = 0; i < n; i++) {
				long long minimalDist = LONG_MAX;

				Vertex* v = vertices[i];
				long long previousDist = dist.at(k - 1, i);
				minimalDist = previousDist;

				for (Edge* e : v->inputs) {
					Vertex* possibleVertexOnPath = e->from;
					long long newPossibleDist = dist.at(k - 1, possibleVertexOnPath->id - 1) + e->length;
					if (newPossibleDist < minimalDist) {
						minimalDist = newPossibleDist;
					}
				}

				dist.at(k, i) = minimalDist;

			}
		}

		for (int i = 0; i < n; i++) {
			if (dist.at(n, i) != dist.at(n + 1, i)) {
				cout << "Negative cycle detected" << endl;
				break;
			}
		}

		for (int i = 0; i < n; i++)
			vertices[i]->pValue = dist.at(n, i);

		
	}

	void reweightEdges() {
		for (Edge* e : memoryPointersToEdges) {
			e->length += e->from->pValue - e->to->pValue;
		}
	}
	
	//void addStartingVertex() {
	//	int startId = numVertices + 1;
	//	addVertex(numVertices + 1);

	//	for (Vertex *v : vertices)
	//		if (v->id != startId)
	//			addEdge(startId, v->id, 0);
	//}


};
