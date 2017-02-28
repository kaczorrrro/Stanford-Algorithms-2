#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

template <class T>
class Heap
{
public:
	struct Entry
	{
		int key;
		T obj;

		bool operator<(Entry & e1) { return this->key < e1.key; };
		bool operator<=(Entry & e1) { return this->key <= e1.key; };
		bool operator>(Entry & e1) { return this->key > e1.key; };
		bool operator>=(Entry & e1) { return this->key >= e1.key; };
	};

	Heap() {};
	Heap(const vector<Entry> & inputData) : storage(inputData) {
		//linear time heapify
		numElements = storage.size();
		for (int i = numElements - 1; i >= 0; i--) {
			bubbleDown(i);
		}
	};

	void insert(T object, int key) {
		int position = numElements++;
		storage.push_back(Entry{ key, object });
		bubbleUp(position);
	};

	void insert(Entry object) {
		int position = numElements++;
		storage.push_back(object);
		bubbleUp(position);
	};

	Entry & extractMin() {
		if (!numElements) {
			cout << "Extract from 0 elem" << endl;
			throw std::exception("Extract from empty heap");
		}
		return storage[0];
	}

	void popMin() {
		if (!numElements) {
			cout << "Extract from 0 elem" << endl;
			throw std::exception("Extract from empty heap");
		}
		if (numElements == 1) {
			numElements = 0;
			storage.pop_back();
			return;
		}

		int position = --numElements;
		storage[0] = storage[position];
		storage.pop_back();
		bubbleDown(0);
	}

	void printMe() {
		int power = 1;
		int spaces = 64;
		cout << std::string(spaces / 2, ' ');

		for (int i = 0, j = 0; i < numElements; i++, j++) {
			cout << storage[i].key << std::string(spaces / power, ' ');
			if (j % power == 0) {
				power *= 2;
				j = 0;
				cout << endl;
				cout << std::string(spaces / power / 2, ' ');
			}
		}

		cout << endl << endl;
	}

	int size() {
		return numElements;
	}
	~Heap() {};
private:
	vector<Entry> storage;
	int numElements = 0;

	void bubbleUp(int index) {
		if (index == 0)
			return;

		int parentIndex = getParentIndex(index);
		if (storage[index] < storage[parentIndex]) {
			std::swap(storage[index], storage[parentIndex]);
			bubbleUp(parentIndex);
		}
	}

	void bubbleDown(int index) {
		//swap with smaller children
		int currentParentKey = storage[index].key;
		int child1Index = getFirstChildrenIndex(index);

		if (child1Index >= numElements) //first children out of range -> reached bottom, terminate
			return;

		int child1Key = storage[child1Index].key;

		int child2Key;
		if (child1Index + 1 >= numElements)  //just first child exist, so lets assume child 2 has so big key that we don't care
			child2Key = INT32_MAX;
		else								//but if it exist we want it's real key
			child2Key = storage[getFirstChildrenIndex(index) + 1].key;

		if (currentParentKey <= child1Key && currentParentKey <= child2Key)	//parent smaller than both children, terminate
			return;

		int smallerChildIndex = child1Key <= child2Key ? child1Index : child1Index + 1;
		std::swap(storage[smallerChildIndex], storage[index]);
		bubbleDown(smallerChildIndex);
	}

	static int inline getParentIndex(int i) {
		return (i - 1) / 2;
	}

	static int inline getFirstChildrenIndex(int i) {
		return i * 2 + 1;
	}

};