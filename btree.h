#include "storage.h"


class BTree : public Storage {
	private:

		struct Node {
			//stores of key_value pairs. max size is 2*order
			keyValue* stores;

			//pointer to childen pointers
			Node** children;
			Node* parent;
			int node_fill;
			bool isleaf;
		};

		int order;
		int depth;
		Node* root_node;

		//create a new node. parent is default set to nullptr
		Node* create_node(Node*);

		void insert_into_node(Node* insert_node, int key, int value, Node* child_node);

		//recursively finds the leaf node for a given key
		Node* find_leaf(Node* insert_node, int key);

		//For destructor. delete the nodes in post-order traversal manner
		void delete_node(Node* parent);


		//node: the node to be split
		//key, value: the key value to be added to the node 
		//sibling_node: the node that comes from the splitting of a child
		void split_node(Node* node, int key, int value, Node* child_node);
	public:

		//Constructor.
		BTree(int order);

		bool insert(int key, int value);

		int get(int key);

		bool remove(int key);

		bool update(int key, int value);

		void print_all();
		void print_node(Node* node);
		void closeFile();
		~BTree();		
};

