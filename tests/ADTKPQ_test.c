//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Priority Queue.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTKPQ.h"

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
	KPQ kpq = kpq_create(compare_ints, 3 , NULL);
	kpq_set_destroy_value(kpq, free);

	TEST_ASSERT(kpq != NULL);
	TEST_ASSERT(kpq_size(kpq) == 0);

	// create με αρχικά στοιχεία

    kpq_insert(kpq,create_int(1));
    kpq_insert(kpq,create_int(2));
    kpq_insert(kpq,create_int(3));
    kpq_insert(kpq,create_int(5));
    kpq_insert(kpq,create_int(4));

	TEST_ASSERT(kpq_size(kpq) == 5);

	TEST_ASSERT(*(int*)kpq_max(kpq) == 5);
    kpq_remove_max(kpq);
	TEST_ASSERT(*(int*)kpq_max(kpq) == 4);

	TEST_ASSERT(*(int*)kpq_k_th(kpq) == 2);

	kpq_destroy(kpq);
}

void test_k_th(void){
	KPQ kpq = kpq_create(compare_ints, 10 , free);
	int N=100;
	int *array = malloc(N*sizeof(int));

	for(int i=0;i<N;i++){
		array[i] = i;
		kpq_insert(kpq,create_int(array[i]));
		TEST_ASSERT( kpq_size(kpq) == i+1);
		if(i>9){
			TEST_ASSERT( *(int*)kpq_k_th(kpq) == i-9);
		}
	}
	free(array);
	kpq_destroy(kpq);
}

void test_insert(void) {
	KPQ kpq = kpq_create(compare_ints,3, free);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));

	// insert N στοιχεία
	for (int i = 0; i < N; i++) {
		array[i] = i;
		kpq_insert(kpq, create_int(array[i]));
		TEST_ASSERT(kpq_size(kpq) == i+1);			// Το size ενημερώθηκε;
		TEST_ASSERT( *(int*)kpq_max(kpq) == array[i]);	// Εισαγωγή σε αύξουσα σειρά, το στοιχείο που μόλις βάλαμε πρέπει να είναι στην κορυφή
	}

	kpq_destroy(kpq);
	free(array);
}

// Απλουστευμένη δοκιμή διαγραφής (για εύκολο debugging).
void test_remove(void) {
	KPQ kpq = kpq_create(compare_ints, 3 ,free);

	// προσθήκη δεδομένων, τυχαία σειρά
	int N = 10;
	int** array = malloc(N * sizeof(*array));
	for (int i = 0; i < N; i++)
		array[i] = create_int(i);

	for (int i = 0; i < N; i++)
		kpq_insert(kpq, array[i]);

	// Διαδοχικά remove ώστε να συμβούν και resizes
	for (int i = N-1; i >= 0; i--) {
		int* value = kpq_max(kpq);
		TEST_ASSERT(*value == i);
		kpq_remove_max(kpq);
		TEST_ASSERT(kpq_size(kpq) == i);
	}

	kpq_destroy(kpq);

	// remove από ουρά χωρίς συνάρτηση destroy
	kpq = kpq_create(compare_ints,3, NULL);
	kpq_insert(kpq, &N);
	TEST_ASSERT(kpq_max(kpq) == &N);
	kpq_remove_max(kpq);
	kpq_destroy(kpq);
	free(array);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "kpq_create", test_create },
	{ "kpq_insert", test_insert },
	{ "kpq_k_th", test_k_th },
	{ "kpq_remove", test_remove },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
