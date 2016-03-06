#include "basicarray.h"



BasicArray::BasicArray(int size) {
	std::cout<<"creating array of size "<< size<<"\n";
	array = new keyValue[size];
	fill = 0;
}
void BasicArray::insert(int key, int value) {
	array[fill].key = key;
	array[fill].value = value;
	fill += 1;
}

int BasicArray::get(int key) {
	for (int x = 0; x< fill; x++) {
		if (array[x].key == key)
			return array[x].value;
	}
	return NOT_FOUND;
}


bool BasicArray::remove(int key) {
	for (int x = 0; x< fill; x++) {
		if (array[x].key == key)
			array[x].value = NOT_FOUND;
			return 1;
	}	
	return 0;
}

bool BasicArray::update(int key, int value){
	for (int x = 0; x< fill; x++) {
		if (array[x].key == key)
			array[x].value = value;
			return 1;
	}	
	return 0;	
}

BasicArray::~BasicArray(){
	delete[] array;
}
