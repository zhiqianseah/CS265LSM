#include "heap_merge_k_list.h"


//private functions for the heap
void heap_merge_k_list::min_heap(HeapNode* starting_nodes, int k){

}

int heap_merge_k_list::left_child (int parent) {
	return parent*2 + 1;
}

int heap_merge_k_list::right_child (int parent) {
	return parent*2 + 2;
}

HeapNode heap_merge_k_list::get_min() {
	return Heap[0];
}

void heap_merge_k_list::replace_min(HeapNode new_node) {

	Heap[0] = new_node;
	heapify(0);
}

void heap_merge_k_list::heapify (int parent){
	int left = left_child(parent);
	int right = right_child(parent);

	int smallest = parent;

	//check bounds, and if the child is smaller than the parent
	//TODO: duplicates???
	if (left < size && Heap[left].KV.key <Heap[smallest].KV.key ){
		smallest = left;
	}
	if (right < size && Heap[right].KV.key <Heap[smallest].KV.key ){
		smallest = right;
	}
	if (smallest != parent) {
		swap (&Heap[smallest], &Heap[parent]);
		heapify(smallest);
	}

}

void heap_merge_k_list::swap(HeapNode* first, HeapNode* second) {
	HeapNode temp = *second;
	*second = *first;
	*first = temp;
}


heap_merge_k_list::heap_merge_k_list(std::pair<keyValue*, int>* k_lists, int k) {
	size = k;
	Heap = new HeapNode[k];
	k_sorted_lists = k_lists;
	total_keys = 0;

	//store the first elements in the heap
	for (int x = 0; x< size; x++) {
		Heap[x].KV.key = k_lists[x].first[0].key;
		Heap[x].KV.value = k_lists[x].first[0].value;
		Heap[x].sorted_list = x;
		Heap[x].sorted_list_index = 0;
		total_keys = total_keys + k_lists[x].second;

	}

    int i = (size - 1)/2;
    while (i >= 0)
    {
        heapify(i);
        i--;
    }
}



int heap_merge_k_list::merge(keyValue* dest, int max_size){

	std::cout<<"total keys are:"<<total_keys<<"\n";

	int added_keys = 0;
	for (int x = 0; x<total_keys; x++)
	{
		HeapNode min_node = get_min();
		dest[x].key = min_node.KV.key;
		dest[x].value = min_node.KV.value; 
		added_keys++;


		//Get the next entry from the list where min_node comes from
		if (min_node.sorted_list_index+1 < k_sorted_lists[min_node.sorted_list].second)
		{
			min_node.sorted_list_index++;
			min_node.KV.key = k_sorted_lists[min_node.sorted_list].first[min_node.sorted_list_index].key;
			min_node.KV.value = k_sorted_lists[min_node.sorted_list].first[min_node.sorted_list_index].value;
		}
		else
		{
			min_node.KV.key = INT_MAX; 
		}

		replace_min(min_node);
	}


	return added_keys;
}

heap_merge_k_list::~heap_merge_k_list() {
	delete Heap;
}