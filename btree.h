#include "storage.cpp"


class BTree : public Storage {
	private:

		struct keyValue{
			int key;
			int value;
		};

		struct Node {
			//stores of key_value pairs. max size is 2*order
			keyValue* stores;

			//pointer to childen pointers
			Node** children;
			Node* parent;
			int node_fill;
			bool isleaf;
		};

		int fill;
		int order;
		int depth;
		Node* root_node;

		//create a new node. parent is default set to nullptr
		Node* create_node(Node*);

		void insert_into_node(Node* insert_node, int key, int value);

		//recursively finds the leaf node for a given key
		Node* find_leaf(Node* insert_node, int key);

		void split_node(Node* node, int key, int value);
	public:

		//Constructor. Create a sorted array with given input size
		BTree(int order);

		void insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);

		void print_all();
		void print_node(Node* node);
		~BTree();		
};

