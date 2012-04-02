#include "list.h"
#include <stdio.h>
#include <string.h>

/**
* Private function that swaps two adjacent nodes.
* The nodes should be in order, with n1 before n2 in
* the list.
* @param lst The list that these nodes belong to. This is
* needed to update the head and tail pointers if they are
* effected.
* @param n1 The node that occurs earlier in the list.
* @param n2 The node that occurs after n1.
*/
void _swap(list* lst, node* n1, node* n2){
	node* n1Prev = n1->prev;
	node* n1Next = n1->next;
	node* n2Prev = n2->prev;
	node* n2Next = n2->next;

	int adjacent = n1->next == n2 ? 1 : 0;

	if(lst->head == n1)
		lst->head = n2;

	if(lst->tail == n2)
		lst->tail = n1;

	n1->prev = adjacent ? n2 : n2Prev;
	n1->next = n2Next;

	n2->next = adjacent ? n1 : n1Next;
	n2->prev = n1Prev;

	if(n1Prev)
		n1Prev->next = n2;
	if(n1Next && !adjacent)
		n1Next->prev = n2;
	if(n2Next)
		n2Next->prev = n1;
	if(n2Prev && !adjacent)
		n2Prev->next = n1;
}

/**
* Private function that gets a node by index. This 
* function returns the node, whereas teh get() function
* returns the data in the node.
* @param lst A pointer to the list to get from.
* @param index The index.
* @return Pointer to the node at index index
*/
node* _get(list* lst, int index){
	
	if(index >= lst->length || index < 0){
		printf("index %i out of bounds\n", index);
		exit(1);
	}
	
	node* n = lst->head;
	int i;
	for(i=0; i<index; i++){
		n = n->next;
	}

	return n;
}

/**
* Pop the first item off the list and return it.
* This is a private version of pop() that returns
* a node instead of a void* to data. Used to make
* cycling more efficient.
* @param lst A pointer to the list to pop from.
* @return A node* pointing to the popped node.
*/
node* _pop_node(list* lst) {

	if(lst->length == 0){
		printf("called 'pop' on an empty list.\n");
		exit(1);
	}

	node* popped = lst->head;
	lst->head = popped->next; 
	lst->head->prev = 0; //head node has no previous node
	lst->length--;
	return popped;
}

/**
* Private version of the append() function. This
* function deals with nodes instead of data for
* more efficient cycles.
* @param lst A pointer to the list to append to.
* @param n A pointer to the node to append.
*/
void _append_node(list* lst, node* n) {

	if (lst->length == 0) {
		lst->head = n;
		lst->tail = n;
	} else {
		node* oldTail = lst->tail;

		oldTail->next = n;
		n->next = 0;
		n->prev = oldTail;

		lst->tail = n;
	}

	lst->length++;
}


void* pop(list* lst) {

	if(lst->length == 0){
		printf("called 'pop' on list an empty list.\n");
		exit(1);
	}

	node* popped = lst->head;
	lst->head = popped->next;
	lst->head->prev = 0;
	lst->length--;
	return popped->data;
}

void* get(list* lst, int index){
	
	if(index >= lst->length || index < 0){
		printf("index %i out of bounds\n", index);
		exit(1);
	}
	
	node* n = lst->head;
	int i;
	for(i=0; i<index; i++){
		n = n->next;
	}

	return n->data;
}

void* rm(list* lst, int index){

	if(lst->length <= index){
		printf("list does not have that many elements, cannot remove");
		exit(1);
	}

	if(index == 0)
		return pop(lst);
	else{
		int i;
		node* n = lst->head;
		for(i = 0; i < index-1; i++){
			n = n->next;
		}

		node* rm = n->next; // node to be removed
		n->next = rm->next;
		rm->next->prev = n;

		if(index == lst->length-1)
			lst->tail = n;

		lst->length--;
		return rm->data;
	}

}

void cycle(list* lst){
	if(lst->length > 1){
		_append_node(lst, _pop_node(lst));
	}
}


void append(list* lst, void* data) {

	node* newNode = (node*) malloc(sizeof(node));
	newNode->data = data;

	if(lst->length == 0) {
		lst->head = newNode;
		lst->tail = newNode;
	}else{
		node* oldTail = lst->tail;

		oldTail->next = newNode;
		newNode->next = 0;
		newNode->prev = oldTail;

		lst->tail = newNode;
	}

	lst->length++;
}


