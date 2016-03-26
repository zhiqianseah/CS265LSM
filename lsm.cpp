#include "basicarray.h"
#include "sortedarray.h"
#include "btree.h"
#include "lsm.h"
#include <iostream> 
#include <cstdlib>
#include <sstream>
#include <cmath>

/*
Overview of LSM store.

int-int KV pairs are stored in a hierarchial manner where each level is a user-input multiple (R) larger than the previous.
Each level can have different form of storage format. 

Level 0 is a single sorted array. Size = user-input multiple of a page
Subsequent levels have R number of storages. Each storage can store all the elements of the level above it.




*/





/*
level_inputs: number of levels in the LSM tree
level_types: array that specify the type of storage for each level. length = level_input
c0_size: number of pages for the first level. subsequent levels are determined by the ratio parameter
ratio: ratio between storage sizes in each level
*/
LSM::LSM(int levels_input, int* level_types, int c0_size, int ratio_input) {






	//get size of a page
	int pagesize = getpagesize();
	levels = levels_input;
	ratio = ratio_input;

	lsm_storage = new Storage**[levels];
	curr_fill_level = 0;
	curr_fill_index_per_level = new int[levels];

	//for now, default to basic storage for all levels

	//calculate how many KV pairs can be stored in a page
	int KV_in_page = pagesize/sizeof(keyValue) ;

	//first level of LSM tree doesn't need a file path
	lsm_storage[0] = new Storage*[1];
	lsm_storage[0][0] = new SortedArray(c0_size* KV_in_page);
	curr_fill_index_per_level[0] = 0; //this should never be used

	//remaining levels need a file path to do memory-mapped files
	for (int x =1; x< levels; x++){
		lsm_storage[x] = new Storage*[ratio];
		curr_fill_index_per_level[x] = 0;

		for (int y = 0; y< ratio; y++){
			//workaround for tostring due to bug in cgywin for storing the file path
	    	std::ostringstream ss;
	    	ss << x;
	    	std::ostringstream ss2;
	    	ss2 << y;
			std::string filepath = std::string(FOLDERPATH) + "LSM"+ss.str()+"_"+ss2.str()+".bin";

			std::cout<<"creating file at:"<<filepath<<"\n";
			lsm_storage[x][y] = new SortedArray(c0_size*KV_in_page*pow(ratio,x-1), filepath.c_str());
		}
	}
}

bool LSM::insert(int key, int value){ 

	bool success = lsm_storage[0][0]->insert(key, value);

	//if the top level c0 is full, we have to merge it down first
	if (success == false)
	{
		std::cout<<"MERGING"<<std::endl;

		
		std::pair <keyValue*, int> transfer = lsm_storage[0][0]->transferAll();

		//bulkload the array to the lower level
		lsm_storage[1][curr_fill_index_per_level[1]]->bulkload(transfer.first, transfer.second);

		curr_fill_index_per_level[1]++;
		if (curr_fill_index_per_level[1] == ratio)
		{
			std::cout<<"LSM IS FULL"<<std::endl;
			return false;
		}

		lsm_storage[0][0]->deleteAll();


		bool success = lsm_storage[0][0]->insert(key, value);

		//identify the array starting point and size to copy to lower level
		//this is guaranteed to be a page of data
		//std::pair <keyValue*, int> transfer = lsm_storage[0][0]->transferPage();

		//bulkload the array to the lower level
		//lsm_storage[1]->bulkload(transfer.first, transfer.second);

		//delete a page of data from C0
		//lsm_storage[0]->deletePage();
	}

	return success;
}

int LSM::get(int key) {

	int curr_level = 0;
	int get_level_index = 0;
	int result = lsm_storage[curr_level][get_level_index]->get(key);

	//if the result is not found on the first level, try the lower levels
	while(result == NOT_FOUND && curr_level < levels-1) {
		std::cout<<"reading from lower levels\n";
		curr_level++;
		get_level_index = 0;
		while(get_level_index < curr_fill_index_per_level[curr_level]) {
			result = lsm_storage[curr_level][get_level_index]->get(key);

			if (result != NOT_FOUND)
			{
				break;
			}
			get_level_index++;
		}
	}	

	return result;

}

bool LSM::remove(int key) {

}

bool LSM::update(int key, int value){

}

void LSM::bulkload(keyValue* input, int size) {

}

void LSM::deletePage() {

}

std::pair<keyValue*, int> LSM::transferPage() {

}

void LSM::deleteAll() {

}

std::pair<keyValue*, int> LSM::transferAll() {

}

LSM::~LSM(){
	for (int x =0; x< levels; x++){
		delete lsm_storage[x];
	}	
	delete lsm_storage;
}	