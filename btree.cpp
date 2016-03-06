#include "btree.h"
#include <iostream>

BTree::BTree(int order_input){
	order = order_input;
	depth = 0;  //depth of the btree
	fill = 0;


	//create a new node and assign it to root
	//root node does not have a parent. set as nullptr
	root_node = create_node(nullptr);

}

void BTree::insert(int key, int value){

	//node to insert our element. start at the root node
	Node* insert_node = find_leaf(root_node, key);

	std::cout<<"depth:"<<depth<<std::endl;


	//navigate to the leaf of the BTree
	/*
	for (int x = 0; x< depth; x++) {
		std::cout<<"nodefill:"<<insert_node->node_fill<<std::endl;
		for (int y = 0; y<insert_node->node_fill; y++)
		{
			std::cout<<"comparing:"<<insert_node->stores[y].key<<std::endl;
			//find the first node where the input key exceeds the node value
			if (insert_node->stores[y].key > key)
			{
				insert_node = insert_node->children[y];
				break;
			}
			insert_node = insert_node->children[insert_node->node_fill];
		}
	}
	*/
	insert_into_node(insert_node, key, value);

}

BTree::Node* BTree::find_leaf(Node* insert_node, int key){
	if (insert_node->isleaf == true) {
		return insert_node;
	}

	for (int y = 0; y<insert_node->node_fill; y++)
	{
		std::cout<<"comparing:"<<insert_node->stores[y].key<<std::endl;
		//find the first node where the input key exceeds the node value
		if (insert_node->stores[y].key > key)
		{
			return find_leaf(insert_node->children[y], key);
		}
		return find_leaf(insert_node->children[insert_node->node_fill], key);
	}	

}

void BTree::insert_into_node(Node* insert_node, int key, int value) {

	int curr_fill = insert_node->node_fill;
	//if there is space in the node, add it into the node
	if ( curr_fill< 2* order) {

		//run the sorted keys forwards to find the insertion point
		for (int x = 0; x<curr_fill; x++){

			//special case: the key already existed in the node. update the value
			if (insert_node->stores[x].key == key){
				insert_node->stores[x].value = value;
				return;
			}

			//insert the existing KV if the position key is larger than the input KV
			else if (insert_node->stores[x].key > key) {
				//swap the positions of the input KV with the existing KV
				int tempkey = insert_node->stores[x].key;
				int tempvalue = insert_node->stores[x].value;
				insert_node->stores[x].key = key;
				insert_node->stores[x].value = value;
				key = tempkey;
				value = tempvalue;
			}
		}
		insert_node->stores[insert_node->node_fill].key = key;
		insert_node->stores[insert_node->node_fill].value = value;
		insert_node->node_fill++;

	}
	//else if the node is full, then we have to split
	else{
		std::cout<<"NODE IS FULL\n";
		split_node(insert_node, key, value);

	}
}

