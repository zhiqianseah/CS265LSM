#include "basicarray.h"
#include "btree.h"
#include "lsm.h"
#include <iostream> 
#include <cstdlib>



//Input Parameters
int main(int argc, char* argv[]) {

	std::cout<<"Starting LSM\n";
	LSM lsm(2, 3, 3, true);
	Storage *storage = &lsm;

	std::cout<<"Starting test\n";
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


	std::cout<< storage->insert(9, 10) <<"\n";	

	int onepage = 24576;
	int num_inputs = onepage *4;
	std::cout<< "Starting mass input of "<< num_inputs <<" KV\n";	



	for (int x = 0; x<num_inputs+1; x++) {
		storage->insert(x, x);
	}

	std::cout<< storage->get(1) <<"\n";

	std::cout<< storage->get(24576 *3 + 10) <<"\n";


	std::cout<< storage->get(24576 *2 + 10) <<"\n";

	std::cout<< storage->get(24576 *1 + 10) <<"\n";

	std::cout<<"Ending LSM\n";	
}
