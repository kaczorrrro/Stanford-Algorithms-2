#include <iostream>
#include <vector>
#include <algorithm>

template <class type>
class Matrix {
public:
	Matrix(int rows, int columns) :rows(rows), columns(columns) {
		matrix = new type[rows*columns];
	}

	~Matrix() {
		delete matrix;
	}

	type & at(int row, int column) {
		if (row < 0 || row >= rows || column < 0 || column >= columns)
			throw invalid_argument("Wrong index");
		return matrix[row*columns + column];
	}

private:
	int rows, columns;
	type * matrix;
};

using namespace std;

struct Element {
	int size;
	int value;
};

class Knapsack {
public:
	Knapsack(int size) :size(size) {};
	
	void addElement(Element e) {
		elements.push_back(e);
	}

	long long solve() {
		int numElems = elements.size();
		Matrix<long long> m(size+1, numElems+1);

		//fill some places with zero
		for (int i = 0; i <= numElems; i++)
			m.at(0, i) = 0;
		for (int i = 0; i <= size; i++)
			m.at(i, 0) = 0;

		for (int s = 1; s <= size; s++) {
			for (int n = 1; n <= numElems; n++) {
				Element e = elements[n-1];
				if (n - 1 < 0)
					continue;
				if (s - e.size < 0)
					m.at(s, n) = m.at(s, n - 1);
				else
					m.at(s, n) = max(m.at(s, n - 1), m.at(s - e.size, n - 1) + e.value);
			}
		}

		return long long(m.at(size, numElems));


	}

private:
	int size;
	vector<Element> elements;
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