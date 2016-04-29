#include "basicarray.h"
#include "btree.h"
#include <iostream> 
#include <cstdlib>



//Input Parameters
int main(int argc, char* argv[]) {
	std::cout<<"Starting basic tester\n";

	if (argc < 2) {
		std::cout<<"Insufficient number of parameters. Exiting.";
		return 0;
	}

	std::string filepath = std::string(FOLDERPATH) + "LSM.bin";
	BasicArray SA(atoi(argv[1]), filepath.c_str());
	Storage *storage = &SA;
 
/*
	storage->insert(8192, 8191);
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
	std::cout<< storage->insert(9, 11) <<"\n";	
*/

	for (int x =0; x< 10; x++)
	{
		storage->insert(x, 2);		
	}
	storage->printAll();

	storage->transferAll();




	storage->printAll();

	std::cout<<"Ending basic tester\n";
}
