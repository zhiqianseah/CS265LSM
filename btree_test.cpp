#include "basicarray.h"
#include "btree.h"
#include <iostream> 
#include <cstdlib>



//Input Parameters
int main(int argc, char* argv[]) {
	std::cout<<"Starting LSM\n";

	if (argc < 2) {
		std::cout<<"Insufficient number of parameters. Exiting.";
		return 0;
	}

/*
	BTree b_tree(2);
	Storage *storage2 = &b_tree;

	storage2->insert(7, 8);
	storage2->insert(1, 5);
	storage2->insert(6, 8);
	storage2->insert(3, 2);
	b_tree.print_all();

	storage2->insert(19,1);
	b_tree.print_all();

*/



	BTree b_tree(1);
	Storage *storage2 = &b_tree;

	storage2->insert(7, 8);
	storage2->insert(1, 5);
	storage2->insert(6, 8);
	storage2->insert(3, 2);
	b_tree.print_all();

	std::cout<<"splitting tree\n";
	storage2->insert(15, 8);

	storage2->insert(16, 1);
	b_tree.print_all();
	std::cout<<"Adding more elements\n\n";


	//storage2->insert(2, 1);


	//storage2->insert(8, 1);
	//storage2->insert(9, 1);


	storage2->insert(17,1);
	storage2->insert(19,1);
	b_tree.print_all();
	


	std::cout<<"\n";
	std::cout<< storage2->get(17) <<"\n";
	std::cout<< storage2->get(1) <<"\n";












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
