#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <random>
#include <unordered_set>
#include <algorithm>
#include <sstream>
#include <stack>

using namespace std;

int vertCount;
int variablesCount;

int negatedIndex(int index) {
	if (index >= variablesCount)
		return index - variablesCount;
	return variablesCount + index;
}


class Edge;

class Vertex {
public:
	int id;
	int masterId = -1;
	int timeFinished = -1;
	bool visited = false;
	vector<Edge*> inputs, outputs;
};

class Edge {
public:
	Vertex *from, *to;
};

//bool edgeConnectsVertex(Edge * e, Vertex * v) {
//	return e->v1 == v || e->v2 == v;
//}

namespace std {
	template<>
	struct hash<pair<int, int>>{
		size_t operator()(const pair<int,int>& p)const{
			return p.first ^ p.second;
		}
	};
}


class Graph {
public:
	/*
	 * Appends vertices with ids [0, count) + #verts before adding
	 */
	void batchAddVertex(int count) {
		if (count <= 0)
			return;
		Vertex* verts = new Vertex[count];
		memoryPointersToVerts.push_back(verts);

		for (int i = 0; i < count; i++) {
			Vertex* v = verts + i;
			v->id = numVertices+i;
			vertices.push_back(v);
		}
		numVertices += count;
	}


	/*
	 * Adds edges based on positions in verts array (that should correspond to their ids
	 * Force simple checks if there are no repeated edges or self loops in vector
	 */
	void batchAddEdge(vector<pair<int, int>> fromToPairs, bool forceSimple) {
		int size = fromToPairs.size();
		if (size == 0)
			return;

		//used only with force simple on
		unordered_set<pair<int, int>> alreadyAdded;

		Edge * edgeObjects = new Edge[size];
		for (int i = 0; i < size; i++) {
			Vertex * from = vertices[fromToPairs[i].first];
			Vertex * to = vertices[fromToPairs[i].second];
			if (forceSimple) {
				if (from == to || alreadyAdded.count(fromToPairs[i]) > 0) {
					continue;
				}
				alreadyAdded.insert(fromToPairs[i]);
			}
			Edge* e = edgeObjects + i;
			e->from = from;
			e->to = to;
			from->outputs.push_back(e);
			to->inputs.push_back(e);
		}
		memoryPointersToEdges.insert(edgeObjects);
	}

	void printMe() {
		for (Vertex* v : vertices) {
			cout << "Vertex " << v->id << "\tFinished time:" << v->timeFinished << endl;
			cout << "Inputs: ";
			for (Edge* e : v->inputs)
				cout << e->from->id << " ";
			cout << endl;

			cout << "Outputs: ";
			for (Edge* e : v->outputs)
				cout << e->to->id << " ";
			cout << endl << endl;
		}
	}

	/*
	 * This makes sense only for 2SAT reduction
	 * Used to check assignment for variable and it's compliment
	 */
	void printAssingments() {
		for (int i = 0; i<numVertices / 2; i++) {
			cout << "X" << i + 1 << ": " << groupAssignment[i] << "\t ~X" << i + 1 << ": " << groupAssignment[negatedIndex(i)] << endl;
		}
	}

	/*
	 * 2SAT only
	 */
	bool satisfiable() {
		for (int i = 0; i<numVertices / 2; i++) {
			if (groupAssignment[i] == groupAssignment[negatedIndex(i)])
				return false;
		}
		return true;
	}

	Graph() {};

	~Graph() {
		for (Edge* e : memoryPointersToEdges) delete e;
		for (Vertex* v : memoryPointersToVerts) delete[] v;
	}

	void DFS_loop(bool reversed) {
		if (!reversed && !sortedByTime) {
			cout << "First sort by time!" << endl;
			return;
		}

		if (!reversed)
			groupAssignment = vector<int>(numVertices);

		for (Vertex* v : vertices) {
			if (v->visited)continue;
			int parentVertexId = v->id;
			DFS(v, reversed, parentVertexId);
		}
	}

	void unvisitAll() {
		for (Vertex *v : vertices) {
			v->visited = false;
		}
	}

	void sortVerticesByFinishingTime() {
		sort(vertices.begin(), vertices.end(), [](Vertex* v1, Vertex* v2) {return v1->timeFinished > v2->timeFinished; });
		sortedByTime = true;
		//this should also block possibility of adding new edges/vertices
	}


private:
	vector<Vertex*> vertices;
	vector<Vertex*> memoryPointersToVerts;
	unordered_set<Edge*> memoryPointersToEdges;
	vector<int> groupAssignment;
	int numVertices = 0;
	int currentTimeFinished = 0;
	bool sortedByTime = false;


	/*
	 * Non recursive DFS that uses stack structure
	 */
	void DFS(Vertex* start, bool reversed, int parentVertexId) {
		stack<Vertex*> stackedVerts;
		stackedVerts.push(start);

		while (!stackedVerts.empty()) {
			Vertex* vert = stackedVerts.top();
			stackedVerts.pop();

			//check here is only necessary in verion with stack, and not recursive calls
			if (vert->visited) {
				if (vert->timeFinished == -1)//check to assing value only once, important
					vert->timeFinished = currentTimeFinished++;
				continue;
			}

			//put on stack for postProcessing after all nodes visible from here will be processed
			//this trick won't work if there are edges pointing v1->v1
			//post processing means asigning timeFinished value
			stackedVerts.push(vert);

			vert->visited = true;


			vector<Edge*> & outputs = reversed ? vert->inputs : vert->outputs;

			for (Edge *e : outputs) {
				Vertex *v = reversed ? e->from : e->to;
				if (!v->visited)
					stackedVerts.push(v);
			}
			if (!reversed)
				groupAssignment[vert->id] = parentVertexId;
		}
	}
};


