#include "basicarray.h"
#include "btree.h"
#include <iostream> 
#include <cstdlib>



//Input Parameters
//argv[1] = Size of 
int main(int argc, char* argv[]) {
	std::cout<<"Starting LSM\n";

	if (argc < 2) {
		std::cout<<"Insufficient number of parameters. Exiting.";
		return 0;
	}


	BTree b_tree(2);
	Storage *storage2 = &b_tree;

	storage2->insert(7, 8);
	storage2->insert(1, 5);
	storage2->insert(6, 8);
	storage2->insert(3, 2);
	b_tree.print_all();

	std::cout<<"splitting tree\n";
	storage2->insert(9, 8);

	b_tree.print_all();

	storage2->insert(10, 1);
	storage2->insert(5, 1);

	b_tree.print_all();
/*
	BasicArray SA(atoi(argv[1]));
	Storage *storage = &SA;


	storage->insert(1, 5);
	storage->insert(3, 2);
	storage->insert(6, 8);

	std::cout<< storage->get(3) <<"\n";
	std::cout<< storage->get(5) <<"\n";

*/

	std::cout<<"Ending LSM\n";
}
