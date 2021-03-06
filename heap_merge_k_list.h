//This class merges k sorted list into one sorted list
//Uses a merge sort with a heap.
//the heap is indexed by positions in the HeapNode array
//idea taken from http://www.geeksforgeeks.org/merge-k-sorted-arrays/
#ifndef HEAPMERGE_H
#define HEAPMERGE_H

#include "storage.h"
#include <iostream>
#include <cstdlib>
struct HeapNode {
	keyValue KV; 					//current key-value pair
	int sorted_list;				//which sorted list it belongs to
	int sorted_list_index;			//which index it is in
};

class heap_merge_k_list {
	private:
		HeapNode* Heap;
		int size;
		std::pair<keyValue*, int>* k_sorted_lists;
		int total_keys;
		//indexed by position, the tree is
		//0 is parent of (1, 2)
		//1 is parent of (3, 4)
		//2 is parent of (5, 6)
		//etc
		int left_child (int parent);

		int right_child (int parent);

		void min_heap(HeapNode* starting_nodes, int k);

		//get the minimum entry in the heap
		HeapNode get_min();

		void replace_min(HeapNode new_node);

		void heapify(int parent);

		void swap(HeapNode* first, HeapNode* second);

		bool verbose;
	public:

		//constructor. takes in an array of (KV pointers, length), and an int
		//for number of arrays
		heap_merge_k_list(std::pair<keyValue*, int>* k_lists, int k, bool verbose );


		//dest is the memory address to write the merged KVs to
		//max_size is the maximum size of the dest addr
		//index is the location to store indexes to the KV. NULL if not applicable.
		int merge(keyValue* dest, int max_size, int* index);

		~heap_merge_k_list();

};

#endif