void BTree::split_node(Node* node, int key, int value){

	Node* sibling_node = create_node(nullptr);

	//get the middle 2 entries
	int middlekey_right = node->stores[order].key;
	int middlekey_left = node->stores[order-1].key;

	//there are 3 possible cases.
	//case 1: the inserted key is less than middlekey_left
	//case 2: the inserted key is in the middle of middlekey_left and middlekey_right
	//case 3: the inserted key is greater than middlekey_right

	//case 1 and 2, we shift half of the KV to the sibling node
	if (key < middlekey_right){

		//transfer half of the KV to the sibling node
		for (int x = order; x< 2*order; x++)
		{
			sibling_node->stores[x-order].key = node->stores[x].key;
			sibling_node->stores[x-order].value = node->stores[x].value;
		} 
		//
		sibling_node->node_fill = order;
		node->node_fill = order;


		//Now, find the key-value to promote up as to the parent node
		//if it is case 1, we take the last entry as the promoted KV
		//and insert the original KV
		if (key < middlekey_left) {
			int tempkey = node->stores[order-1].key;
			int tempvalue = node->stores[order-1].value;
			node->node_fill = order-1;
			insert_into_node(node, key, value);
			key = tempkey;
			value = tempvalue;			
		}

		//if it is case 2, we don't have to do anything!
	}
	else {
		int tempkey = node->stores[order].key;
		int tempvalue = node->stores[order].value;	


		int added = 0;
		//transfer the remaining of the KV to the sibling node
		//while inserting the original KV into the right position
		for (int x = order+1; x< 2*order; x++)
		{

			//check if it the insertion point
			if (added == 0 && node->stores[x].key > key) {
				sibling_node->stores[x-order-1 + added].key = key;
				sibling_node->stores[x-order-1 + added].value = value;
				added++;				
			}

			sibling_node->stores[x-order-1 + added].key = node->stores[x].key;
			sibling_node->stores[x-order-1 + added].value = node->stores[x].value;


		} 	

		if (added == 0) {
			sibling_node->stores[order-1].key = key;
			sibling_node->stores[order-1].value = value;			
		}

		//set sizes of node
		sibling_node->node_fill = order;
		node->node_fill = order;

		key = tempkey;
		value = tempvalue;
	}


	//INVARIANT at this point. 'key' and 'value' are the KV pairs to promote to parent
	std::cout<<"Key Value to be promoted are:"<<key<<"  "<<value<<std::endl;


	//if the root node is being split, we have to add a new level to the tree
	if (node->parent == nullptr) {
		//create a new root node
		Node* new_root = create_node(nullptr);


		//set parent of the child nodes
		node->parent = new_root;
		sibling_node->parent = new_root;

		root_node = new_root;

		insert_into_node(new_root, key, value);

		new_root->isleaf = false;		
		new_root->children[0] = node;
		new_root->children[1] = sibling_node;

		//increase depth of tree
		depth++;

	}
	//if the current node has a parent, add the key value to the parent
	else {

		//code adapted from insert_into_node. with addition of children pointers
		Node* insert_node = node->parent;
		//if the parent node is not full yet, we can add it in
		if (insert_node->node_fill < 2*order)
		{
			bool childfound = 0;
			//find the insertion point
			for (int x =0; x<node->parent->node_fill; x++) {
				if (insert_node->stores[x].key > key) {
					//swap the positions of the input KV with the existing KV
					int tempkey = insert_node->stores[x].key;
					int tempvalue = insert_node->stores[x].value;
					Node* tempnode = insert_node->children[x+1];

					insert_node->stores[x].key = key;
					insert_node->stores[x].value = value;

					//add child node
					insert_node->children[x+1] = sibling_node;
					key = tempkey;
					value = tempvalue;
					sibling_node = tempnode;
				}
			}
				insert_node->stores[insert_node->node_fill].key = key;
				insert_node->stores[insert_node->node_fill].value = value;
				insert_node->children[insert_node->node_fill+1] = sibling_node;

				insert_node->node_fill++;
			
		}

		//if the parent node is full, we have to split the parent node too
		else {
			split_node(node->parent, key, value);
		}
	}
}


int BTree::get(int key){
	return NOT_FOUND;
}

bool BTree::remove(int key){

}

bool BTree::update(int key, int value){

}

BTree::~BTree(){

}

BTree::Node* BTree::create_node(Node* parent){
	//Create a new node
	Node* new_node = new Node;
	new_node->node_fill = 0;
	new_node->stores = new keyValue[2*order];
	new_node->children = new Node*[2*order+1];
	new_node->parent = parent;
	new_node->isleaf = true;
	return new_node;
}

void BTree::print_all(){

	print_node(root_node);
}

void BTree::print_node(Node* node){
	std::cout<<"----Node contains:"<<node->node_fill<<"----\n";
	for (int x =0; x< node->node_fill; x++) {
		std::cout<< node->stores[x].key <<","<<node->stores[x].value<<" ";
	}
	std::cout<<"\n----Node----\n";
	if (!node->isleaf){
		for (int x = 0; x< node->node_fill+1; x++) {
			print_node(node->children[x]);
		}
	}
}