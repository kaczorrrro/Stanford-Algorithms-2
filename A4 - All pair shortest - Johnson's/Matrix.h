#pragma once
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
		if (row < 0 || row >= rows || column < 0 || column >= columns) {
			cout << "Row: " << row << ", col: " << column << " out of index" << endl;
			throw std::invalid_argument("Wrong index");
		}
			
		return matrix[row*columns + column];
	}

private:
	int rows, columns;
	type * matrix;
};