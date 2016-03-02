#include "storage.cpp"
#include <iostream>

class SortedArray : public Storage {
	private:

		//pointer to 2D array
		int** array;
		int fill;

		//return index of the key, or the position immediately right of it
		int binary_search(int key);
	public:

		//Constructor. Create a sorted array with given input size
		SortedArray(int size);

		void insert(int key, int value);

		int* get(int key);
		~SortedArray();
};