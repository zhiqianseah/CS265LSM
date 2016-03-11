#include "sortedarray.h"
#include <iostream> 
#include <cstdlib>



//Input Parameters
int main(int argc, char* argv[]) {
	std::cout<<"Starting sorted tester\n";

	if (argc < 2) {
		std::cout<<"Insufficient number of parameters. Exiting.";
		return 0;
	}

	std::string filepath = std::string(FOLDERPATH) + "LSM.bin";
	SortedArray SA(atoi(argv[1]), filepath.c_str());
	Storage *storage = &SA;
 

	storage->insert(1, 5);
	storage->insert(3, 2);
	storage->insert(6, 8);
	storage->insert(5, 8);
	storage->insert(2, 8);
	storage->insert(4, 8);
	SA.print_all();


	storage->remove(3);
	storage->remove(6);

	storage->insert(10, 8);
	SA.print_all();
	keyValue bulk[3];

	bulk[0].key = 3;
	bulk[0].value = 1;
	bulk[1].key = 5;
	bulk[1].value = 1;
	bulk[2].key = 7;
	bulk[2].value = 1;

	storage->bulkload(bulk, 3);
	SA.print_all();
	std::cout<<std::endl;


	bulk[0].key = -7;
	bulk[0].value = 1;
	bulk[1].key = -5;
	bulk[1].value = 1;
	bulk[2].key = -3;
	bulk[2].value = 1;

	storage->bulkload(bulk, 3);
	SA.print_all();

/*
	std::cout<< storage->get(3) <<"\n";

	//this should return NOT_FOUND
	std::cout<< storage->get(5) <<"\n";

	//this should return 0
	std::cout<< storage->insert(9, 10) <<"\n";	

	//remove 1 element to clear up space
	std::cout<< storage->remove(3) <<"\n";	

	//this should return 1
	std::cout<< storage->insert(9, 10) <<"\n";	
*/
	std::cout<<"Ending sorted tester \n";
}
