#include "sortedarray.h"
#include <iostream> 
#include <cstdlib>




int main(int argc, char* argv[]) {
	std::cout<<"Starting LSM\n";

	if (argc < 2) {
		std::cout<<"Insufficient number of parameters. Exiting.";
		return 0;
	}

	SortedArray SA(atoi(argv[1]));
	Storage *storage = &SA;


	storage->insert(1, 5);
	storage->insert(3, 2);
	storage->insert(6, 8);

	std::cout<< *storage->get(3) <<"\n";

	std::cout<<"Ending LSM\n";
}
