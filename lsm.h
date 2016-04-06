#include "storage.h"


class LSM : public Storage {
	private:
		Storage*** lsm_storage;

		//total number of levels in the LSM
		int levels;

		//ratio of sizes between levels
		int ratio;


		//current fill up to which level
		int curr_fill_level;

		//current fill up to which index
		int* curr_fill_index_per_level;
	public:

		/*Constructor.
		input variables:
			levels = number of levels in the LSM tree 
			level_types = the type of storage for each level
			ratio: the size ratio of each level

		*/
		LSM(int levels, int* level_types, int c0_size, int ratio);

		bool insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);

		void bulkload(std::pair<keyValue*, int>* k_lists, int k);

		
		std::pair<keyValue*, int> transferAll();

		void deleteAll();


		//recursive algorithm to merge level with the level below
		void merge(int level);

		~LSM();	
};