void printList(list* lst){
	node* n = lst->head;
	int* p;
	printf("[ ");
	int i;
	for(i=0; i<lst->length; i++){
		p = (int*)n->data;
		printf("%i, ", *p);
		n = n->next;
	}
	printf("]\n\n");
}

void swap(list* lst, int index1, int index2){
	if(index1 < 0 || index1 >= lst->length ||
		 index2 < 0 || index2 >= lst->length){
		printf("index out of bounds\n");
		exit(1);
	}
	
	node* n1 = _get(lst, index1);
	node* n2 = _get(lst, index2);
	if(index1 < index2)
		_swap(lst,n1,n2);
	else
		_swap(lst,n2,n1);
}


/////////////// START LIST ITERATOR FUNCTIONS /////////////////


list_iterator* get_iterator(list* lst){
	list_iterator* iter = (list_iterator*)malloc(sizeof(list_iterator));
	iter->current = lst->head;
	iter->lst = lst;
	iter->index = 0;
	return iter;
}

int next(list_iterator* iter){
	if(!iter->current->next)
		return iter->index;
	
	iter->current = iter->current->next;
	iter->index++;
	return iter->index;
}


int prev(list_iterator* iter){
	if(!iter->current->prev)
		return iter->index;
	
	iter->current = iter->current->prev;
	iter->index--;
	return iter->index;
}


void* data(list_iterator* iter){
	return iter->current->data;
}

void* iget(list_iterator* iter, int index){
	//ensure the index is in bounds
	if(index >= iter->lst->length || index < 0){
		printf("index out of bounds\n");
		exit(1);
	}

	//determine if the iterator should move forwards or backwards
	int forward = index > iter->index ? 1 : 0;
	int diff = forward ? index - iter->index : iter->index - index;
	
	//small optimization
	if(!forward && index < diff){
		//it is faster to reset iter to the head
		//and move forward, instead of backwards
		iter->current = iter->lst->head;
		iter->index = 0;
		forward = !forward;
		diff = index;
	}

	//small optimization
	if(forward && (iter->lst->length - index - 1) < diff){
		//it is faster to reset iter to the tail
		//and move backwards, instead of forwards
		iter->current = iter->lst->tail;
		iter->index = iter->lst->length - 1;
		forward = !forward;
		diff = (iter->lst->length - index - 1);
	}

	//create a pointer to either the next() or prev() functions.
	//Which one depends on the index being before or after our
	//iterators current position. 
	int (*action)(list_iterator*) = forward ? next : prev;

	//find the element
	int i;
	for(i=0; i < diff; i++){
		action(iter);
	}
	
	return data(iter);
}

node* irm(list_iterator* iter, int index){
	
	rm(iter->lst, index);

}

void iappend(list_iterator* iter, void* data){
	append(iter->lst, data);

	if(iter->lst->length == 1)
		iter->current = iter->lst->head;
}

int cmp_str(void* vstr1, void* vstr2){
	//cast the void* to char*. This function only
	//compares strings
	char* str1 = (char*)vstr1;
	char* str2 = (char*)vstr2;
	int i;
	//compare the strings char by char
	for(i = 0; str1[i] != 0 && str2[i] != 0; i++){
		if(str1[i] < str2[i])
			return 0;
		else if(str1[i] > str2[i])
			return 1;
	}

	//if it got this far, then the shorter of the
	//two strings must be the smaller one alphabetically
	if(strlen(str1) > strlen(str2))
		return 1;
	else if(strlen(str2) > strlen(str1))
		return 0;
	else //they are equal
		return 2;
}

int cmp_int(void* vint1, void* vint2){
	int* int1 = (int*)vint1;
	int* int2 = (int*)vint2;

	if(*int1 < *int2)
		return 0;
	else if(*int1 > *int2)
		return 1;
	else
		return 2;
}


void* extract_pair(void* data){
	pair* p = (pair*)data;
	return p->key;
}

void* extract_int(void* data){
	int* i = (int*)data;
	return i;
}

void bubble_sort(list_iterator* iter, int(*cmp_func)(void*,void*),
                void*(*data_func)(void*)){
	int i,j, index = iter->index;
	void* p1;
	void* p2;
	for(i = 0; i < iter->lst->length-1; i++){
		for(j = 0; j < iter->lst->length-1; j++){
			iget(iter, j);
			printf("index: %i\n",iter->index);	
			p1 = data_func(iter->current->data);
			p2 = data_func(iter->current->next->data);
			if( cmp_func( p1, p2)){
				_swap(iter->lst, iter->current, iter->current->next);
				iter->index++;
			}
		}
	}
	iget(iter,index);//restore the iterator to its original index

}



