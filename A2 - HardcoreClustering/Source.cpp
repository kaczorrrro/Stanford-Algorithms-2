#include <iostream>
#include <vector>
#include <bitset>
#include <unordered_set>
#include <string>
#include <list>
#include <algorithm>

#define BITS_COUNT 24

class Node;

namespace std
{
	template<>
	struct hash<Node>
	{
		size_t operator()(const Node& k) const;
	};
}

class Node : public std::bitset<BITS_COUNT> {
public:
	Node(std::string s) : bitset<BITS_COUNT>(s) {}
	Node(const Node & n) : bitset<BITS_COUNT>(n) { currentGroup = n.currentGroup; }
	void setGroup(std::unordered_set<Node*>* newSet) { currentGroup = newSet; }
	std::unordered_set<Node*>* getCurrentSet() const{ return currentGroup; }
private:
	std::unordered_set<Node*>* currentGroup = nullptr;
};


namespace std
{
	size_t hash<Node>::operator()(const Node& k) const
	{
		return k.hash();
	}
}

using namespace std;

class Problem {
public:
	void addNode(string s) {
		Node n(s);
		numNodes++;
		nodes.insert(n);
	}

	void group() {
		for (const Node & nConst : nodes) {
			Node & n = const_cast<Node&>(nConst);
			unordered_set<Node*>* newGroup = new unordered_set<Node*>();
			newGroup->insert(&n);
			n.setGroup(newGroup);
		}
		numGroups = nodes.size();

		mergeByDist(1);
		cout << "First pass ended" << endl;
		mergeByDist(2);
	}

	int getGroupCount() {
		return numGroups;
	}

private:
	int numNodes = 0;
	int numGroups = 0;
	unordered_set<Node> nodes;

	
	void mergeByDist(int dist) {
		int i = 0;
		for (const Node & sourceNode : nodes) {
			if (i++ % 1000 == 0) cout << i / (double)numNodes << " groups: " << numGroups << endl;
			vector<Node> neighbours = getNodesWithinDist(sourceNode, dist);
			for (Node neigh : neighbours) {
				dealWithNeighbour(sourceNode.getCurrentSet(), neigh);
			}
		}
	}

	void dealWithNeighbour(unordered_set<Node*>* sourceGroup, Node neighbourFake) {
		auto neighbourIter = nodes.find(neighbourFake);
		if (neighbourIter == nodes.end())
			return;
		auto neighbourGroup = (*neighbourIter).getCurrentSet();
		if (sourceGroup == neighbourGroup)
			return;
		mergeGroups(sourceGroup, neighbourGroup);
	}

	void mergeGroups(unordered_set<Node*>* group1, unordered_set<Node*>* group2) {
		swapGroups(group1, group2);
		for (Node * n : *group2) {
			n->setGroup(group1);
			group1->insert(n);
		}
		delete group2;
		numGroups--;
	}

	void swapGroups(unordered_set<Node*>* & group1, unordered_set<Node*>* & group2) {
		if (group1->size() >= group2->size())
			return;
		auto temp = group1;
		group1 = group2;
		group2 = temp;
	}

	vector<Node> getNodesWithinDist(const Node & source, int dist) {
		vector<Node> output;

		if (dist == 1) {
			for (int i = 0; i < BITS_COUNT; i++) {
				Node newNode(source);
				newNode[i].flip();
				output.push_back(newNode);
			}
		}
		else if (dist > 1) {
			auto recursiveOutput = getNodesWithinDist(source, dist - 1);
			for (Node n : recursiveOutput) {
				vector<Node> localOutput = getNodesWithinDist(n, 1);
				output.insert(output.end(), localOutput.begin(), localOutput.end());
			}
		}
		return output;
	}
};




int main() {
	if (!freopen("input/trueInput.txt", "r", stdin)) {
		cout << "Problem z plikiem" << endl;
		return 1;
	}
	Problem p1;
	int numValues;
	cin >> numValues;
	string s;
	getline(cin, s);
	for (int i = 0; i < numValues; i++) {
		getline(cin, s);
		p1.addNode(s);
	}
	cout << "Reading finished" << endl;
	p1.group();
	cout << endl;
	cout << p1.getGroupCount() << endl;

}

