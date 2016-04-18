#include <iostream>
#include <boost/thread.hpp>
#include <random>
#include <functional>
#include <string>
using namespace std;

const int write_percentage = 30;
const int size = 10;
int data[size];


boost::shared_mutex _access;

boost::mutex printlock;
int reader(int key)
{
  // get shared access
  boost::shared_lock<boost::shared_mutex> lock(_access);
  // now we have shared access
  return data[key];

}


void writer(int key, int value)
{
  // get upgradable access
  boost::upgrade_lock<boost::shared_mutex> lock(_access);

  // get exclusive access
  boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
  // now we have exclusive access


  data[key] = value;
}


void print_output(int thread_id, string output)
{
  	boost::mutex::scoped_lock l(printlock);
  	cout<<"Thread "<< thread_id<<": "<<output<<"\n";	
}

void child_thread(int id) 
{

	//initialize random number generators 
 	std::mt19937 random_number_engine(id+103);
 	std::uniform_int_distribution<int> int_dist(1, size);

 	std::mt19937 random_percentage_engine(id);
 	std::uniform_int_distribution<int> percent_dist(1, 100);

	//auto dice_roller = std::bind(int_dist, random_number_engine);
	//int random_roll= dice_roller();  // Generate one of the integers 1,2,3,4,5,6.
 	int random_number;
 	int percentage;

 	
	for (int x = 0; x< 2; x++)
	{
		//draw a random number to see if the action is a read or a write
		
		percentage = percent_dist(random_percentage_engine);
		if (percentage < write_percentage)
		{
			random_number = int_dist(random_number_engine);
			writer (random_number, x*random_number);
			print_output(id, "writing "+to_string(x*random_number)+" to"+to_string(random_number));
		}
		else
		{
			random_number = int_dist(random_number_engine);
			print_output(id, "reading "+to_string(reader(random_number) ) +" from "+to_string(random_number));
		}
	}
	
}

int main() {

	for (int x =0; x< size; x++)
	{
		data[x] = -x;
	}

	boost::thread_group group;
	for (int i = 0; i < 10; ++i)
		group.add_thread(new boost::thread(child_thread, i));
	    //group.create_thread(reader,i);
	group.join_all();
	/*
    cout << "1" << endl;
    SharedLock lock1(tempLock); // (1) aquire a shared lock
    cout << "2" << endl;
    boost::thread tempThread(main2);
    cout << "3" << endl;
    boost::this_thread::sleep(boost::posix_time::seconds(3));
    cout << "4" << endl;
    SharedLock lock3(tempLock); // (3) try getting antoher shared lock, deadlock here
    cout << "5" << endl;
    lock1.unlock();
    lock3.unlock();
    */
    return 0;
}