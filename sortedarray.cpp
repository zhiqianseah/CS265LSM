#include "sortedarray.h"

int SortedArray::find_position(int key) {
	int first = 0;
	int last = fill-1;
	int middle = (first+last)/2;

	while (first <= last) {
		if (array[middle].key < key) {
			first = middle+1;
		}
		else if(array[middle].key == key) {
			return middle;
		}
		else {
			last = middle -1;
		}
		middle = (first+last)/2;
	}

	//std::cout<<"middle"<<middle<<" fill"<<fill <<"\n";

	if (array[middle].key < key && middle < fill){
		if (fill != max_size){
			return middle+1;
		}
		else {
			return middle;
		}

	}
	else{
		return middle;
	}
}

bool SortedArray::insert(int key, int value) {



	int pos = find_position(key);
	//std::cout<<"inserting "<<key<<" into "<<pos<<" is the insert position. fill is:"<<fill<<"\n";


	//if array is full, return 0
	if (fill == max_size && array[pos].key != key){
		//print_all();
		return 0;
	}
	//check if the key already exists
	else if (array[pos].key == key && pos<fill) {
		array[pos].value = value;
		return 1;
	}
	fill+=1;
	int tempkey;
	int tempvalue;
	//if the key does not exist
	//insert the element in by shifting all elements back by one
	for (int x = pos; x< fill; x++) {
		tempkey = array[x].key;
		tempvalue = array[x].value;
		array[x].key = key;
		array[x].value = value;
		//std::cout<<"x is:"<<x<<" value:"<<array[x].key<<"\n";
		key = tempkey;
		value = tempvalue;

	}

	//print_all();
	return 1;
}

int SortedArray::get(int key){

	int pos = find_position(key);
	if (array[pos].key == key){
		return array[pos].value;
	}
	else{
		return NOT_FOUND;
	}	
}

bool SortedArray::remove(int key){
	if (verbose) std::cout<<"removing\n";
	int pos = find_position(key);

	if (array[pos].key != key){
		return NOT_FOUND;
	}
	else{

		fill -= 1;

		//delete an KV by shifting all the KV after it forward by one
		for (int x =pos; x< fill; x++)
		{
			array[x].key = array[x+1].key;
			array[x].value = array[x+1].value;
		}
	}
	return 1;
}

void SortedArray::bulkload(std::pair<keyValue*, int>* k_lists, int k){
	if (k == 1){
		keyValue* input = k_lists[0].first;
		int size = k_lists[0].second;

		if (size > max_size){
			if (verbose) std::cout<<"Error. Array is not large enough\n";
			return;
		}

		for (int x = 0; x< size; x++)
		{
			array[x].key = input[x].key;
			array[x].value = input[x].value;
		}

		fill = size;
	}
	else {

		heap_merge_k_list heapmerge = heap_merge_k_list(k_lists, k, verbose);
		fill = heapmerge.merge(array, max_size, nullptr);
	}

} 
void SortedArray::print_all(){
	std::cout<<"---------printing current array----------\n";
	for (int x =0; x< fill; x++){
		std::cout<<array[x].key<<","<<array[x].value<<" ";
	}
	std::cout<<"---------END printing current array-----------\n";
}

std::pair<keyValue*, int> SortedArray::transferAll()
{
	std::pair <keyValue*, int> output;
	output.first = array;
	output.second = fill;
	return output;
}

