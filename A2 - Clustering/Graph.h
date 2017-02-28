#pragma once

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <unordered_set>
#include <algorithm>


using namespace std;

class Edge;


class Vertex {
public:
	int id;
	Vertex* master;
	vector<Vertex*> groupMembers;
	int groupSize;
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
			v->master = v;
			v->groupSize = 1;
			v->groupMembers.push_back(v);
			vertices.push_back(v);
		}
		numVertices += size;
	}

	void addEdge(int fromId, int toId, int len) {
		Vertex * from = vertices[fromId - 1];
		Vertex * to = vertices[toId - 1];
		Edge * e = new Edge{ len, from, to };
		edges.push_back(e);
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
		for (Edge* e : edges) delete e;
		for (Vertex* v : memoryPointersToVerts) delete[] v;
	}


	void cluster(int finalClustersNum) {
		int currentClustersNum = vertices.size();
		vector<Edge*> edges(this->edges);
		sort(edges.begin(), edges.end(), [](Edge * e1, Edge * e2) {return e1->length > e2->length; });

		while (currentClustersNum > finalClustersNum) {
			Edge* e = edges.back();
			edges.pop_back();
			if (e->from->master == e->to->master)
				continue;
			Vertex* master1 = e->from->master;
			Vertex* master2 = e->to->master;
			Vertex* bestMaster = master1->groupSize > master2->groupSize ? master1 : master2;
			Vertex* worseMaster = master1 == bestMaster ? master2 : master1;

			bestMaster->groupSize += worseMaster->groupSize;
			worseMaster->groupSize = 0;

			bestMaster->groupMembers.insert(bestMaster->groupMembers.end(), worseMaster->groupMembers.begin(), worseMaster->groupMembers.end());
			for (Vertex* memberToUpdate : worseMaster->groupMembers)
				memberToUpdate->master = bestMaster;

			currentClustersNum--;
		}

		while (true) {
			Edge* e = edges.back();
			edges.pop_back();
			if (e->from->master == e->to->master)
				continue;
			cout << "Next min size: " << e->length << endl;
			break;
		}
	}

	void printEdges() {
		for (Edge * e : edges)
			cout << e->length << endl;
	}


private:
	vector<Vertex*> vertices;
	vector<Vertex*> memoryPointersToVerts;
	vector<Edge*> edges;
	int numVertices = 0;

};
