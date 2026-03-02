#include <stdlib.h>
#include <assert.h>

#include "ADTDEPQ.h"
#include "ADTKPQ.h"
#include "ADTPriorityQueue.h"

struct kpq{
    DEPQ depq;
    PriorityQueue pqueue;
    CompareFunc compare;
    DestroyFunc destroy_value;
    int k;
};

KPQ kpq_create(CompareFunc compare, int k, DestroyFunc destroy_value){
    assert(compare != NULL);

	KPQ kpq = malloc(sizeof(*kpq));
	kpq->compare = compare;
	kpq->destroy_value = destroy_value;

    kpq->k=k;

    kpq->depq = depq_create(compare,destroy_value);

    kpq->pqueue = pqueue_create(compare,destroy_value,NULL);


	return kpq;
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η ουρά kpq

int kpq_size(KPQ kpq){
    // Το size της kpq είναι το size της depq + το size της pqueue
    return depq_size(kpq->depq) + pqueue_size(kpq->pqueue);
}

// Επιστρέφει το μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

Pointer kpq_max(KPQ kpq){
    // Το μέγιστο στοιχείο της kpq είναι το μέγιστο της depq
    return depq_max(kpq->depq);
}

// Επιστρέφει το k-οστό μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι λιγότερα από k στοιχεία)

Pointer kpq_k_th(KPQ kpq){
    // Το k-οστό στοιχείο της kpq είναι το k-οστό στοιχείο της depq
    // (το οποίο είναι το min της depq)
    return depq_min(kpq->depq);
}

// Προσθέτει την τιμή value στην ουρά kpq.

void kpq_insert(KPQ kpq, Pointer value){
    // Θέτουμε ως destroy_value της depq NULL
    // Ώστε να μην χαθεί το value που αλλάζουμε από την depq στη pqueue
    DestroyFunc destroy = depq_set_destroy_value(kpq->depq,NULL);

    depq_insert(kpq->depq,value);

    if(depq_size(kpq->depq) > kpq->k){
        Pointer min = depq_min(kpq->depq);
        depq_remove_min(kpq->depq);
        pqueue_insert(kpq->pqueue,min);
    }
    // Θέτουμε ξανά το destroy_value της depq
    depq_set_destroy_value(kpq->depq,destroy);
}

// Αφαιρεί το μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

void kpq_remove_max(KPQ kpq){
    // Θέτουμε ως destroy_value της pqueue NULL
    // Ώστε να μην χαθεί το value που αλλάζουμε από την pqueue στη depq
    DestroyFunc destroy = pqueue_set_destroy_value(kpq->pqueue,NULL);

    depq_remove_max(kpq->depq);

    if(pqueue_size(kpq->pqueue) != 0){
        Pointer max = pqueue_max(kpq->pqueue);
        pqueue_remove(kpq->pqueue,pqueue_max_node(kpq->pqueue));
        depq_insert(kpq->depq,max);
    }
    // Θέτουμε ξανά το destroy_value της pqueue
    pqueue_set_destroy_value(kpq->pqueue,destroy);
}

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc kpq_set_destroy_value(KPQ kpq, DestroyFunc destroy_value){
   	DestroyFunc old = kpq->destroy_value;
	kpq->destroy_value = destroy_value;
    depq_set_destroy_value(kpq->depq,destroy_value);
    pqueue_set_destroy_value(kpq->pqueue,destroy_value);
    return old;
}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η ουρά kpq.
// Οποιαδήποτε λειτουργία πάνω στη ουρά μετά το destroy είναι μη ορισμένη.

void kpq_destroy(KPQ kpq){
	kpq_set_destroy_value(kpq,kpq->destroy_value);
	depq_destroy(kpq->depq);
    pqueue_destroy(kpq->pqueue);
    free(kpq);
}