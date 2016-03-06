
#ifndef STORAGE_H
#define STORAGE_H

//Abstract base class of storage formats 

//constant that is used when the element is not found
#define NOT_FOUND -2147483648

class Storage {
	public:

		//insert a key-value pair into the storage 
		virtual void insert(int key, int value) = 0;

		//get the value of a key from the storage
		//return NOT_FOUND if 
		virtual int get(int key) = 0;

		//return 1 if the key-value pair is successfully deleted
		//return 0 otherwise
		virtual bool remove(int key) = 0;

		//return 1 if the key-value pair is successfully updated
		//return 0 otherwise
		virtual bool update(int key, int value) = 0;
};

#endif