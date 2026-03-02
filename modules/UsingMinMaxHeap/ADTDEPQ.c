
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "ADTVector.h"
#include "ADTDEPQ.h"

struct depq{
    Vector values;
    CompareFunc compare;
    DestroyFunc destroy_value;
};

static Pointer node_value(DEPQ depq, int node_id) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	return vector_get_at(depq->values, node_id - 1);
}

static void push_down(DEPQ depq, int node_id);

// Ανταλλάσει τις τιμές των κόμβων node_id1 και node_id2

static void node_swap(DEPQ depq, int node_id1, int node_id2) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	Pointer value1 = node_value(depq, node_id1);
	Pointer value2 = node_value(depq, node_id2);

	vector_set_at(depq->values, node_id1 - 1, value2);
	vector_set_at(depq->values, node_id2 - 1, value1);
}

// Επιστρέφει το ελάχιστο από τα παιδιά και τα εγγόνια του κόμβου node_id
int smallest_Child_grdChild(struct depq* depq, int node_id) {
    int size = vector_size(depq->values);
    int smallest = -1;

    // Λόγο της ιδιότητας του min-max heap χρειάζεται να ελέγξουμε μόνο τα παιδιά και 4 εγγόνια
    int candidates[6];
    int count = 0;

    // Παιδιά
    int c1 = 2 * node_id;
    int c2 = 2 * node_id + 1;

    // Ελέγχουμε αν τα παιδιά είναι εντός ορίων
    // και τα προσθέτουμε στη λίστα υποψηφίων
    if(c1 <= size) candidates[count++] = c1;
    if(c2 <= size) candidates[count++] = c2;

    // Ελέγχουμε τα 4 εγγόνια
    // και τα προσθέτουμε στη λίστα υποψηφίων
    for(int i = 0; i < 4; ++i){
        int gc = 4 * node_id + i;
        if(gc <= size) candidates[count++] = gc;
    }

    // Ελέγχουμε ποιο είναι το μικρότερο από τους υποψηφίους
    // και το επιστρέφουμε
    for(int i = 0; i < count; ++i){
        if(smallest == -1){
            smallest = candidates[i];
        }else{
            if(depq->compare(node_value(depq, candidates[i]), node_value(depq, smallest)) < 0){
                smallest = candidates[i];
            }
        }
    }

    return smallest;
}

// Επιστρέφει το μέγιστο από τα παιδιά και τα εγγόνια του κόμβου node_id
int largest_Child_grdChild(DEPQ depq, int node_id){
    int size = vector_size(depq->values);
    int largest = -1;

    // Λόγο της ιδιότητας του min-max heap χρειάζεται να ελέγξουμε μόνο τα παιδιά και 4 εγγόνια
    int candidates[6];
    int count = 0;

    // Παιδιά
    int c1 = 2 * node_id;
    int c2 = 2 * node_id + 1;

    // Ελέγχουμε αν τα παιδιά είναι εντός ορίων
    if(c1 <= size) candidates[count++] = c1;
    if(c2 <= size) candidates[count++] = c2;

    // Ελέγχουμε τα 4 εγγόνια
    // και τα προσθέτουμε στη λίστα υποψηφίων
    for(int i = 0; i < 4; ++i){
        int gc = 4 * node_id + i;
        if(gc <= size) candidates[count++] = gc;
    }

    // Ελέγχουμε ποιο είναι το μεγαλύτερο από τους υποψηφίους
    // και το επιστρέφουμε
    for(int i = 0; i < count; ++i){
        if(largest == -1) {
            largest = candidates[i];
        }else{
            if(depq->compare(node_value(depq, candidates[i]), node_value(depq, largest)) > 0){
                largest = candidates[i];
            }
        }
    }

    return largest;
}

// Ανεβάζει κόμβο προς τα πάνω στο max επίπεδο αν είναι μεγαλύτερος από τον παππού του
static void push_up_max(DEPQ depq, int node_id){
    int grandparent = node_id / 4;
    // Αν ο κόμβος είναι μεγαλύτερος από τον παππού του, τους ανταλλάσσει
    if(grandparent > 0 && depq->compare(node_value(depq, node_id), node_value(depq, grandparent)) > 0){
        node_swap(depq, node_id, grandparent);
        push_up_max(depq, grandparent);  // Συνέχεια προς τα πάνω
    }
}
// Ανεβάζει κόμβο προς τα πάνω στο min επίπεδο αν είναι μικρότερος από τον παππού του
static void push_up_min(DEPQ depq, int node_id){
    int grandparent = node_id/4;
    // Αν ο κόμβος είναι μικρότερος από τον παππού του, τους ανταλλάσσει
    if( grandparent > 0 && depq->compare(node_value(depq,node_id),node_value(depq,grandparent)) < 0){
        node_swap(depq,node_id,grandparent);
        push_up_min(depq,grandparent); // Συνέχεια προς τα πάνω
    }
}

