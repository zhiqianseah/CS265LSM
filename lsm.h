#include "storage.h"


class LSM : public Storage {
	private:
		Storage** lsm_storage;

		//Threshold of a LSM level before rolling merge. between 0 and 1
		int threshold;
		int levels;


	public:

		/*Constructor.
		input variables:
			levels = number of levels in the LSM tree 
			level_types = the type of storage for each level
			ratio: the size ratio of each level

		*/
		LSM(int levels, int* level_types, int c0_size, int ratio, int threshold_percentage);

		bool insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);

		void bulkload(keyValue* input, int size);

		std::pair<keyValue*, int> transferPage();

		void deletePage();
		
		~LSM();	
};