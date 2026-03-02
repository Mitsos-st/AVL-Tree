///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT DEPQ μέσω του ADT Set
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTDEPQ.h"
#include "ADTSet.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί Set

struct depq{
    Set set;
    CompareFunc compare;
    DestroyFunc destroy_value;
};

DEPQ depq_create(CompareFunc compare, DestroyFunc destroy_value){
	assert(compare != NULL);

	DEPQ depq = malloc(sizeof(*depq));
	depq->compare = compare;
	depq->destroy_value = destroy_value;


	depq->set = set_create(compare,destroy_value);


	return depq;
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η ουρά depq
int depq_size(DEPQ depq){
    return set_size(depq->set);
}

// Επιστρέφει το μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)
Pointer depq_max(DEPQ depq){
    return set_node_value(depq->set,set_last(depq->set));
}

// Επιστρέφει το μικρότερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)
Pointer depq_min(DEPQ depq){
    return set_node_value(depq->set,set_first(depq->set));
}

// Προσθέτει την τιμή value στην ουρά depq.
void depq_insert(DEPQ depq, Pointer value){
    set_insert(depq->set,value);
}

// Αφαιρεί την μεγαλύτερη τιμή της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)
void depq_remove_max(DEPQ depq){
    set_remove(depq->set,depq_max(depq));
}

// Αφαιρεί τη μικρότερη τιμή της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)
void depq_remove_min(DEPQ depq){
    set_remove(depq->set,depq_min(depq));
}

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.
DestroyFunc depq_set_destroy_value(DEPQ depq, DestroyFunc destroy_value){
	set_set_destroy_value(depq->set,destroy_value);
	DestroyFunc old = depq->destroy_value;
	depq->destroy_value = destroy_value;
	return old;
}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η ουρά depq.
void depq_destroy(DEPQ depq){
	set_set_destroy_value(depq->set,depq->destroy_value);
	set_destroy(depq->set);
	free(depq);
}

