#include "basicarray.h"
#include "sortedarray.h"
#include "indexedarray.h"
#include "btree.h"
#include "lsm_mt.h"
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
Types of levels:
	1: SortedArray
	2: IndexedArray
c0_size: number of pages for the first level. subsequent levels are determined by the ratio parameter
ratio: ratio between storage sizes in each level
*/
LSM_mt::LSM_mt(int level_types_input, int c0_size_input, int ratio_input, bool verbose_input) {






	//get size of a page
	int pagesize = getpagesize();
	ratio = ratio_input;
	level_types = level_types_input;
	c0_size = c0_size_input;
	verbose = verbose_input;
	lsm_storage_head = new StorageLL_Node;

	curr_fill_level = 0;

	//for now, default to basic storage for all levels

	//calculate how many KV pairs can be stored in a page
	int KV_in_page = pagesize/sizeof(keyValue) ;

	if (verbose) std::cout<<"KV in a page is:"<<KV_in_page<<"\n";

	//first level of LSM tree doesn't need a file path
	lsm_storage_head->level_storage = new Storage*[1];
	lsm_storage_head->level_storage[0] = new SortedArray(c0_size* KV_in_page, verbose);
	lsm_storage_head->next_node = nullptr;
	lsm_storage_head->level_fill = 1;
	lsm_storage_head->storagelocks = new boost::shared_mutex[1];


}

bool LSM_mt::storage_inserter(int key, int value, StorageLL_Node* curr_node, int index) {
  // get upgradable access
  boost::upgrade_lock<boost::shared_mutex> lock(curr_node->storagelocks[index]);

  // get exclusive access
  boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
  // now we have exclusive access


  return curr_node->level_storage[index]->insert(key, value);
}

bool LSM_mt::insert(int key, int value){ 

	//bool success = lsm_storage_head->level_storage[0]->insert(key, value);
	bool success = storage_inserter(key, value, lsm_storage_head, 0);
	//if the top level c0 is full, we have to merge it down first
	if (success == false)
	{
		if (verbose) std::cout<<"MERGING when input key is"<<key<<std::endl;

		StorageLL_Node* curr_node = lsm_storage_head;
		merge(curr_node, 0);

		//try again
		//success = lsm_storage_head->level_storage[0]->insert(key, value);	
		success = storage_inserter(key, value, lsm_storage_head, 0);
	}



	return success;
}

//merge level with the level below it
void LSM_mt::merge(StorageLL_Node* curr_node, int curr_level) {


	int num_lists;

	//only the first level has 1 list. subsequent levels has 'ratio' lists
	if (curr_level == 0){
		num_lists = 1;
	}
	else{
		num_lists = ratio;
	}


	lock_before_transfer(curr_node,  curr_level, num_lists, 0);

}


//recursively lock each storage on a level
void LSM_mt::lock_before_transfer(StorageLL_Node* curr_node,  int curr_level, int num_lists, int curr_lock_index) {

	if (curr_lock_index < num_lists) {
		if (verbose) std::cout<<"locking level:"<<curr_lock_index<<std::endl;
		// get upgradable access
		boost::upgrade_lock<boost::shared_mutex> curr_storage_lock(curr_node->storagelocks[curr_lock_index]);
		// get exclusive access
		boost::upgrade_to_unique_lock<boost::shared_mutex> curr_uniqueLock(curr_storage_lock);

		curr_lock_index++;
		lock_before_transfer(curr_node, curr_level, num_lists, curr_lock_index+1);
	}
	else {
		//Once we lock all the storages on one level, do the mass transfer
		Mass_Transfer(curr_node, curr_level, num_lists);
	}
}

