#ifndef INDEXEDARRAY_H
#define INDEXEDARRAY_H

#include "storage.h"
#include <cstdlib>
#include "heap_merge_k_list.h"
//A B-plus tree, where keys are guaranteed to be unique
//the first page contains the B-tree nodes (excluding the leaves)
//while the second page onwards contains the leaves (the data itself)
//the B-tree nodes on the lowest level will point to the page that contains the data
//the data is then found within that page using binary search


//NOTE: currently the B-tree does not support element-wise insertion or deletion
//because it is used primary as a component of LSM tree

#include <iostream>

class IndexedArray : public Storage {
	protected:

		//Array where the keyValues are stored
		keyValue* array;

		int* index;

		//end of the array
		int endarray;

		//size of the file in disk (if used)
		int filesize;

		//size of a page on the system
		int pagesize;

		//number of pages for the index
		int indexsize;

		bool verbose; 
		int KV_in_page;
		int find_page_in_index(int key);
		int find_position(int first, int last, int key);

	public:

		//Constructor. 
		IndexedArray(int size, bool verbose);

		//Constructor. Create  on disk at filepath
		IndexedArray(int size, const char* filepath, bool verbose);



		bool insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);

		void bulkload(std::pair<keyValue*, int>* k_lists, int k) ;

		std::pair<keyValue*, int> transferAll();

		void deleteAll();

		void printAll();
		void closeFile();
};

#endif