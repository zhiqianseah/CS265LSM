#include "basicarray.h"
#include "btree.h"
#include "lsm_mt.h"
#include <iostream> 
#include <cstdlib>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <chrono>
#include <fstream>



using namespace std;

Storage *storage;
const int RATIO = 3;
const int INITIAL_PAGES = 10;
const int NUM_THREADS = 8;
const bool VERBOSE = false;

int work_counter = 0;
unsigned workload_total=10;
boost::mutex m;
boost::condition_variable cv;

void task(int operation, int key, int value, int* work_counter)
{

	if (operation == 0){
		storage->insert(key, value);
	}
	else {
		storage->get(key);
	}


    {
        boost::lock_guard<boost::mutex> lk(m);
        ++*work_counter;
    }
    cv.notify_one();
}


//threadpool code taken from
//http://stackoverflow.com/questions/19500404/how-to-create-a-thread-pool-using-boost-in-c
//http://stackoverflow.com/questions/22495402/using-boost-threads-and-io-service-to-create-a-threadpool

double run_test(char* filename)
{

	LSM_mt lsm(2, INITIAL_PAGES, RATIO, VERBOSE);
	storage = &lsm;

	std::ifstream workload_file(filename);

	workload_file>>workload_total;
	work_counter = 0;

	//set up timers
    chrono::time_point<chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();


    boost::asio::io_service ioService;
	boost::thread_group threadpool;

	//this is to start the ioService processing loop and exceute task assigned via ioService.post()
    boost::asio::io_service::work work(ioService);



	//add threads to the thread pool
	for (int i = 0; i< NUM_THREADS; i++)
	{
		threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
	}


	for (int i = 0; i < workload_total; i++)
	{
		int operation;
		int key;
		int value;
		workload_file >> operation >> key >> value;
		ioService.post(boost::bind(&task,operation, key, value, &work_counter));
	}

    {
        boost::unique_lock<boost::mutex> lk(m);
        cv.wait(lk, []{return work_counter == workload_total; });
    }

	ioService.stop();
	threadpool.join_all();


    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    return elapsed_seconds.count();
}


//Input Parameters
int main(int argc, char* argv[]) {



	std::cout<<"Starting LSM\n";


	cout<<"Starting test\n";

	int num_tests = 10;
	double time_result = 0;
	for (int x = 0; x< num_tests; x++)
	{
		time_result = time_result + run_test(argv[1]);
	}

    std::cout << "Average elapsed time for "<<num_tests  <<" runs: " << time_result/num_tests << "s\n";

/*
	cout<<storage->get(3)<<"\n";
	cout<<storage->get(513)<<"\n";
	cout<<storage->get(1024)<<"\n";
	cout<<storage->get(2040)<<"\n";	
*/
	//storage->printAll();

	std::cout<<"Ending LSM\n";	
}
