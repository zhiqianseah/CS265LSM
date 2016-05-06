#include <iostream>
#include <boost/thread.hpp>

using namespace std;

typedef boost::shared_mutex Lock;
typedef boost::unique_lock< Lock > UniqueLock;
typedef boost::shared_lock< Lock > SharedLock;

Lock tempLock;

void main2() {
    cout << "10" << endl;
    UniqueLock lock2(tempLock); // (2) queue for a unique lock
    cout << "11" << endl;
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    lock2.unlock();
}

int main() {
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
    return 0;
}


/*

boost::shared_mutex _access;
void reader()
{
  // get shared access
  boost::shared_lock<boost::shared_mutex> lock(_access);

  // now we have shared access
}

void writer()
{
  // get upgradable access
  boost::upgrade_lock<boost::shared_mutex> lock(_access);

  // get exclusive access
  boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
  // now we have exclusive access
}

*/