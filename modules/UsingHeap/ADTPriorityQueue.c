///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω σωρού.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"
#include "ADTVector.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί Vector

// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	Vector nodes;
	Vector values;				// Τα δεδομένα, σε Vector ώστε να έχουμε μεταβλητό μέγεθος χωρίς κόπο
	CompareFunc compare;		// Η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του vector.
};

struct priority_queue_node{
	int index; // Η θέση του κόμβου στο vector
};


// Βοηθητικές συναρτήσεις ////////////////////////////////////////////////////////////////////////////

// Προσοχή: στην αναπαράσταση ενός complete binary tree με πίνακα, είναι βολικό τα ids των κόμβων να
// ξεκινάνε από το 1 (ρίζα), το οποίο απλοποιεί τις φόρμουλες για εύρεση πατέρα/παιδιών. Οι θέσεις
// ενός vector όμως ξεκινάνε από το 0. Θα μπορούσαμε απλά να αφήσουμε μία θέση κενή, αλλά δεν είναι ανάγκη,
// μπορούμε απλά να αφαιρούμε 1 όταν διαβάζουμε/γράφουμε στο vector. Για απλοποίηση του κώδικα, η
// πρόσβαση στα στοιχεία του vector γίνεται από τις παρακάτω 2 βοηθητικές συναρτήσεις.

// Επιστρέφει την τιμή του κόμβου node_id

static Pointer node_value(PriorityQueue pqueue, int node_id) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	return vector_get_at(pqueue->values, node_id - 1);
}

// Ανταλλάσει τις τιμές των κόμβων node_id1 και node_id2

static void node_swap(PriorityQueue pqueue, int node_id1, int node_id2) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	Pointer value1 = node_value(pqueue, node_id1);
	Pointer value2 = node_value(pqueue, node_id2);

	// Ανταλλάσσουμε τις θέσεις των κόμβων στο vector
	PriorityQueueNode node1 = vector_get_at(pqueue->nodes,node_id1-1);
	node1->index = node_id2;
	PriorityQueueNode node2 = vector_get_at(pqueue->nodes,node_id2-1);
	node2->index = node_id1;

	// Ανταλλάσσουμε τις τιμές των κόμβων στο vector
	// Οπότε δεν χρειάζεται να καλέσουμε destroy_value
	vector_set_destroy_value(pqueue->nodes,NULL);
	
	vector_set_at(pqueue->nodes, node_id1 - 1, node2);
	vector_set_at(pqueue->nodes, node_id2 - 1, node1);
	
	vector_set_destroy_value(pqueue->nodes,free);

	vector_set_at(pqueue->values, node_id1 - 1, value2);
	vector_set_at(pqueue->values, node_id2 - 1, value1);
}

// Αποκαθιστά την ιδιότητα του σωρού.
// Πριν: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού, εκτός από
//       τον node_id που μπορεί να είναι _μεγαλύτερος_ από τον πατέρα του.
// Μετά: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού.

static void bubble_up(PriorityQueue pqueue, int node_id) {
	// Αν φτάσαμε στη ρίζα, σταματάμε
	if (node_id == 1)
		return;

	int parent = node_id / 2;		// Ο πατέρας του κόμβου. Τα node_ids είναι 1-based

	// Αν ο πατέρας έχει μικρότερη τιμή από τον κόμβο, swap και συνεχίζουμε αναδρομικά προς τα πάνω
	if (pqueue->compare(node_value(pqueue, parent), node_value(pqueue, node_id)) < 0) {
		node_swap(pqueue, parent, node_id);
		bubble_up(pqueue, parent);
	}
}

// Αποκαθιστά την ιδιότητα του σωρού.
// Πριν: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού, εκτός από τον
//       node_id που μπορεί να είναι _μικρότερος_ από κάποιο από τα παιδιά του.
// Μετά: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού.

static void bubble_down(PriorityQueue pqueue, int node_id) {
	// βρίσκουμε τα παιδιά του κόμβου (αν δεν υπάρχουν σταματάμε)
	int left_child = 2 * node_id;
	int right_child = left_child + 1;

	int size = pqueue_size(pqueue);
	if (left_child > size)
		return;

	// βρίσκουμε το μέγιστο από τα 2 παιδιά
	int max_child = left_child;
	if (right_child <= size && pqueue->compare(node_value(pqueue, left_child), node_value(pqueue, right_child)) < 0)
			max_child = right_child;

	// Αν ο κόμβος είναι μικρότερος από το μέγιστο παιδί, swap και συνεχίζουμε προς τα κάτω
	if (pqueue->compare(node_value(pqueue, node_id), node_value(pqueue, max_child)) < 0) {
		node_swap(pqueue, node_id, max_child);
		bubble_down(pqueue, max_child);
	}
}