// Ανεβάζει κόμβο ανάλογα με το επίπεδο (min ή max)
static void push_up(DEPQ depq, int node_id){
    int parent = node_id/2;
    if(node_id != 1){
        // Αν το επίπεδο είναι min-level
        if( (int)log2(node_id) % 2 == 0){
            // Αν ο κόμβος είναι μεγαλύτερος από τον πατέρα του
            // τότε τους ανταλλάσσει και συνεχίζει προς τα πάνω σε max-level
            if( depq->compare( node_value(depq,node_id) , node_value(depq,parent )) > 0 ){
                node_swap(depq,node_id,parent);
                push_up_max(depq,parent);
            }else{
                // Αν ο κόμβος είναι μικρότερος από τον πατέρα του
                // τότε συνεχίζει προς τα πάνω σε min-level
                push_up_min(depq,node_id);
            }
        // Αν το επίπεδο είναι max-level
        }else{
            // Αν ο κόμβος είναι μικρότερος από τον πατέρα του
            // τότε τους ανταλλάσσει και συνεχίζει προς τα πάνω σε min-level
            if( depq->compare( node_value(depq,node_id) , node_value(depq,parent )) < 0 ){
                node_swap(depq,node_id,parent);
                push_up_min(depq,parent);
            }else{
                // Αν ο κόμβος είναι μεγαλύτερος από τον πατέρα του
                // τότε συνεχίζει προς τα πάνω σε max-level
                push_up_max(depq,node_id);
            }
        }
    }
}

// Κατεβάζει κόμβο στο max επίπεδο αν παραβιάζει την ιδιότητα του max-level
static void push_down_max(DEPQ depq, int node_id){
    int left_child = node_id * 2;

    // Αν έχει παιδιά (άρα και πιθανούς εγγονούς)
    if(left_child <= vector_size(depq->values)){
        // Βρίσκει το μεγαλύτερο παιδί ή εγγόνι
        int m = largest_Child_grdChild(depq, node_id);

        // Αν m είναι εγγόνι (θέσεις > right_child)
        if(m > node_id * 2 + 1){
            // Αν εγγόνι > κόμβος τότε ανταλλαγή και πιθανή διόρθωση με γονιό
            if(depq->compare(node_value(depq, m), node_value(depq, node_id)) > 0){
                node_swap(depq, m, node_id);
                if(depq->compare(node_value(depq, m), node_value(depq, m / 2)) < 0){
                    node_swap(depq, m, m / 2);
                }
                push_down(depq, m);  // Συνέχιση προς τα κάτω
            }
        }else if(depq->compare(node_value(depq, m), node_value(depq, node_id)) > 0){
            node_swap(depq, m, node_id);
        }
    }
}

// Κατεβάζει κόμβο στο min επίπεδο αν παραβιάζει την ιδιότητα του min-level
static void push_down_min(DEPQ depq, int node_id){
    int left_child = node_id*2;
    // Αν έχει παιδιά (άρα και πιθανούς εγγονούς)
    if(left_child <= vector_size(depq->values)){
        int m = smallest_Child_grdChild(depq,node_id);
        // Αν m είναι εγγόνι (θέσεις > right_child)
        if( m > node_id*2+1 ){
            // Αν εγγόνι < κόμβος τότε ανταλλαγή και πιθανή διόρθωση με γονιό
            if( depq->compare(node_value(depq,m),node_value(depq,node_id)) < 0 ){
                node_swap(depq,m,node_id);
                if( depq->compare(node_value(depq,m),node_value(depq,m/2)) > 0 ){
                    node_swap(depq,m,m/2);
                }
                push_down(depq,m);
            }
        }else if( depq->compare(node_value(depq,m),node_value(depq,node_id)) < 0 ){
            node_swap(depq,m,node_id);
        }
    }
}

