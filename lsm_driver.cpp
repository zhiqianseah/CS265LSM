#include "basicarray.h"
#include "btree.h"
#include "lsm.h"
#include <iostream> 
#include <cstdlib>



//Input Parameters
int main(int argc, char* argv[]) {

	int leveltypes[2];
	leveltypes[0] = 0;
	leveltypes[1] = 0;

	LSM lsm(2, leveltypes, 3, 5, 100);
	Storage *storage = &lsm;


	storage->insert(1, 5);
	storage->insert(3, 2);
	storage->insert(6, 8);

	std::cout<< storage->get(3) <<"\n";

	//this should return NOT_FOUND
	std::cout<< storage->get(5) <<"\n";

	//this should return 0
	std::cout<< storage->insert(9, 10) <<"\n";	

	//remove 1 element to clear up space
	std::cout<< storage->remove(3) <<"\n";	

	//this should return 1
	std::cout<< storage->insert(9, 10) <<"\n";	

	//this should return 1
	std::cout<< "Starting mass input\n";	

	for (int x = 0; x<(24577* 100)/100+1; x++) {
		storage->insert(x, x);
	}

	std::cout<< storage->get(1) <<"\n";

	std::cout<<"Ending LSM\n";	
}
