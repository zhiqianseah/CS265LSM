//Adapted from http://stackoverflow.com/questions/8635963/read-write-lock-implementation-using-mutex-only

#include <mutex>         

using std::mutex;

class ReadWriteLock {
    mutex writeLock;
    mutex readLock;
    int readCount;
public:
    ReadWriteLock() {
        readCount = 0;
    }
    void LockWrite() {
        writeLock.lock();
    }
    void UnlockWrite() {
        writeLock.unlock();
    }
    void LockRead() {
        lock_guard<mutex> lock(readLock);
        ++readCount;
        if (1 == readCount) {
            LockWrite();
        }
    }
    void UnlockRead() {
        lock_guard<mutex> lock(readLock);
        --readCount;
        if (0 == readCount) {
            UnlockWrite();
        }
    }
};