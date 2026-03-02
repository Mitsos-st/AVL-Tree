///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω του ADT Set
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"
#include "ADTSet.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί Set


// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	Set set;		// το Set στο οποίο αποθηκεύουμε τα στοιχεία
	CompareFunc compare;
	DestroyFunc destroy_value;
	PriorityQueueNode first; // το πρώτο στοιχείο της ουράς (το μέγιστο)
};

struct priority_queue_node {
	Pointer value;
	PriorityQueue pq;
};

int compare_f(Pointer a,Pointer b){
	PriorityQueueNode ap=a;
	PriorityQueueNode bp=b;
	return ap->pq->compare(bp->value,ap->value); 
}

// Καταστροφή τιμής (καλεί την destroy_value αν υπάρχει και ελευθερώνει τη μνήμη του κόμβου)
void destroy_f(Pointer a){
	PriorityQueueNode ap=a;
	DestroyFunc destroy = ap->pq->destroy_value;
	if(destroy != NULL) destroy(ap->value);
	free(ap);
}


// Συναρτήσεις του ADTPriorityQueue ////////////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	assert(compare != NULL);

	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	pqueue->compare = compare;
	pqueue->destroy_value = destroy_value;

	pqueue->first = NULL;

	// Δημιουργούμε το Set που θα αποθηκεύει τους κόμβους της ουράς
	pqueue->set = set_create(compare_f,destroy_f);
	// Αν values != NULL, τότε αρχικοποιούμε την ουρά με τα στοιχεία του Vector
	if(values != NULL){
		for(int i=0; i<vector_size(values); i++){
			pqueue_insert(pqueue,vector_get_at(values,i));
		}
	}

	return pqueue;
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η ουρά pqueue
int pqueue_size(PriorityQueue pqueue) {
	return set_size(pqueue->set);
}

// Επιστρέφει το μεγαλύτερο στοιχείο της ουράς
Pointer pqueue_max(PriorityQueue pqueue) {
	if(pqueue->first == NULL) return NULL;
	return pqueue->first->value;
}

// Επιστρέφει το μεγαλύτερο κόμβο της ουράς
PriorityQueueNode pqueue_max_node(PriorityQueue pqueue) {
	return pqueue->first;
}

PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value) {
	// Δημιουργούμε τον κόμβο που θα αποθηκεύει το στοιχείο
	PriorityQueueNode node = malloc(sizeof(*node));
	node->pq=pqueue;
	node->value=value;
	set_insert(pqueue->set,node);
	// Ενημερώνουμε τον πρώτο κόμβο της ουράς
	pqueue->first = set_node_value(pqueue->set,set_first(pqueue->set));
	return node;
}

void pqueue_remove(PriorityQueue pqueue, PriorityQueueNode node) {
	// Αφαιρούμε τον κόμβο από την ουρά
	set_remove(pqueue->set,node);
	// Ενημερώνουμε τον πρώτο κόμβο της ουράς
	// Αν το set είναι κενό, τότε το πρώτο στοιχείο είναι NULL
	SetNode s_first = set_first(pqueue->set);
	if(s_first == SET_BOF) pqueue->first = NULL;
	else pqueue->first = set_node_value(pqueue->set,s_first);
}

// Επιστρέφει το στοιχείο που περιέχει ο κόμβος node
Pointer pqueue_node_value(PriorityQueue pqueue, PriorityQueueNode node) {
	return node->value;
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	DestroyFunc old = pqueue->destroy_value;
	pqueue->destroy_value = destroy_value;
	set_set_destroy_value(pqueue->set,destroy_f);
	return old;
}

void pqueue_destroy(PriorityQueue pqueue) {
	pqueue_set_destroy_value(pqueue,pqueue->destroy_value);
	set_destroy(pqueue->set);
	free(pqueue);
}
