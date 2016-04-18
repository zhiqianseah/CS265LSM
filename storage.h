//Abstract base class of storage formats 
#ifndef STORAGE_H
#define STORAGE_H

//these are for memory mapped storage
#include <fcntl.h>   //for open
#include <unistd.h>  //for lseek
#include <stdlib.h>  //for exit
#include <sys/mman.h> //for mmap
#include <utility>    //for pair
#include <limits.h>

//constant that is used when the element is not found
#define NOT_FOUND -2147483648
#define DELETED 2147483647
//folder for on-disk storage
#define FOLDERPATH "/tmp/"

struct keyValue{
	int key;
	int value;
};


class Storage {
	protected:


		//number of elements in this storage
		int fill;

		//max size of storage
		int max_size;

		//file descriptor. used if on disk
		//default = -1. ie not on disk
    	int fd  = -1;
	public:

		//insert a key-value pair into the storage 
		//returns 0 if the element cannot be inserted because storage is full 
		virtual bool insert(int key, int value) = 0;

		//get the value of a key from the storage
		//return NOT_FOUND if 
		virtual int get(int key) = 0;

		//return 1 if the key-value pair is successfully deleted
		//return 0 otherwise
		virtual bool remove(int key) = 0;

		//return 1 if the key-value pair is successfully updated
		//return 0 otherwise
		virtual bool update(int key, int value) = 0;

		//load an array of KeyValues into the storage 
		virtual void bulkload(std::pair<keyValue*, int>* k_lists, int k) = 0;

		//transfer all the KV pairs out 
		//returns a pointer to the start of the keyvalue store, and an int for the number of KV pairs to transfer
		virtual std::pair<keyValue*, int> transferAll() = 0;

		//delete all keyvalue pairs
		virtual void deleteAll() = 0;

		virtual void closeFile() = 0;

		int get_fill() {return fill;} 

		int get_max_size() {return max_size;} 

		virtual void printAll() = 0;
};

#endif