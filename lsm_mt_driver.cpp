#include "basicarray.h"
#include "btree.h"
#include "lsm_mt.h"
#include <iostream> 
#include <cstdlib>
#include <boost/thread.hpp>
#include <chrono>

using namespace std;

Storage *storage;
const int RATIO = 2;
const int INITIAL_PAGES = 1;
const int NUM_THREADS = 2;
const bool VERBOSE = false;

void child_thread(int id)
{
	//storage->insert(1, 5);
	//storage->insert(3, 2);
	//storage->insert(6, 8);

	//int onepage = 24576;
	int onepage = 512;
	int num_inputs = onepage *(INITIAL_PAGES+1);
	//cout<<"running thread "<<id<<". Inserting: "<<num_inputs<<" elements\n";
	for (int x = 0; x<num_inputs; x++) {
		storage->insert(x+id*num_inputs, x+id*num_inputs);
	}	
}
//Input Parameters
int main(int argc, char* argv[]) {



	std::cout<<"Starting LSM\n";
	LSM_mt lsm(2, INITIAL_PAGES, RATIO, VERBOSE);

	cout<<"Starting test\n";


	storage = &lsm;

	//set up timers
    chrono::time_point<chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

	boost::thread_group group;
	for (int i = 0; i < NUM_THREADS; ++i)
		group.add_thread(new boost::thread(child_thread, i));
	    //group.create_thread(reader,i);
	group.join_all();


    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

	cout<<storage->get(3)<<"\n";
	cout<<storage->get(513)<<"\n";
	cout<<storage->get(1024)<<"\n";
	cout<<storage->get(2040)<<"\n";

	//storage->printAll();
/*

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
*/
	std::cout<<"Ending LSM\n";	
}