/*
 * Much simpler solution, but useful only for small number of variables
 */
class BruteforceWithBacktrack {
public:
	BruteforceWithBacktrack(vector<pair<int, int>> rules, int numVars) : rules(rules) {
		vars = vector<bool>(numVars);
	}


	/*
	 * Finding assignment using backtracking
	 * I hoped it would work for lots of variables so I didn't make it recursive
	 */
	bool findSolution() {
		int numVars = vars.size();
		stack<int> varTries;
		varTries.push(0);

		while (!varTries.empty()) {
			bool valToAsign;
			bool end = false;

			switch (varTries.top()) {
			case 0: valToAsign = true; break;
			case 1: valToAsign = false; break;
			case 2: varTries.pop(); end = true; break;
			}

			if (end) {
				continue;
			}

			vars[varTries.size() - 1] = valToAsign;

			++varTries.top();

			//check correctness
			bool failed = false;
			for (auto rule : rules) {
				if (abs(rule.first) <= varTries.size() && abs(rule.second) <= varTries.size()) {
					if (rule.first == 0 || rule.second == 0) {
						cout << "No rule should query 0 variable" << endl;
					}
					bool first = rule.first > 0 ? vars[rule.first - 1] : !vars[-rule.first - 1];
					bool second = rule.second > 0 ? vars[rule.second - 1] : !vars[-rule.second - 1];
					if (!first && !second) {
						failed = true;
						break;
					}
				}
			}

			if (!failed) {
				if (varTries.size() == numVars)
					return true;
				varTries.push(0);
			}

		}
		return false;


	}

	void printSolution() {
		for (int i=0;i<vars.size();i++) {
			cout << i + 1 << ": " << vars[i] << endl;
		}
	}

private:
	vector<pair<int, int>> rules;
	vector<bool> vars;
};

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

stringstream generateProblem(int n) {
	stringstream dataStream;
	dataStream << n << endl;
	uniform_int_distribution<int> uni(-n, n);
	for (int i=0;i<n;i++) {
		auto var1 = uni(rng);
		auto var2 = uni(rng);
		if (var1 == 0 || var2 == 0) {
			i--;
			continue;
		}
		dataStream << var1 << " " << var2 << endl;
	}
	return dataStream;

}

void runTest(int size) {
	auto dataStream = generateProblem(size);
	auto dataCopy = dataStream.str();
	dataStream >> variablesCount;

	vertCount = 2 * variablesCount;
	Graph g;
	g.batchAddVertex(vertCount);
	vector<pair<int, int>> rules;
	vector<pair<int, int>> inputEdges;
	for (int i = 0; i<variablesCount; i++) {
		int var1, var2;
		dataStream >> var1 && dataStream >> var2;
		rules.push_back(make_pair(var1, var2));

		var1 = var1 >= 0 ? var1 - 1 : negatedIndex(-var1 - 1);
		var2 = var2 >= 0 ? var2 - 1 : negatedIndex(-var2 - 1);

		inputEdges.push_back(make_pair(negatedIndex(var1), var2));
		inputEdges.push_back(make_pair(negatedIndex(var2), var1));

	}

	BruteforceWithBacktrack brute(rules, variablesCount);


	g.batchAddEdge(inputEdges, true);
	g.DFS_loop(true);
	g.unvisitAll();
	g.sortVerticesByFinishingTime();
	g.DFS_loop(false);

	if (brute.findSolution() != g.satisfiable()) {
		brute.printSolution();
		cout << "Fail" << endl;
		cout << dataCopy << endl << endl;
	
	}
}

void runTests(int nTests, int size) {
	for (int i = 0; i < nTests; i++)
		runTest(size);
}

int main(int argc, char* argv[]) {
	//runTests(100000, 40);
	//exit(0);

	cin >> variablesCount;


	vertCount = 2 * variablesCount;

	Graph g;

	g.batchAddVertex(vertCount);	
	

	vector<pair<int, int>> rules;
	vector<pair<int, int>> inputEdges;
	for (int i = 0; i<variablesCount; i++) {
		int var1, var2;
		cin >> var1 && cin >> var2;
		


		//burte force uses rules in the same form as given
		rules.push_back(make_pair(var1, var2));

		//we read x1 v x2, so we add edges ~x1 -> x2, ~x2->x1

		//get variables in basic form to negate them later
		//-1 since variables in input start from 1
		var1 = var1 >= 0 ? var1 - 1 : negatedIndex(-var1 - 1);
		var2 = var2 >= 0 ? var2 - 1 : negatedIndex(-var2 - 1);

		inputEdges.push_back(make_pair(negatedIndex(var1), var2));
		inputEdges.push_back(make_pair(negatedIndex(var2), var1));
	}
	BruteforceWithBacktrack brute(rules, variablesCount);
	//cout << "Brute: " <<  brute.findSolution() << endl;
	//brute.printSolution();

	//cout << "Edges read" << endl;
	g.batchAddEdge(inputEdges, true);
	//cout << "Edges added" << endl;

	//g.printMe();
	//g.printMe();
	g.DFS_loop(true);
	//cout << "First loop finished" << endl;
	g.unvisitAll();
	g.sortVerticesByFinishingTime();
	//cout << "Sorted" << endl;	
	g.DFS_loop(false);
	//g.printAssingments();
	cout << g.satisfiable() << endl;






}
