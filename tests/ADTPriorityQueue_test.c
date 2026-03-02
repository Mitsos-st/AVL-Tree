//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Priority Queue.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTPriorityQueue.h"

// θέτει τα στοιχεία του πίνακα array  σε τυχαία σειρά
void shuffle(int* array[], int size) {
	for (int i = 0; i < size; i++) {
		int new_pos = i + rand() / (RAND_MAX / (size - i) + 1);
		int* temp = array[new_pos];
		array[new_pos] = array[i];
		array[i] = temp;
	}
}

// Επιστρέφει έναν ακέραιο σε νέα μνήμη με τιμή value
int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void test_create(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, NULL, NULL);
	pqueue_set_destroy_value(pqueue, NULL);

	TEST_ASSERT(pqueue != NULL);
	TEST_ASSERT(pqueue_size(pqueue) == 0);

	pqueue_destroy(pqueue);

	// create με αρχικά στοιχεία
	Vector values = vector_create(0, NULL);			// χωρίς destroy function, το destroy θα το κάνει η ουρά!
	vector_insert_last(values, create_int(0));
	vector_insert_last(values, create_int(1));
	vector_insert_last(values, create_int(2));
	vector_insert_last(values, create_int(3));

	pqueue = pqueue_create(compare_ints, free, values);
	TEST_ASSERT(pqueue != NULL);
	TEST_ASSERT(pqueue_size(pqueue) == 4);

	TEST_ASSERT(*(int*)pqueue_max(pqueue) == 3);
	pqueue_remove(pqueue, pqueue_max_node(pqueue));
	TEST_ASSERT(*(int*)pqueue_max(pqueue) == 2);

	vector_destroy(values);
	pqueue_destroy(pqueue);
}	

void test_insert(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, NULL, NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));					// Στο pqueue θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα
	PriorityQueueNode* nodes = malloc(N * sizeof(PriorityQueueNode));// Τα nodes που εισάγαμε

	// insert N στοιχεία
	for (int i = 0; i < N; i++) {
		array[i] = i;
		nodes[i] = pqueue_insert(pqueue, &array[i]);
		TEST_ASSERT(pqueue_size(pqueue) == i+1);			// Το size ενημερώθηκε;
		TEST_ASSERT(pqueue_max(pqueue) == &array[i]);	// Εισαγωγή σε αύξουσα σειρά, το στοιχείο που μόλις βάλαμε πρέπει να είναι στην κορυφή
		TEST_ASSERT(pqueue_node_value(pqueue, nodes[i]) == &array[i]);	// Ο κόμβος που προσθέσαμε περιέχει το σωστό στοιχείο
	}

	// Έλεγχος ότι τα nodes παραμένουν σωστά μετά από όλα τα inserts
	for (int i = 0; i < N; i++) {
		TEST_ASSERT(pqueue_node_value(pqueue, nodes[i]) == &array[i]);	// Ο κόμβος που προσθέσαμε περιέχει το σωστό στοιχείο
	}

	pqueue_destroy(pqueue);
	free(array);
	free(nodes);
}

// Απλουστευμένη δοκιμή διαγραφής (για εύκολο debugging).
void test_simple_remove(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, free, NULL);

	// προσθήκη τριών στοιχείων
	PriorityQueueNode node1 = pqueue_insert(pqueue, create_int(1));
	PriorityQueueNode node2 = pqueue_insert(pqueue, create_int(2));
	PriorityQueueNode node3 = pqueue_insert(pqueue, create_int(3));

	// αφαίρεση
	pqueue_remove(pqueue, node1);					// αφαίρεση του 1
	TEST_ASSERT(*(int*)pqueue_max(pqueue) == 3);	// δεν άλλαξε το μέγιστο
	TEST_ASSERT(pqueue_max_node(pqueue) == node3);
	TEST_ASSERT(pqueue_size(pqueue) == 2);

	pqueue_remove(pqueue, node3);					// αφαίρεση του 3
	TEST_ASSERT(*(int*)pqueue_max(pqueue) == 2);	// άλλαξε το μέγιστο
	TEST_ASSERT(pqueue_max_node(pqueue) == node2);
	TEST_ASSERT(pqueue_size(pqueue) == 1);

	pqueue_remove(pqueue, node2);
	TEST_ASSERT(pqueue_size(pqueue) == 0);

	// ελευθέρωση μνήμης
	pqueue_destroy(pqueue);
}

// Έλεγχος διαγραφής μέγιστου στοιχείου
void test_remove_max(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, free, NULL);

	// προσθήκη δεδομένων, τυχαία σειρά
	int N = 10;
	int** array = malloc(N * sizeof(*array));
	for (int i = 0; i < N; i++)
		array[i] = create_int(i);
	shuffle(array, N);

	for (int i = 0; i < N; i++)
		pqueue_insert(pqueue, array[i]);

	// Διαδοχικά remove ώστε να συμβούν και resizes
	for (int i = N-1; i >= 0; i--) {
		int* value = pqueue_max(pqueue);
		TEST_ASSERT(*value == i);
		pqueue_remove(pqueue, pqueue_max_node(pqueue));
		TEST_ASSERT(pqueue_size(pqueue) == i);
	}

	pqueue_destroy(pqueue);

	// remove από ουρά χωρίς συνάρτηση destroy
	pqueue = pqueue_create(compare_ints, NULL, NULL);
	pqueue_insert(pqueue, &N);
	TEST_ASSERT(pqueue_max(pqueue) == &N);
	pqueue_remove(pqueue, pqueue_max_node(pqueue));
	pqueue_destroy(pqueue);
	free(array);
}

// Έλεγχος διαγραφής οποιουδήποτε στοιχείου
void test_remove(void) {
	PriorityQueue pqueue = pqueue_create(compare_ints, free, NULL);

	// προσθήκη δεδομένων, τυχαία σειρά
	int N = 10;
	int** array = malloc(N * sizeof(*array));
	PriorityQueueNode* nodes = malloc(N * sizeof(*nodes));
	for (int i = 0; i < N; i++)
		array[i] = create_int(i);
	shuffle(array, N);

	for (int i = 0; i < N; i++)
		nodes[i] = pqueue_insert(pqueue, array[i]);

	// Διαδοχικά remove
	for (int i = 0; i < N; i++) {
		TEST_ASSERT(pqueue_node_value(pqueue, nodes[i]) == array[i]);

		int old_max = *(int*)pqueue_max(pqueue);	// αποθήκευση πριν
		bool removing_max = old_max == *array[i];	// γίνει free!

		pqueue_remove(pqueue, nodes[i]);
		TEST_ASSERT(pqueue_size(pqueue) == N-i-1);

		if(pqueue_size(pqueue) > 0) {
			int new_max = *(int*)pqueue_max(pqueue);
			if(removing_max)
				// Αν αφαιρέσαμε το μέγιστο, το νέο μέγιστο πρέπει να είναι μικρότερο
				TEST_ASSERT(new_max < old_max);
			else
				// Αν όχι, το νέο μέγιστο δεν έχει αλλάξει
				TEST_ASSERT(new_max == old_max);
		}
	}

	pqueue_destroy(pqueue);

	free(array);
	free(nodes);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "pqueue_create", test_create },
	{ "pqueue_insert", test_insert },
	{ "pqueue_simple_remove", test_simple_remove },
	{ "pqueue_remove_max", test_remove_max },
	{ "pqueue_remove", test_remove },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
