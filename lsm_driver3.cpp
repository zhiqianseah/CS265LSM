#include "basicarray.h"
#include "btree.h"
#include "lsm.h"
#include <iostream> 
#include <cstdlib>
#include <cmath>


//Input Parameters
int main(int argc, char* argv[]) {


	std::cout<<"Starting LSM\n";
	LSM lsm(2, 3, 3, false);
	Storage *storage = &lsm;


	std::cout<<"Starting test\n";



	int onepage = 8192;

	int levels = 7;

	int multiplier = (1- pow(3,levels))/(1-3);
	std::cout<< "Multiplier is "<< multiplier <<"\n";	
	int num_inputs = onepage *3*(multiplier);

	std::cout<< "Starting mass input of "<< num_inputs <<" KV\n";	



	for (int x = 0; x<num_inputs; x++) {
		storage->insert(x, x);
	}

	std::cout<< storage->get(1) <<"\n";
/*
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
*/
	std::cout<<"Ending LSM\n";	
}