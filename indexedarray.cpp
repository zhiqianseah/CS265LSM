#include "indexedarray.h"

IndexedArray::IndexedArray(int size, bool verbose_input) {
	verbose = verbose_input;
	if (verbose) std::cout<<"creating array of size "<< size<<"\n";
	array = new keyValue[size];

	//number of valid entries in the array
	fill = 0;

	//end of the valid array. there may be holes, so fill != endarray
	endarray = 0;
	max_size = size;

	//get size of a page
	pagesize = getpagesize();

	//get number of pages of index
	indexsize = (size+pagesize-1)/pagesize;

	index = new int[pagesize*indexsize];

	KV_in_page = pagesize/sizeof(keyValue);
}

IndexedArray::IndexedArray(int size, const char* filepath, 	bool verbose_input) {
	verbose = verbose_input;
	fill = 0;
	endarray = 0;
	max_size = size;

    int result;


	//get size of a page
	pagesize = getpagesize();
	KV_in_page = pagesize/sizeof(keyValue);
	//get number of pages of index
	indexsize = (size+pagesize-1)/pagesize;

    filesize = size*sizeof(keyValue) + indexsize * pagesize;
    //filesize = size*sizeof(keyValue);
	if (verbose) std::cout<<"creating array of size "<< size<< " with indexsize " <<indexsize<<" and filesize "<<filesize<<" at "<< filepath<<"\n";

	//open file at filepath. create it if necessary
	fd = open(filepath, O_RDWR | O_CREAT,  0666);
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
    //Btree = (int*)mmap(0, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //array = (keyValue*)mmap(pagesize, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
 

    index = (int*)mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //array = (keyValue*)mmap(pagesize, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    array = (keyValue*)&index[(pagesize*indexsize)/sizeof(int)];

    if (array == MAP_FAILED) {
	close(fd);
	std::cout<<"Error in mmapping the file to KeyValue pointer";
	exit(EXIT_FAILURE);
    }


}



bool IndexedArray::insert(int key, int value)
{
	return false;
}

int IndexedArray::get(int key)
{
	int page = find_page_in_index(key);
	//std::cout<<"key is found in page:"<<page<<"\n";
	int position = find_position(page* KV_in_page, ((page+1)*KV_in_page)-1,key);

	if (array[position].key != key){
		return NOT_FOUND;
	}
	else {
		return array[position].value;
	}

}

bool IndexedArray::remove(int key)
{
	return false;
}

bool IndexedArray::update(int key, int value)
{
	return false;
}


void IndexedArray::bulkload(std::pair<keyValue*, int>* k_lists, int k) 
{
	if (k == 1){
		keyValue* input = k_lists[0].first;
		int size = k_lists[0].second;

		if (size > max_size){
			std::cout<<"Error. Array is not large enough\n";
			return;
		}
		for (int x = 0; x< size; x++)
		{
			//Btree[x + pagesize-1] = input[x].key;
			array[x].key = input[x].key;
			array[x].value = input[x].value;

			//if the entry is in a new page, add it to the index
			if (x%KV_in_page == 0)
			{
				int input_index = x/KV_in_page;
				index[input_index] = input[x].key;
				if (verbose) std::cout<<"inserting index of:"<<index[input_index]<<" at "<<input_index<<"\n";
			}
		}

		fill = size;
	}
	else {

		heap_merge_k_list heapmerge = heap_merge_k_list(k_lists, k, verbose);
		fill = heapmerge.merge(array, max_size, index);
	}
}

std::pair<keyValue*, int> IndexedArray::transferAll()
{
	std::pair <keyValue*, int> output;
	output.first = array;
	output.second = fill;
	return output;
}

void IndexedArray::deleteAll()
{

}


void IndexedArray::closeFile()
{

	if (fd == -1) {
		delete[] array;
	}
	else{
		//unmap the file
	    if (munmap(index, filesize) == -1) {
		std::cout<<"Error in un-mmapping the file";
		/* Decide here whether to close(fd) and exit() or not. Depends... */
	    }
   
		//close the file descriptor
    	close(fd);		
	}

}

int IndexedArray::find_page_in_index(int key)
{
	int first = 0;

	//calculate number of filled pages
	int last = ((fill + KV_in_page - 1)/KV_in_page) - 1;
	int middle = (first+last)/2;

	while (first <= last) {
		if (index[middle] < key) {
			first = middle+1;
		}
		else if(index[middle] == key) {
			return middle;
		}
		else {
			last = middle -1;
		}
		middle = (first+last)/2;
	}

	return middle;
}

int IndexedArray::find_position(int first, int last, int key) {
	int middle = (first+last)/2;

	while (first <= last) {
		if (array[middle].key < key) {
			first = middle+1;
		}
		else if(array[middle].key == key) {
			return middle;
		}
		else {
			last = middle -1;
		}
		middle = (first+last)/2;
	}

	//std::cout<<"middle"<<middle<<" fill"<<fill <<"\n";

	if (array[middle].key < key && middle < fill){
		if (fill != max_size){
			return middle+1;
		}
		else {
			return middle;
		}

	}
	else{
		return middle;
	}
}

void IndexedArray::printAll()
{
	std::cout<<"Fill is:"<<fill<<"\n";
	for (int x =0; x<fill; x++)
	{
		std::cout<<array[x].key<<" ";
	}
}