// Κατεβάζει κόμβο ανάλογα με το επίπεδο (min ή max)
static void push_down(DEPQ depq, int node_id){
    if( (int)log2(node_id) % 2 == 0){
        // Αν το επίπεδο είναι min-level
        push_down_min(depq,node_id);
    }else{
        // Αν το επίπεδο είναι max-level
        push_down_max(depq,node_id);
    }
}


DEPQ depq_create(CompareFunc compare, DestroyFunc destroy_value){
	assert(compare != NULL);

    DEPQ depq = malloc(sizeof(*depq));

    depq->compare = compare;
    depq->destroy_value = destroy_value;

    depq->values = vector_create(0,NULL);

	return depq;
}

// Επιστρέφει το πλήθος στοιχείων του DEPQ
int depq_size(DEPQ depq){
    return vector_size(depq->values);
}

// Επιστρέφει το μέγιστο στοιχείο της DEPQ
Pointer depq_max(DEPQ depq){
    // Λόγω της ιδιότητας του min-max heap
    // το μέγιστο είναι το μεγαλύτερο από τα 2 παιδιά
    // του πρώτου κόμβου (ρίζα)
    switch (vector_size(depq->values))
    {
    case 0: return NULL;
        break;
    case 1: return node_value(depq,1);
        break;
    case 2: return node_value(depq,2);
        break;
    default:
        Pointer a = node_value(depq,2);
        Pointer b = node_value(depq,3);
        if( depq->compare(a,b) > 0 ) return a;
        else return b;
        break;
    }

}

// Επιστρέφει το ελάχιστο στοιχείο της DEPQ
Pointer depq_min(DEPQ depq){
    // Λόγω της ιδιότητας του min-max heap
    // το ελάχιστο είναι ο πρώτος κόμβος (ρίζα)
    return node_value(depq,1);
}

// Εισάγει την τιμή value στην DEPQ
void depq_insert(DEPQ depq, Pointer value){

    // Προσθέτει την τιμή στο τέλος του vector
    vector_insert_last(depq->values,value);

    // Καλέι την push_up για να επαναφέρει την ιδιότητα του min-max heap
    push_up(depq,vector_size(depq->values));
}

// Αφαιρεί την μεγαλύτερη τιμή της DEPQ
void depq_remove_max(DEPQ depq){
	int last_node = depq_size(depq);
	assert(last_node != 0);

    int max;

    switch (last_node)
    {
    case 1: max = 1;
        break;
    case 2: max =2;
        break;
    default:
        if( depq->compare(node_value(depq,2),node_value(depq,3)) > 0 ) max = 2;
        else max = 3;
        break;
    }

	// Destroy την τιμή που αφαιρείται
	if (depq->destroy_value != NULL)
		depq->destroy_value(depq_max(depq));

	// Αντικαθιστούμε τον max κόμβο με τον τελευταίο και αφαιρούμε τον τελευταίο
	node_swap(depq, max, last_node);
	vector_remove_last(depq->values);

 	// Ολοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού εκτός από τον max
 	// που μπορεί να μην ικανοποιεί την ιδιότητα του min-max Heap. Αρα μπορούμε να
 	// επαναφέρουμε την ιδιότητα του σωρού καλώντας τη push_down για το max.
	push_down(depq, max);
}

// Αφαιρεί την μικρότερη τιμή της DEPQ
void depq_remove_min(DEPQ depq){
	int last_node = depq_size(depq);
	assert(last_node != 0);

    int min = 1;

	// Destroy την τιμή που αφαιρείται
	if (depq->destroy_value != NULL)
		depq->destroy_value(depq_min(depq));

	// Αντικαθιστούμε τον min κόμβο με τον τελευταίο και αφαιρούμε τον τελευταίο
	node_swap(depq, min, last_node);
	vector_remove_last(depq->values);

 	// Ολοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού εκτός από το min
 	// που μπορεί να μην ικανοποιεί την ιδιότητα του min-max Heap. Αρα μπορούμε να
 	// επαναφέρουμε την ιδιότητα του σωρού καλώντας τη push_down για το min.
	push_down(depq, min);
}

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.
DestroyFunc depq_set_destroy_value(DEPQ depq, DestroyFunc destroy_value){
	DestroyFunc old = depq->destroy_value;
	depq->destroy_value = destroy_value;
	return old;
}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η DEPQ
void depq_destroy(DEPQ depq){
    vector_set_destroy_value(depq->values,depq->destroy_value);
    vector_destroy(depq->values);
    free(depq);
}
