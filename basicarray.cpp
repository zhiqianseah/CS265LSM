#include "basicarray.h"



BasicArray::BasicArray(int size) {
	std::cout<<"creating array of size "<< size<<"\n";
	array = new keyValue[size];

	//number of valid entries in the array
	fill = 0;

	//end of the valid array. there may be holes, so fill != endarray
	endarray = 0;
	max_size = size;
	rolling_merge_index = 0;
}

BasicArray::BasicArray(int size, const char* filepath) {
	std::cout<<"creating array of size "<< size<< " at "<< filepath<<"\n";
	fill = 0;
	endarray = 0;
	max_size = size;
	rolling_merge_index = 0;

    int result;
    filesize = size*sizeof(keyValue);

	//open file at filepath. create it if necessary
	fd = open(filepath, O_RDWR | O_CREAT);
    if (fd == -1) {
	std::cout<<"Error in opening or creating file. Exiting.\n";
	exit(EXIT_FAILURE);
    }

    /* Stretch the file size to the size of the (mmapped) array of ints
     */
    result = lseek(fd, filesize-1, SEEK_SET);
    if (result == -1) {
	close(fd);
	std::cout<<"Error in stretching the file to the right size. Exiting";
	exit(EXIT_FAILURE);
    }

    //write a zero byte to the last byte of the array to allocate the space
    //this is needed for the file to actually have that amount of size
    result = write(fd, "", 1);
    if (result != 1) {
	close(fd);
	std::cout<<"Error in writing "" to the last byte of the file";
	exit(EXIT_FAILURE);
    }

    //now, the file is ready to be used
    array = (keyValue*)mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (array == MAP_FAILED) {
	close(fd);
	std::cout<<"Error in mmapping the file to KeyValue pointer";
	exit(EXIT_FAILURE);
    }


}


bool BasicArray::insert(int key, int value) {

	//std::cout<<"inserting:"<<key<<std::endl;
	int hole = -1;
	for (int x = 0; x< fill; x++) {
		if (array[x].key == key) {
			array[x].value = value;
			return 1;
		}
		else if (hole == -1 && array[x].value == NOT_FOUND) {
			hole = x;
		} 
	}	

	//if the element is not found, insert it into a hole if possible
	if (hole != -1) {
		array[hole].key = key;
		array[hole].value = value;
	}
	else {

		//if the array is maxed out. The check is only done here because
		//a duplicate key may be inserted
		if (max_size == endarray) {
			return 0;
		}

		array[endarray].key = key;
		array[endarray].value = value;
		endarray+= 1;		
	}

	fill+= 1;
	return 1;
}

int BasicArray::get(int key) {
	for (int x = 0; x< fill; x++) {
		if (array[x].key == key) {
			return array[x].value;
		}
	}
	return NOT_FOUND;
}


bool BasicArray::remove(int key) {
	for (int x = 0; x< fill; x++) {
		if (array[x].key == key) {
			array[x].value = NOT_FOUND;
			fill -= 1;
			return 1;
		}
	}	
	return 0;
}

bool BasicArray::update(int key, int value){
	for (int x = 0; x< fill; x++) {
		if (array[x].key == key) {
			array[x].value = value;
			return 1;
		}
	}	
	return 0;	
}

void BasicArray::bulkload(std::pair<keyValue*, int>* k_lists, int k){
	if (k == 1){
		keyValue* input = k_lists[0].first;
		int size = k_lists[0].second; 
		for (int x = 0; x<size; x++) {
			if (input[x].key != NOT_FOUND) {
				insert(input[x].key, input[x].value);
			}
		}
	}
	else {
		std::cout<<"NOT IMPLEMENTED\n";
	}
} 


std::pair<keyValue*, int> BasicArray::transferPage(){
	std::pair <keyValue*, int> output;

	int transfersize = getpagesize()/sizeof(keyValue);
	output.first = &array[transfersize*rolling_merge_index];
	output.second = transfersize;

	std::cout<<"moving "<< transfersize<<" from:"<<array[transfersize*rolling_merge_index].key<<"onwards\n";
	//move the rolling merge index to the next page
	//so that the next transfer will be the next page


	if (rolling_merge_index == max_size/transfersize) {
		rolling_merge_index = 0;
	}
	return output;
}

std::pair<keyValue*, int> BasicArray::transferAll(){
	std::pair <keyValue*, int> output;

	int transfersize = fill;
	output.first = array;
	output.second = transfersize;

	std::cout<<"moving "<< transfersize<<" from:"<<array[0].key<<"onwards\n";
	return output;
}

void BasicArray::deletePage() {

	int deletesize = getpagesize()/sizeof(keyValue);
	for (int x =deletesize*rolling_merge_index; x< deletesize; x++) {
		array[x].value = NOT_FOUND;
	}
	rolling_merge_index++;
	fill = fill - deletesize;
}

void BasicArray::deleteAll() {
	fill = 0;
	endarray = 0;
}

BasicArray::~BasicArray(){

	if (fd == -1) {
		delete[] array;
	}
	else{
		//unmap the file
	    if (munmap(array, filesize) == -1) {
		std::cout<<"Error in un-mmapping the file";
		/* Decide here whether to close(fd) and exit() or not. Depends... */
	    }


		//close the file descriptor
    	close(fd);		
	}

}
