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
const int ONE_PAGE = 512;
const int RAND_SEED = 0;
//Mutex 
int work_counter = 0;
int workload_total, workload_load;
boost::mutex m;
boost::condition_variable cv;
int rw_total;

//Input Parameters
int INITIAL_PAGES = 1;
int RATIO = 3;
bool VERBOSE = 0;
int NUM_LOAD = 1000000;
int NUM_READ_UPDATE = 1000000;
int READ_PERCENTAGE = 99;
int DISTRIBUTION_TYPE = 0;   // 0 for uniform, 1 for normal
int NORMAL_MEAN = 500000;
int NORMAL_SIGMA = 100000;
int NUM_THREADS = 4;
int num_tests = 1;

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

	//int multiplier = (1- pow(3,LEVELS))/(1-3);
	//workload_load = multiplier * ONE_PAGE;

	workload_load = NUM_LOAD;

	std::default_random_engine normal_generator;
	std::normal_distribution<double> distribution(NORMAL_MEAN,NORMAL_SIGMA);


	//set up timers
    chrono::time_point<chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();


	for (int i = 0; i < workload_load; i++)
	{
		ioService->post(boost::bind(&task,0, i, i, &work_counter));
		rw_total++;
		{
		    boost::unique_lock<boost::mutex> lk(m);
		    cv.wait(lk, []{return work_counter == rw_total; });
		}
	}

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Load time: " << elapsed_seconds.count() <<" ";
   // std::cout <<"Loaded:"<<workload_load<< "\n";


    workload_total = NUM_LOAD + NUM_READ_UPDATE;


    start = std::chrono::system_clock::now();

	for (int i = 0; i < NUM_READ_UPDATE; i++)
	{
		int op = rand() % 100;

		int key;
		//check distribution type
		if (DISTRIBUTION_TYPE == 0)
		{
			key = rand() % workload_load;
		}
		else if (DISTRIBUTION_TYPE == 1)
		{
			key = distribution(normal_generator);
		}


		//this is doing reads
		if (op <READ_PERCENTAGE) {
			ioService->post(boost::bind(&task,1, key, i, &work_counter));
			rw_total++;
		}

		//this is doing writes
		else {

		    {
		        boost::unique_lock<boost::mutex> lk(m);
		        cv.wait(lk, []{return work_counter == rw_total; });
		    }

			ioService->post(boost::bind(&task,0, key, i, &work_counter));			
			rw_total++;
		    {
		        boost::unique_lock<boost::mutex> lk(m);
		        cv.wait(lk, []{return work_counter == rw_total; });
		    }

		}
	}



	{
        boost::unique_lock<boost::mutex> lk(m);
        cv.wait(lk, []{return work_counter == workload_total; });
    }

    end = chrono::system_clock::now();
    elapsed_seconds = end-start;
    std::cout << "Read/Update time: " << elapsed_seconds.count() <<" ";

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

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;

	ioService.stop();
	threadpool.join_all();



    std::cout << "elapsed time: " << elapsed_seconds.count() << "s.   Number of operations:"<<work_counter<<"\n";

    return elapsed_seconds.count();
}

int main(int argc, char* argv[]) {

	if (argc < 12)
	{
		std::cout<<"Insufficient Number of input parameters. Using Default Parameters.\n";

	}
	else
	{
		//Get command line inputs
		INITIAL_PAGES = atoi(argv[1]);
		RATIO = atoi(argv[2]);
		VERBOSE = atoi(argv[3]);
		NUM_LOAD = atoi(argv[4]);
		NUM_READ_UPDATE = atoi(argv[5]);
		READ_PERCENTAGE = atoi(argv[6]);
		DISTRIBUTION_TYPE = atoi(argv[7]);   // 0 for uniform, 1 for normal
		NORMAL_MEAN = atoi(argv[8]);
		NORMAL_SIGMA = atoi(argv[9]);
		NUM_THREADS = atoi(argv[10]);
		num_tests = atoi(argv[11]);

	}

	double time_result = 0;
	for (int x = 0; x< num_tests; x++)
	{
		time_result = time_result + run_test(RAND_SEED);
	}

	std::cout << "Average elapsed time for "<<num_tests  <<" runs: " << time_result/num_tests << "s\n";


}
