#include "storage.h"
#include <boost/thread.hpp>



struct StorageLL_Node {
	Storage** level_storage;
	StorageLL_Node* next_node;
	int level_fill;
	boost::shared_mutex* storagelocks;
};




class LSM_mt : public Storage {
	private:

		StorageLL_Node* lsm_storage_head;
		//total number of levels in the LSM
		int levels;

		//ratio of sizes between levels
		int ratio;
		int level_types;
		int c0_size;

		//current fill up to which level
		int curr_fill_level;

		bool verbose;

		int storage_getter(int key, StorageLL_Node* curr_node, int index);

		bool storage_inserter(int key, int value, StorageLL_Node* curr_node, int index);

		void lock_before_transfer(StorageLL_Node* curr_node, int curr_level, int num_lists, int curr_lock_index);

		void Mass_Transfer(StorageLL_Node* curr_node, int curr_level, int num_lists);
	public:

		/*Constructor.
		input variables:
			levels = number of levels in the LSM tree 
			level_types = the type of storage for each level
			ratio: the size ratio of each level

		*/
		LSM_mt(int level_types, int c0_size, int ratio, bool verbose);

		bool insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);

		void bulkload(std::pair<keyValue*, int>* k_lists, int k);

		
		std::pair<keyValue*, int> transferAll();

		void deleteAll();

		void closeFile();
		//recursive algorithm to merge level with the level below
		void merge(StorageLL_Node* curr_node, int level);


		void printAll();
		~LSM_mt();	
};