// Αρχικοποιεί το σωρό από τα στοιχεία του vector values.

static void naive_heapify(PriorityQueue pqueue, Vector values) {
	// Απλά κάνουμε insert τα στοιχεία ένα ένα.
	// TODO: υπάρχει πιο αποδοτικός τρόπος να γίνει αυτό!
	for(int i=0;i<vector_size(values); i++){
		PriorityQueueNode pq_node = malloc(sizeof(*pq_node));
		pq_node->index = i+1;
		vector_insert_last(pqueue->values,vector_get_at(values,i));
		vector_insert_last(pqueue->nodes,pq_node);
	}
	if(values==NULL) return;
	int node=vector_size(values)/2;
	while(node!=0){
		bubble_down(pqueue,node);
		node--;
	}
}


// Συναρτήσεις του ADTPriorityQueue //////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	pqueue->compare = compare;
	pqueue->destroy_value = destroy_value;

	// Δημιουργία του vector που αποθηκεύει τα στοιχεία.
	// ΠΡΟΣΟΧΗ: ΔΕΝ περνάμε την destroy_value στο vector!
	// Αν την περάσουμε θα καλείται όταν κάνουμε swap 2 στοιχεία, το οποίο δεν το επιθυμούμε.
	pqueue->values = vector_create(0, NULL);
	pqueue->nodes = vector_create(0, free);

	// Αν values != NULL, αρχικοποιούμε το σωρό.
	if (values != NULL)
		naive_heapify(pqueue, values);

	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	return vector_size(pqueue->values);
}

Pointer pqueue_max(PriorityQueue pqueue) {
	return node_value(pqueue, 1);		// root
}

PriorityQueueNode pqueue_max_node(PriorityQueue pqueue){
	return vector_get_at(pqueue->nodes,0);
}

PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value){
	// Προσθέτουμε την τιμή στο τέλος το σωρού
	vector_insert_last(pqueue->values, value);

	// Δημιουργούμε τον κόμβο που έχει τη θέση του στο vector
	PriorityQueueNode node = malloc(sizeof(*node));
	node->index = vector_size(pqueue->values);
	vector_insert_last(pqueue->nodes,node);

 	// Ολοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού εκτός από τον τελευταίο, που μπορεί να είναι
	// μεγαλύτερος από τον πατέρα του. Αρα μπορούμε να επαναφέρουμε την ιδιότητα του σωρού καλώντας
	// τη bubble_up γα τον τελευταίο κόμβο (του οποίου το 1-based id ισούται με το νέο μέγεθος του σωρού).
	bubble_up(pqueue, pqueue_size(pqueue));
	return node;
}

Pointer pqueue_node_value(PriorityQueue pqueue, PriorityQueueNode node){
	// Επιστρέφει την τιμή του κόμβου node
	// node->index είναι 1-based, το vector ξεκινάει από το 0
	return vector_get_at(pqueue->values,node->index-1);
}

void pqueue_remove(PriorityQueue pqueue,PriorityQueueNode node) {
	int pq_node=node->index;
	int last_node = pqueue_size(pqueue);
	assert(pqueue_size(pqueue) != 0);		// LCOV_EXCL_LINE

	// Destroy την τιμή που αφαιρείται
	if (pqueue->destroy_value != NULL)
		pqueue->destroy_value(pqueue_node_value(pqueue,node));
	
	// Αντικαθιστούμε τον pq_node με τον τελευταίο και αφαιρούμε τον τελευταίο
	node_swap(pqueue, pq_node ,last_node);
	vector_remove_last(pqueue->values);
	vector_remove_last(pqueue->nodes);

	// Ολοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού εκτός από τον pq_node
 	// που μπορεί να είναι μικρότερος από κάποιο παιδί του. Αρα μπορούμε να
 	// επαναφέρουμε την ιδιότητα του σωρού καλώντας τη bubble_down για τον pq_node.
	bubble_down(pqueue, pq_node);
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	DestroyFunc old = pqueue->destroy_value;
	pqueue->destroy_value = destroy_value;
	return old;
}

void pqueue_destroy(PriorityQueue pqueue) {
	// Αντί να κάνουμε εμείς destroy τα στοιχεία, είναι απλούστερο να
	// προσθέσουμε τη destroy_value στο vector ώστε να κληθεί κατά το vector_destroy.
	vector_set_destroy_value(pqueue->values, pqueue->destroy_value);
	vector_destroy(pqueue->nodes);
	vector_destroy(pqueue->values);

	free(pqueue);
}