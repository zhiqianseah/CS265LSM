#include "storage.cpp"



#include <iostream>

class BasicArray : public Storage {
	private:

		struct keyValue{
			int key;
			int value;
		};
		//Array where the key is stored in (x*2) and the value is stored in (x*2 + 1) for some x 
		keyValue* array;
		int fill;

		//return index of the key, or the position immediately right of it
		int binary_search(int key);

	public:

		//Constructor. Create a sorted array with given input size
		BasicArray(int size);

		void insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);
		~BasicArray();
};