#ifndef BASICARRAY_H
#define BASICARRAY_H

#include "storage.h"

//A basic unsorted array, where keys are guaranteed to be unique
//insertion is done by scanning first to see if it exist in the array
//deletion is done lazily by finding the element and marking it as NOT_FOUND

#include <iostream>

class BasicArray : public Storage {
	protected:

		//Array where the keyValues are stored
		keyValue* array;


		//end of the array
		int endarray;

		//size of the file in disk (if used)
		int filesize;

		bool verbose;
	public:

		//Constructor. Create a sorted array with given input size in memory
		BasicArray(int size, bool verbose);

		//Constructor. Create a sorted array with given input size on disk at filepath
		BasicArray(int size, const char* filepath, bool verbose);



		bool insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);

		void bulkload(std::pair<keyValue*, int>* k_lists, int k) ;

		std::pair<keyValue*, int> transferAll();

		void deleteAll();
		void printAll();

		void closeFile();

		void RemoveDuplicates();

		void merge(int l, int m, int r);

		void mergeSort(int left, int right);
};


#endif