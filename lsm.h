#include "storage.h"

struct StorageLL_Node {
	Storage** level_storage;
	StorageLL_Node* next_node;
	int level_fill;
};




class LSM : public Storage {
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
	public:

		/*Constructor.
		input variables:
			levels = number of levels in the LSM tree 
			level_types = the type of storage for each level
			ratio: the size ratio of each level

		*/
		LSM(int level_types, int c0_size, int ratio, bool verbose);

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

		~LSM();	
};