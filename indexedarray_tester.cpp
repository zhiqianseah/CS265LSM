#include "indexedarray.h"
#include <iostream> 
#include <cstdlib>



//Input Parameters
int main(int argc, char* argv[]) {
	std::cout<<"Starting indexedarray tester\n";

	if (argc < 2) {
		std::cout<<"Insufficient number of parameters. Exiting.";
		return 0;
	}



	int num_inputs = 8192*3;
	keyValue* bulk = new keyValue[num_inputs];
	for (int x = 0; x< num_inputs; x++)
	{
		bulk[x].key = x*2+1;
		bulk[x].value = x*2+1;
	}

	keyValue* bulk2 = new keyValue[num_inputs];
	for (int x = 0; x< num_inputs; x++)
	{
		bulk2[x].key = x*2;
		bulk2[x].value = x*2;
	}


	std::string filepath = std::string(FOLDERPATH) + "LSM1_0.bin";
	IndexedArray BPT(num_inputs*2, filepath.c_str());
	Storage *storage = &BPT;



	std::pair<keyValue*, int> bulkload_input[1];
	bulkload_input[0].first = bulk;
	bulkload_input[0].second = num_inputs;
	bulkload_input[1].first = bulk2;
	bulkload_input[1].second = num_inputs;

	storage->bulkload(bulkload_input, 2);

	std::cout<< storage->get(-10) <<"\n";

	std::cout<< storage->get(3) <<"\n";

	std::cout<< storage->get(8192*3) <<"\n";
	std::cout<< storage->get(8192*3-1) <<"\n";
	std::cout<< storage->get(8192*3+1) <<"\n";

	std::cout<< storage->get(8192*4) <<"\n";
	std::cout<< storage->get(8192*4-1) <<"\n";
	std::cout<< storage->get(8192*4+1) <<"\n";

	std::cout<< storage->get(8192*5) <<"\n";
	std::cout<< storage->get(8192*5-1) <<"\n";
	std::cout<< storage->get(8192*5+1) <<"\n";


	std::cout<< storage->get(8192*10) <<"\n";
 /*

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


	std::cout<<"Ending indexedarray tester \n";
}
