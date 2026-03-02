///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT KPQ μέσω του ADT Set
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTKPQ.h"
#include "ADTSet.h"

struct kpq{
    Set k_set; // Σε αυτό το set κρατάμε τα k μεγαλύτερα στοιχεία
    Set main_set; // Σε αυτο το set κρατάμε τα υπόλοιπα στοιχεία
    CompareFunc compare;
    DestroyFunc destroy_value;
    int k;
};

KPQ kpq_create(CompareFunc compare, int k, DestroyFunc destroy_value){
    assert(compare != NULL);

    KPQ kpq = malloc(sizeof(*kpq));
    kpq->compare = compare;
    kpq->destroy_value = destroy_value;

    kpq->k = k;

    kpq->k_set = set_create(compare,destroy_value);
    kpq->main_set = set_create(compare,destroy_value);

	return kpq;
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η ουρά kpq

int kpq_size(KPQ kpq){
    return set_size(kpq->k_set) + set_size(kpq->main_set);
}

// Επιστρέφει το μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

Pointer kpq_max(KPQ kpq){
    // Το μέγιστο στοιχείο της kpq είναι το μέγιστο της k_set
    return set_node_value(kpq->k_set,set_last(kpq->k_set));
}

// Επιστρέφει το k-οστό μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι λιγότερα από k στοιχεία)

Pointer kpq_k_th(KPQ kpq){
    // Το k-οστό στοιχείο της kpq είναι το k-οστό στοιχείο της k_set
    // (το οποίο είναι το min της k_set)
    return set_node_value(kpq->k_set,set_first(kpq->k_set));
}

// Προσθέτει την τιμή value στην ουρά kpq.

void kpq_insert(KPQ kpq, Pointer value){
    set_insert(kpq->k_set,value);

    // Θέτουμε ως destroy_value της k_set NULL
    // Ώστε να μην χαθεί το value που αλλάζουμε από την k_set στη main_set
    DestroyFunc old = set_set_destroy_value(kpq->k_set,NULL);

    if(set_size(kpq->k_set) > kpq->k){
        Pointer kth = kpq_k_th(kpq);
        set_remove(kpq->k_set,kth);
        set_insert(kpq->main_set,kth);
    }
    // Θέτουμε ξανά το destroy_value της k_set
    set_set_destroy_value(kpq->k_set,old);
}

// Αφαιρεί το μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

void kpq_remove_max(KPQ kpq){

    set_remove(kpq->k_set,kpq_max(kpq));
    // Θέτουμε ως destroy_value της k_set NULL
    // Ώστε να μην χαθεί το value που αλλάζουμε από την main_set στη k_set
    DestroyFunc old = set_set_destroy_value(kpq->main_set,NULL);

    if(set_size(kpq->main_set) > 0){
        Pointer max = set_node_value(kpq->main_set,set_last(kpq->main_set));
        set_remove(kpq->main_set,max);
        set_insert(kpq->k_set,max);
    }

    set_set_destroy_value(kpq->main_set,old);
}

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc kpq_set_destroy_value(KPQ kpq, DestroyFunc destroy_value){
   	DestroyFunc old = kpq->destroy_value;
	kpq->destroy_value = destroy_value;
    set_set_destroy_value(kpq->k_set,destroy_value);
    set_set_destroy_value(kpq->main_set,destroy_value);
    return old;
}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η ουρά kpq.
// Οποιαδήποτε λειτουργία πάνω στη ουρά μετά το destroy είναι μη ορισμένη.

void kpq_destroy(KPQ kpq){
    // Ελευθερώνουμε την μνήμη που δεσμεύει η k_set και main_set
    // με την destroy_value που δώσαμε στην αρχή
    set_set_destroy_value(kpq->k_set,kpq->destroy_value);
    set_set_destroy_value(kpq->main_set,kpq->destroy_value);
    set_destroy(kpq->k_set);
    set_destroy(kpq->main_set);
    free(kpq);
}