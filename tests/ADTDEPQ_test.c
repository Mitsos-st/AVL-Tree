//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Priority Queue.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTDEPQ.h"

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
	DEPQ depq = depq_create(compare_ints, NULL);
	depq_set_destroy_value(depq, free);

	TEST_ASSERT(depq != NULL);
	TEST_ASSERT(depq_size(depq) == 0);

	// create με αρχικά στοιχεία

    depq_insert(depq,create_int(1));
    depq_insert(depq,create_int(2));
    depq_insert(depq,create_int(3));
    depq_insert(depq,create_int(30));
    depq_insert(depq,create_int(4));

	TEST_ASSERT(depq != NULL);
	TEST_ASSERT(depq_size(depq) == 5);

	TEST_ASSERT(*(int*)depq_max(depq) == 30);
    depq_remove_max(depq);
	TEST_ASSERT(*(int*)depq_max(depq) == 4);

	depq_destroy(depq);
}

void test_insert(void) {
	DEPQ depq = depq_create(compare_ints, free);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));					// Στο depq θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert N στοιχεία
	for (int i = 0; i < N; i++) {
		array[i] = i;
		depq_insert(depq, create_int(array[i]));
		TEST_ASSERT(depq_size(depq) == i+1);			// Το size ενημερώθηκε;
		TEST_ASSERT( *(int*)depq_max(depq) == array[i]);	// Εισαγωγή σε αύξουσα σειρά, το στοιχείο που μόλις βάλαμε πρέπει να είναι στην κορυφή
	}

	depq_destroy(depq);
	free(array);
}

// Απλουστευμένη δοκιμή διαγραφής (για εύκολο debugging).
void test_remove(void) {
	DEPQ depq = depq_create(compare_ints,free);

	depq_insert(depq,create_int(1));
	depq_insert(depq,create_int(2));
	depq_insert(depq,create_int(3));
	depq_insert(depq,create_int(4));

	TEST_ASSERT( depq_size(depq) == 4 );
	TEST_ASSERT( *(int*)depq_max(depq) == 4 );
	TEST_ASSERT( *(int*)depq_min(depq) == 1 );

	depq_remove_max(depq);

	TEST_ASSERT( depq_size(depq) == 3 );
	TEST_ASSERT( *(int*)depq_max(depq) == 3 );
	TEST_ASSERT( *(int*)depq_min(depq) == 1 );

	depq_remove_min(depq);

	TEST_ASSERT( depq_size(depq) == 2 );
	TEST_ASSERT( *(int*)depq_max(depq) == 3 );
	TEST_ASSERT( *(int*)depq_min(depq) == 2 );

	depq_destroy(depq);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "depq_create", test_create },
	{ "depq_insert", test_insert },
	{ "depq_remove", test_remove },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
