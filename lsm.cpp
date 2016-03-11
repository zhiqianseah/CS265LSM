#include "basicarray.h"
#include "sortedarray.h"
#include "btree.h"
#include "lsm.h"
#include <iostream> 
#include <cstdlib>
#include <sstream>
#include <cmath>


LSM::LSM(int levels_input, int* level_types, int c0_size, int ratio,  int threshold_percentage) {

	lsm_storage = new Storage*[levels];

	//get size of a page
	int pagesize = getpagesize();
	threshold = threshold_percentage; 
	levels = levels_input;

	//for now, default to basic storage for all levels

	//calculate how many KV pairs can be stored in a page
	int KV_in_page = pagesize/sizeof(keyValue) ;

	//first level of LSM tree doesn't need a file path
	lsm_storage[0] = new SortedArray(c0_size* KV_in_page);
	for (int x =1; x< levels; x++){

		//workaround for tostring due to bug in cgywin
    	std::ostringstream ss;
    	ss << x;
		std::string filepath = std::string(FOLDERPATH) + "LSM"+ss.str()+".bin";
		lsm_storage[x] = new SortedArray(c0_size*KV_in_page*pow(ratio,x), filepath.c_str());
	}
}

bool LSM::insert(int key, int value){ 

	bool success = lsm_storage[0]->insert(key, value);

	//check if we exceed the threshold for merging
	if (lsm_storage[0]->get_fill() >= (lsm_storage[0]->get_max_size() * threshold)/ 100)
	{
		std::cout<<"MERGING"<<std::endl;

		//identify the array starting point and size to copy to lower level
		//this is guaranteed to be a page of data
		std::pair <keyValue*, int> transfer = lsm_storage[0]->transferPage();

		//bulkload the array to the lower level
		lsm_storage[1]->bulkload(transfer.first, transfer.second);

		//delete a page of data from C0
		lsm_storage[0]->deletePage();
	}

	return success;
}

int LSM::get(int key) {

	int curr_level = 0;
	int result = lsm_storage[curr_level]->get(key);

	//if the result is not found on the first level, try the lower levels
	while(result == NOT_FOUND && curr_level < levels-1) {
		std::cout<<"reading from lower levels\n";
		curr_level++;
		result = lsm_storage[curr_level]->get(key);
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

LSM::~LSM(){
	for (int x =0; x< levels; x++){
		delete lsm_storage[x];
	}	
	delete lsm_storage;
}	