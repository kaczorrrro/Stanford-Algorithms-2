#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct KnapsackParams {
	int size, nElems;
	bool operator==(const KnapsackParams & p1) const{
		return size == p1.size && nElems == p1.nElems;
	}
};

struct Element {
	int size;
	int value;
};

struct KnapsackParams_hash {
	size_t operator () (const KnapsackParams &p) const {
		auto h1 = std::hash<int>{}(p.size);
		auto h2 = std::hash<int>{}(p.nElems);
		return h1 ^ h2;
	}
};

class Knapsack {
public:
	Knapsack(int size) :size(size) {};

	void addElement(Element e) {
		elements.push_back(e);
	}

	long long solve() {
		return valueForParams(KnapsackParams{ size,(int)elements.size() });
	}


private:
	int size;
	vector<Element> elements;
	unordered_map<KnapsackParams, long long, KnapsackParams_hash> solved;

	long long valueForParams(const KnapsackParams params) {
		if (params.nElems == 0 || params.size == 0)
			return 0;

		auto readySolution = solved.find(params);
		if (readySolution != solved.end())
			return (*readySolution).second;

		Element currentElem = elements[params.nElems - 1];

		auto param1 = params;
		param1.nElems -= 1;

		auto param2 = params;
		param2.nElems -= 1;
		param2.size -= currentElem.size;
		
		long long output;
		if (param2.size < 0)
			output = valueForParams(param1);
		else
			output = max(valueForParams(param1), valueForParams(param2) + currentElem.value);
		
		solved.insert(make_pair(params, output));
		return output;
	}
};

int main() {
	int size, numElems;
	cin >> size >> numElems;

	Knapsack k(size);

	int val, wei;
	for (int i = 0; i < numElems; i++) {
		cin >> val >> wei;
		k.addElement(Element{ wei,val });
	}


	cout << k.solve() << endl;
}