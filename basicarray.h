#include "storage.h"



#include <iostream>

class BasicArray : public Storage {
	private:

		//Array where the keyValues are stored
		keyValue* array;


		//end of the array
		int endarray;

		//size of the file in disk (if used)
		int filesize;

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
		~BasicArray();
};