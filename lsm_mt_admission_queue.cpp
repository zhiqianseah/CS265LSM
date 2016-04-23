#include "basicarray.h"
#include "btree.h"
//#include "lsm_mt.h"
#include "lsm.h"
#include <iostream> 
#include <cstdlib>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <chrono>
#include <fstream>
#include <cmath>


using namespace std;

Storage *storage;
const int RATIO = 3;
const int INITIAL_PAGES = 1;
const int NUM_THREADS = 4;
const bool VERBOSE = false;
const int READ_PERCENTAGE = 99;
const int LEVELS = 8;


const int ONE_PAGE = 512;
int work_counter = 0;

int workload_total, workload_load;
const int num_tests = 1;


boost::mutex m;
boost::condition_variable cv;

int rw_total;
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

//post workloads
void post_workload(boost::asio::io_service* ioService) {

	rw_total = 0;

	int multiplier = (1- pow(3,LEVELS))/(1-3);
	workload_load = multiplier * ONE_PAGE;
	for (int i = 0; i < workload_load; i++)
	{
		ioService->post(boost::bind(&task,0, i, i, &work_counter));
		rw_total++;
		{
		    boost::unique_lock<boost::mutex> lk(m);
		    cv.wait(lk, []{return work_counter == rw_total; });
		}
	}

    std::cout <<"Loaded:"<<workload_load<< "\n";



	int read_multiplier = 50;
	workload_total = (read_multiplier+1)*workload_load;


	for (int i = 0; i < workload_load*read_multiplier; i++)
	{
		int op = rand() % 100;
		int key = rand() % workload_load;

		//this is doing reads
		if (op <READ_PERCENTAGE) {
			ioService->post(boost::bind(&task,1, key, i%workload_load, &work_counter));
			rw_total++;
		}

		//this is doing writes
		else {

		    {
		        boost::unique_lock<boost::mutex> lk(m);
		        cv.wait(lk, []{return work_counter == rw_total; });
		    }

			ioService->post(boost::bind(&task,0, key, i%workload_load, &work_counter));			
			rw_total++;
		    {
		        boost::unique_lock<boost::mutex> lk(m);
		        cv.wait(lk, []{return work_counter == rw_total; });
		    }

		}
	}


}

//This function initialized the LSM tree, set up the timers and threadpool.
//The workload itself is handled by post_workload function
//threadpool code taken from
//http://stackoverflow.com/questions/19500404/how-to-create-a-thread-pool-using-boost-in-c
//http://stackoverflow.com/questions/22495402/using-boost-threads-and-io-service-to-create-a-threadpool
double run_test(int rand_seed)
{

	LSM lsm(2, INITIAL_PAGES, RATIO, VERBOSE);
	storage = &lsm;
	srand(rand_seed);
	work_counter = 0;



    boost::asio::io_service ioService;
	boost::thread_group threadpool;

	//this is to start the ioService processing loop and exceute task assigned via ioService.post()
    boost::asio::io_service::work work(ioService);



	//add threads to the thread pool
	for (int i = 0; i< NUM_THREADS; i++)
	{
		threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
	}


	//set up timers
    chrono::time_point<chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
	post_workload(&ioService);

    {
        boost::unique_lock<boost::mutex> lk(m);
        cv.wait(lk, []{return work_counter == workload_total; });
    }

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;

	ioService.stop();
	threadpool.join_all();



    std::cout << "elapsed time: " << elapsed_seconds.count() << "s.   Number of operations:"<<work_counter<<"\n";

    return elapsed_seconds.count();
}


//Input Parameters
int main(int argc, char* argv[]) {



	std::cout<<"Starting LSM\n";
	std::cout<<"Starting test\n";

	
	double time_result = 0;
	for (int x = 0; x< num_tests; x++)
	{
		time_result = time_result + run_test(0);
	}

    std::cout << "Average elapsed time for "<<num_tests  <<" runs: " << time_result/num_tests << "s\n";

	//storage->printAll();

	std::cout<<"Ending LSM\n";	
}
