///////////////////////////////////////////////////////////////////
//
// ADT Priority Queue
//
// Abstract ουρά προτεραιότητας. Σε κάθε remove αφαιρείται το μεγαλύτερο
// στοιχείο (με βάση τη συνάρτηση compare).
//
// Σε σχέση με την "κλασική" ουρά προτεραιότητας του lecture-code έχει προστεθεί
// η δυνατότητα διαγραφής οποιουδήποτε στοιχείου (pqueue_remove) μέσω της
// λογικής των "κόμβων" που επιστρέφει η pqueue_insert.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"
#include "ADTVector.h"


// Μία ουρά προτεραιότητας αναπαριστάται από τον τύπο PriorityQueue

typedef struct priority_queue* PriorityQueue;
typedef struct priority_queue_node* PriorityQueueNode;


// Δημιουργεί και επιστρέφει μια νέα ουρά προτεραιότητας, της οποίας τα στοιχεία συγκρίνονται με βάση τη συνάρτηση compare.
// Αν destroy_value != NULL, τότε καλείται destroy_value(value) κάθε φορά που αφαιρείται ένα στοιχείο.
// Αν values != NULL, τότε η ουρά αρχικοποιείται με τα στοιχεία του Vector values.

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η ουρά pqueue

int pqueue_size(PriorityQueue pqueue);

// Επιστρέφει το μεγαλύτερο στοιχείο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

Pointer pqueue_max(PriorityQueue pqueue);

// Επιστρέφει το μεγαλύτερο κόμβο της ουράς (μη ορισμένο αποτέλεσμα αν η ουρά είναι κενή)

PriorityQueueNode pqueue_max_node(PriorityQueue pqueue);

// Προσθέτει την τιμή value στην ουρά pqueue, και επιστρέφει τον κόμβο που περιέχει το νέο στοιχείο.

PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value);

// Αφαιρεί το στοιχείο του κόμβου node (μη ορισμένο αποτέλεσμα αν o κόμβος δεν ανήκει στην ουρά).

void pqueue_remove(PriorityQueue pqueue, PriorityQueueNode node);

// Επιστρέφει το στοιχείο που περιέχει ο κόμβος node

Pointer pqueue_node_value(PriorityQueue pqueue, PriorityQueueNode node);

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η ουρά pqueue.
// Οποιαδήποτε λειτουργία πάνω στη ουρά μετά το destroy είναι μη ορισμένη.

void pqueue_destroy(PriorityQueue pqueue);
