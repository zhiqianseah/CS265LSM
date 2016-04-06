#include "basicarray.h"
#include "btree.h"
#include "lsm.h"
#include <iostream> 
#include <cstdlib>



//Input Parameters
int main(int argc, char* argv[]) {


	int leveltypes[3];
	leveltypes[0] = 1;
	leveltypes[1] = 2;
	leveltypes[2] = 2;

	std::cout<<"Starting LSM\n";
	LSM lsm(3, leveltypes, 3, 3);
	Storage *storage = &lsm;

	std::cout<<"Starting test\n";
	storage->insert(1, 5);
	storage->insert(3, 2);
	storage->insert(6, 8);

	std::cout<< storage->get(3) <<"\n";

	//this should return NOT_FOUND
	std::cout<< storage->get(5) <<"\n";

	//this should return 0
	std::cout<<"inserting (9,10):"<< storage->insert(9, 10) <<"\n";	

	//remove 1 element to clear up space
	std::cout<<"removing 3:"<< storage->remove(3) <<"\n";	


	std::cout<< storage->insert(9, 10) <<"\n";	

	int onepage = 24576;
	int num_inputs = onepage *1;
	std::cout<< "Starting mass input of "<< num_inputs <<" KV\n";	



	for (int x = 0; x<num_inputs+1; x++) {
		storage->insert(x, x);
	}

	std::cout<< storage->get(1) <<"\n";

	for (int x = 0; x<num_inputs+1; x++) {
		storage->insert(x, x+1);
	}
	std::cout<< storage->get(1) <<"\n";

	for (int x = 0; x<num_inputs+1; x++) {
		storage->insert(x, NOT_FOUND);
	}
	std::cout<< storage->get(1) <<"\n";

	for (int x = 0; x<num_inputs+1; x++) {
		storage->insert(x+onepage*2, x+2);
	}
	std::cout<< storage->get(1) <<"\n";

	std::cout<<"Ending LSM\n";	
}
