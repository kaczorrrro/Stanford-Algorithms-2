#include <vector>
#include <iostream>

template <class type>
class Matrix {
public:
	Matrix(int rows, int columns) :rows(rows), columns(columns) {
		matrix = new type[rows*columns];
	}

	~Matrix() {
		delete matrix;
	}

	void setAllTo(const type value) {
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				at(i, j) = value;
	}

	void print() {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++)
				cout << at(i, j) << '\t';
			cout << endl;
		}
		
	}

	type & at(int row, int column) {
		if (row < 0 || row >= rows || column < 0 || column >= columns)
			throw invalid_argument("Wrong index");
		return matrix[row*columns + column];
	}

	type atOrZero(int row, int column) {
		if (row < 0 || row >= rows || column < 0 || column >= columns)
			return 0;
		return matrix[row*columns + column];
	}

private:
	int rows, columns;
	type * matrix;
};

using namespace std;

struct Element {
	int value;
	double prob;
};

class OptimalBinaryTree {
public:
	void addElem(Element e) {
		elems.push_back(e);
	}

	double calculate() {
		int numElems = elems.size();
		Matrix<double> values(numElems, numElems);

		values.setAllTo(0);

		for (int i = 0; i < numElems; i++)
			values.at(i, i) = elems[i].prob;

		for (int i = 1; i < numElems; i++) {
			for (int j=0;j<numElems-i;j++){
				int x = j;
				int y = i + j;

				double tempMin = 1e300;//double max
				for (int r = 0; r <= i; r++) {
					double summed = values.atOrZero(y - (r+1), x) + values.atOrZero(y, x + (1 + i - r));
					if (summed < tempMin) {
						tempMin = summed;
					}
				}

				values.at(y, x) = tempMin + sumProbs(x, y);
			
			}
		}

		values.print();
		return values.at(numElems-1, 0);
	}

private:
	vector<Element> elems;

	double sumProbs(int first, int last) {
		double sum = 0;
		for (int i = first; i <= last; i++)
			sum += elems[i].prob;
		return sum;
	}
};

int main() {
	int elements;
	cin >> elements;

	OptimalBinaryTree obt;

	int value;
	double prob;
	for (int i = 0; i < elements; i++) {
		cin >> value >> prob;
		obt.addElem(Element{ value, prob });
	}

	obt.calculate();


}