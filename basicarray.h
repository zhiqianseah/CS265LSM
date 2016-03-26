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

		//current page index for the rolling merges
		int rolling_merge_index;

	public:

		//Constructor. Create a sorted array with given input size in memory
		BasicArray(int size);

		//Constructor. Create a sorted array with given input size on disk at filepath
		BasicArray(int size, const char* filepath);



		bool insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);

		void bulkload(keyValue* input, int size) ;

		void deletePage();

		std::pair<keyValue*, int> transferPage();

		std::pair<keyValue*, int> transferAll();

		void deleteAll();


		~BasicArray();
};


#endif