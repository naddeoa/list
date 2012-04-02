/** @file list.h
* @brief Declarations for the C list. The list is a struct
* that represents a linked list.
*/

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/**
* The node struct represents an element of
* a list.
*/
typedef struct node_t{
	struct node_t* prev;
	struct node_t* next;
	void* data;
}node;

/**
* A list struct that contains a length, 
* head node, and tail node. Methods in this
* header operate mostly on this struct.
*/
typedef struct list_t{
	int length;
	node* head;
	node* tail;
}list;

/**
* An iterator for the linked list.
*/
typedef struct iter_t{
	int index;
	list* lst;
	node* current;
}list_iterator;

/**
* The pair type is a simple struct containing
* a key and val char*. This is included as a 
* possible option for the void* with each node.
*/
typedef struct pair_t{
	char* key;
	char* val;
}pair;

/**
* Pop the first item off the list and return it.
* @param lst A pointer to the list to pop from.
* @return A node* pointing to the popped node.
*/
void* pop(list* lst);

/**
* Index into a list and return the data at that
* node.
* @param lst The list to index into.
* @param index The index.
* @return void* to the data in the element at index.
*/
void* get(list* lst, int index);

/**
* Remove an item from the list. This method is similar
* to pop() but you may specify the node index.
* @param lst A pointer to the list to remove from.
* @param index The index of the item to remove.
* @return A node* pointing to the removed node.
*/
void* rm(list* lst, int index);

/**
* Cycle the list, moving the head to the tail and
* making the item after the original head the new head.
* @param lst A pointer to the list to cycle.
*/
void cycle(list* lst);
 
/**
* Append data to a list.
* @param lst A pointer to the list to append to.
* @param data A pointer to the data to append.
*/
void append(list* lst, void* data);


void printList(list* lst);

/**
* Swap two elements in the list. The order of the
* parameters does not matter. That is, index1 does
* not have to be less than index2.
* @param lst The list to swap elements of.
* @param index1 The index of the first element to swap.
* @param index2 The index of the second element to swap.
*/
void swap(list* lst, int index1, int index2);



/////////////// START LIST ITERATOR FUNCTIONS /////////////////


/**
* Create a new iterator for a linked list. The iterator
* will be pointing to the head of the list.
* @param lst A list to generate an iterator for
* @return A pointer to a new list_iterator struct. 
*/
list_iterator* get_iterator(list* lst);

/**
* Iteratore to the next element in a list.
* @param iter A list_iterator* to the list_iterator
* to iterate forward.
* @return The current index of the iterator. If there were
* no more elements in the list then this will not change
* between calls.
*/
int next(list_iterator* iter);

/**
* Iteratore to the previous element in a list.
* @param iter A list_iterator* to the list_iterator
* to iterate backwards.
* @return The current index of the iterator. If there were
* no more elements in the list then this will not change
* between calls.
*/
int prev(list_iterator* iter);

/**
* Get the data of the current element in the list.
* @param iter A list_iterator.
* @return A void* to the data that is inside of the current
* elemenet.
*/
void* data(list_iterator* iter);

/**
* This finds and returns the data inside of the element
* at the specified index. This function is similar to the
* general purpose get() function that operates on a list,
* but it uses an iterator to search more efficiently (as
* opposed to iterating through the entire list one by one 
* each time an element is desired). This function includes
* small optimizations that set the iterator to the head/tail
* if it is faster to start from there, so you will not end
* up iterating backwards through the entire list to get to index
* 0 from the tail. This means that you will not have to use
* get() at all if you are using an iterator.
* @param iter The list_iterator struct that should be used.
* @param index The index to find.
* @return A void* to the data inside of the element at index.
*/
void* iget(list_iterator* iter, int index);

node* irm(list_iterator* iter, int index);
node* ipop(list_iterator* iter);

/**
* Similar to the append method, but it is done through
* an iterator instead. It is always more useful to use
* (this) linked list through an iterator, so you should
* use the methods that take iterators instead of lists
* (like this one).
*/
void iappend(list_iterator* iter, void* data);

/**
* Used for finding alphabetical order. The parameters
* are void* to allow this to be used as one of the comparator
* functions in the bubble_sort. 
* @param str1 The first string to compare.
* @param str2 The second string to compare.
* @return 0 if str1 comes before str2 in alphabetical order.
* 1 if str2 comes before str3 in alphabetical order.
* 2 if they are equal.
*/
int cmp_str(void* str1, void* str2);

/**
* Compare ints to each other to see which one is smaller.
* This method may be passed in as the comparator function
* to a sort.
* @param int1 The first int to compare.
* @param int2 The second int to compare.
* @return 0 if int1 comes before int2 in alphabetical order.
* 1 if int2 comes before int3 in alphabetical order.
* 2 if they are equal.
*/
int cmp_int(void* int1, void* int2);

/**
* Generic function that extracts data from the void* in
* nodes. This is used to enable generic sorts that work
* on any data type, provided a comparator function and
* data extracting function are passed into the sort.
* @param data The void* in a node. This function is tailored
* to the pair* type, extracting the char* key value from the 
* data and returning it as a void* to be passsed into the
* comparator function.
* @return void* Some data inside the void* data within list
* nodes. Currently, the char* key of the pair struct is returned
* as I designed this to sort by key values in pairs.
*/
void* extract_pair(void* data);

/**
* Generic function that extracts data from a node as an int.
* Can be passed in as the data function to a sort.
* @param data The data of a list node.
* @return A void* of the data extracted the way as it should
* be compared in a comparator function.
*/
void* extract_int(void* data);

/**
* Sort a list using one of its iterators.
* @param iter An iterator over the target list
* @param cmp_func A function pointer to a function that compares
* to types. This function should return 0 if the first parameter
* is less than the second parameter, and 1 if the second 
* parameter is less than the first parameter. 2 is returned if they
* are equal. This allows bubble sort in a generic way. Look at 
* the str_order function for a model of how to make your own.
* @param data_func A function that can be used to extract data
* from the void* in each list node. The contents of the void* 
* are unknown so a function must be provided to extract this
* data as another void*, ready to be passed into the cmp_func.
* This function should return the data that you wish to sort by
* in the cmp_func.
*/
void bubble_sort(list_iterator* iter, int(*cmp_func)(void*, void*),
                 void*(*data_func)(void*));


#endif
