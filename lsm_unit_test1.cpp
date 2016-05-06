#include "lsm.h"
#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <cassert>

int main(int argc, char* argv[]) {

	//TEST CONSTANTS
	int INITIAL_PAGES = 1;
	int RATIO = 2;
	int LEVELS = 2;

	int multiplier = INITIAL_PAGES * (1- pow(RATIO,LEVELS))/(1-RATIO);


	std::cout<<"-------------------Starting unit tests--------------------\n";

	int pagesize = getpagesize();
	//calculate how many KV pairs can be stored in a page
	int KV_in_page = pagesize/sizeof(keyValue) ;

	LSM lsm(2, INITIAL_PAGES, RATIO, false);
	Storage *storage = &lsm;

	std::cout<<"Starting test 1: Loading\n";

	for (int x = 0; x<KV_in_page* multiplier; x++) {
		//std::cout<<"inserting:"<<x<<"\n";
		storage->insert(x, 1);
	}

	//storage->printAll();

	for (int x = 0; x<KV_in_page* multiplier; x++) {
		assert(storage->get(x) == 1);
	}	

	std::cout<<"Starting test 2: Update\n";

	for (int x = 0; x<KV_in_page* multiplier; x=x+3) {
		//std::cout<<"inserting:"<<x<<"\n";
		storage->insert(x, 2);
	}

	for (int x = 0; x<KV_in_page* multiplier; x++) {

		if (x%3){
			assert(storage->get(x) == 1);
		}
		else {
			assert(storage->get(x) == 2);			
		}
	}

	std::cout<<"Starting test 3: deletes\n";

	for (int x = 0; x<KV_in_page* multiplier; x=x+7) {
		//std::cout<<"inserting:"<<x<<"\n";
		storage->remove(x);
	}

	for (int x = 0; x<KV_in_page* multiplier; x++) {

		if (!(x%7)){
			assert(storage->get(x) == NOT_FOUND);
		}
		else if (x%3){
			assert(storage->get(x) == 1);
		}
		else {
			assert(storage->get(x) == 2);			
		}
	}




	std::cout<<"Ending LSM\n";	
}
