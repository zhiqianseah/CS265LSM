#include "basicarray.h"
#include "heap_merge_k_list.h"
#include <iostream>

class SortedArray : public BasicArray {


	//inherits Basic Array's constructor
	using BasicArray::BasicArray;

	private:

		//find the insertion point into the SortedArray via binary search
		int find_position(int key);
	public:
		bool insert(int key, int value);

		bool remove(int key);

		int get(int key);
		void print_all();

		void bulkload(std::pair<keyValue*, int>* k_lists, int k);
		std::pair<keyValue*, int> transferAll();
};