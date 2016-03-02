#include "sortedarray.h"



SortedArray::SortedArray(int size) {
	std::cout<<"creating array of size "<< size<<"\n";
	array = new int*[size];
	fill = 0;
}
void SortedArray::insert(int key, int value) {
	int* temp = new int [2];
	temp[0] = key;
	temp[1] = value;

	array[fill] = temp;
	fill += 1;
}

int* SortedArray::get(int key) {
	for (int x = 0; x< fill; x++) {
		if (array[x][0] == key)
			return &array[x][1];
	}
	return nullptr;
}

SortedArray::~SortedArray(){
	for (int x = 0; x< fill; x++) {
		delete [] array[x];
	}

	delete[] array;
}

int SortedArray::binary_search(int key){

}