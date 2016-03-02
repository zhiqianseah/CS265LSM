//Abstract base class of storage formats 
class Storage {
	public:

		//insert a key-value pair into the storage 
		virtual void insert(int key, int value) = 0;

		//get the value of a key from the storage
		//return nullptr if it is not found
		virtual int* get(int key) = 0;
};