void LSM_mt::Mass_Transfer(StorageLL_Node* curr_node, int curr_level, int num_lists) {


	StorageLL_Node* lower_level_node = curr_node->next_node;

	//if the lower level hasn't been created yet, create it.
	if (lower_level_node == nullptr)
	{
		//initialize a new node in the linked list
		lower_level_node = new StorageLL_Node;
		lower_level_node->level_storage = new Storage*[ratio];
		lower_level_node->next_node = nullptr;
		lower_level_node->level_fill = 0;
		lower_level_node->storagelocks = new boost::shared_mutex[ratio];

		//set node to be child of previous node
		curr_node->next_node = lower_level_node;

		int KV_in_page = getpagesize()/sizeof(keyValue) ;
		for (int y = 0; y< ratio; y++){
			//workaround for tostring due to bug in cgywin for storing the file path
	    	std::ostringstream ss;
	    	ss << curr_level+1;
	    	std::ostringstream ss2;
	    	ss2 << y;
			std::string filepath = std::string(FOLDERPATH) + "LSM"+ss.str()+"_"+ss2.str()+".bin";



			if (level_types == 1) {
				std::cout<<"creating Sorted Array with file at:"<<filepath<<"\n";
				//lsm_storage[x][y] = new SortedArray(c0_size*KV_in_page*pow(ratio,x-1), filepath.c_str());
				lower_level_node->level_storage[y] = new SortedArray(c0_size*KV_in_page*pow(ratio,curr_level), filepath.c_str(), verbose);
			}
			else if(level_types == 2) {
				if (verbose) std::cout<<"creating Indexed Array with file at:"<<filepath<<"\n";
				//lsm_storage[x][y] = new IndexedArray(c0_size*KV_in_page*pow(ratio,x-1), filepath.c_str());
				lower_level_node->level_storage[y] = new IndexedArray(c0_size*KV_in_page*pow(ratio,curr_level), filepath.c_str(), verbose);			
			}
		}	

	}


	//if the lower level is already filled, then we have to merge the lower level down first

	if (lower_level_node->level_fill == ratio)
	{
		if (verbose) std::cout<<"merging lower level"<<std::endl;
		merge(lower_level_node, curr_level+1);

	}



	std::pair <keyValue*, int> all_lists[num_lists];



	//get the list of sorted arrays from each entry in the current level
	for (int x = 0; x< num_lists; x++){

		all_lists[x] = curr_node->level_storage[x]->transferAll();		
	}

		//bulkload the array to the lower level
	int insert_point = lower_level_node->level_fill;



	// get upgradable access
	boost::upgrade_lock<boost::shared_mutex> lower_level_lock(lower_level_node->storagelocks[insert_point]);
	// get exclusive access
	boost::upgrade_to_unique_lock<boost::shared_mutex> lower_level_uniqueLock(lower_level_lock);
	// now we have exclusive access for the lower level



	lower_level_node->level_storage[insert_point]->bulkload(all_lists, num_lists);

	//delete all entries in current level
	for (int x = 0; x< num_lists; x++){
		//lsm_storage[level][x]->deleteAll();
		curr_node->level_storage[x]->deleteAll();
	}


	//This handles the special case where the merge down can actually fit in the current level

	//get number of new elements added in next level
	//int prev_level_max_size = lsm_storage[level][0]->get_max_size();
	int new_fill = lower_level_node->level_storage[insert_point]->get_fill();
	int prev_level_max_size = curr_node->level_storage[0]->get_max_size();

	if (verbose) std::cout<<"new_fill: "<<new_fill<<". prev_level:"<<prev_level_max_size<<"\n";

	//if all the new entries can fit in the previous level, transfer it back.
	if (new_fill <= prev_level_max_size && curr_level != 0)
	{
		std::pair <keyValue*, int> temp[1];
		temp[0] = lower_level_node->level_storage[insert_point]->transferAll();
		if (verbose) std::cout<<"Special case, transferring back after merging...\n";
		curr_node->level_storage[0]->bulkload(temp, 1);	
		curr_node->level_fill = 1;

	}
	else {

		if (curr_fill_level < curr_level +1) {
			curr_fill_level++;
		}

		if (curr_level > 0) {
			curr_node->level_fill = 0;
		}
		lower_level_node->level_fill++;

	}
}

int LSM_mt::storage_getter(int key, StorageLL_Node* curr_node, int index){

 	// get shared access
  	boost::shared_lock<boost::shared_mutex> lock(curr_node->storagelocks[index]);
	return curr_node->level_storage[index]->get(key);
}

int LSM_mt::get(int key) {

	int get_level = 0;
	int get_level_index = 0;
	StorageLL_Node* curr_node = lsm_storage_head;
	//int result = curr_node->level_storage[0]->get(key);
	int result = storage_getter(key, curr_node, 0) ;


	//if the result is not found on the first level, try the lower levels
	while(result == NOT_FOUND && curr_node->next_node != nullptr && get_level <curr_fill_level) {

		get_level++;
		curr_node = curr_node->next_node;
		if (verbose) std::cout<<"reading from level:"<<get_level<<" level_indexes:"<<curr_node->level_fill-1<<"\n";

		//run the array at a level backwards, because the array at the end
		//are more recent
		get_level_index = curr_node->level_fill-1;
		while(get_level_index >= 0) {
			//result = curr_node->level_storage[get_level_index]->get(key);
			result = storage_getter(key, curr_node, get_level_index);
			if (result != NOT_FOUND)
			{
				break;
			}
			get_level_index--;
		}
	}	

	if (result == DELETED) {
		return NOT_FOUND;
	}

	return result;

}

bool LSM_mt::remove(int key) {

	return insert(key, DELETED);
}

bool LSM_mt::update(int key, int value){

}

void LSM_mt::bulkload(std::pair<keyValue*, int>* k_lists, int k) {

}


void LSM_mt::deleteAll() {

}

std::pair<keyValue*, int> LSM_mt::transferAll() {

}

LSM_mt::~LSM_mt(){
	
	StorageLL_Node* curr_node = lsm_storage_head;
	delete curr_node->level_storage[0];
	delete[] curr_node->level_storage;

	StorageLL_Node* temp = curr_node;
	while (curr_node ->next_node != nullptr)
	{
		temp = curr_node;
		curr_node = curr_node ->next_node;
		for (int y = 0; y< ratio; y++){
			curr_node->level_storage[y]->closeFile();
			delete curr_node->level_storage[y];

		}
		delete[] curr_node->storagelocks;
		delete[] curr_node->level_storage;
		delete temp;
	}
	delete curr_node;
}	

void LSM_mt::closeFile(){}



void LSM_mt::printAll() {
	StorageLL_Node* curr_node = lsm_storage_head;

	int counter = 0;
	while(curr_node != nullptr)
	{
		std::cout<<"Printing Level:"<<counter<<"   "<<curr_node->level_fill<<"\n";
		for (int x =0; x< curr_node->level_fill; x++)
		{
			std::cout<<"Printing Level:"<<counter<<" storage:"<<x<<"\n";
			curr_node->level_storage[x]->printAll();
			std::cout<<"\n\n";
		}
		curr_node = curr_node->next_node;
		counter++;
	}
}