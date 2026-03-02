#include <stdlib.h>
#include <assert.h>

#include "ADTDEPQ.h"
#include "ADTPriorityQueue.h"

// Δομή για το DEPQ, περιλαμβάνει δύο ουρές προτεραιότητας, μία για το min και μία για το max
struct depq{
    PriorityQueue min;               // Ουρά προτεραιότητας για εύρεση του ελάχιστου
    PriorityQueue max;               // Ουρά προτεραιότητας για εύρεση του μέγιστου
    CompareFunc compare;      
    DestroyFunc destroy_value;
};

// Τυποποίηση μιας δομής που περιέχει τον κόμβο της ουράς, την τιμή, και δείκτη στο depq
typedef struct value{
    PriorityQueueNode node;          // Κόμβος που αντιστοιχεί στην άλλη ουρά
    Pointer value;                   // Η πραγματική τιμή του στοιχείου
    DEPQ depq;                       // Αναφορά στο DEPQ για χρήση στις συναρτήσεις
} *Value;

// Συνάρτηση σύγκρισης για την max ουρά (χρησιμοποιεί την πραγματική compare)
int compare_max(Pointer a, Pointer b){
    Value av = a;
    Value bv = b;
    return av->depq->compare(av->value, bv->value);
}

// Συνάρτηση σύγκρισης για την min ουρά (αρνητικό της compare)
int compare_min(Pointer a, Pointer b){
    Value av = a;
    Value bv = b;
    return -av->depq->compare(av->value, bv->value);
}

// Καταστροφή τιμής (καλεί την destroy_value αν υπάρχει και ελευθερώνει τη μνήμη της Value)
void destroy_f(Pointer a){
    Value av = a;
    if (av->depq->destroy_value != NULL)
        av->depq->destroy_value(av->value);
    free(av);
}

// Δημιουργία του DEPQ, αρχικοποιεί τις δύο ουρές και αποθηκεύει τις συναρτήσεις
DEPQ depq_create(CompareFunc compare, DestroyFunc destroy_value){
    assert(compare != NULL);

    DEPQ depq = malloc(sizeof(*depq));
    depq->compare = compare;
    depq->destroy_value = destroy_value;

    depq->min = pqueue_create(compare_min, destroy_f, NULL);
    depq->max = pqueue_create(compare_max, destroy_f, NULL);

    return depq;
}

// Επιστρέφει το πλήθος στοιχείων του DEPQ (ίδιο για min και max)
int depq_size(DEPQ depq){
    return pqueue_size(depq->max);
}

// Επιστρέφει την μέγιστη τιμή
Pointer depq_max(DEPQ depq){
    Value max = pqueue_max(depq->max);
    return max->value;
}

// Επιστρέφει την ελάχιστη τιμή
Pointer depq_min(DEPQ depq){
    Value min = pqueue_max(depq->min); // επειδή στην min ουρά το min είναι στο max
    return min->value;
}

// Εισάγει νέο στοιχείο στο DEPQ (σε δύο ουρές ταυτόχρονα)
void depq_insert(DEPQ depq, Pointer value){
    // Δημιουργία αντίγραφων του ίδιου value για min και max ουρά
    Value max = malloc(sizeof(*max));
    max->value = value;
    max->depq = depq;

    Value min = malloc(sizeof(*min));
    min->value = value;
    min->depq = depq;

    // Κάθε κόμβος κρατά pointer στον αντίστοιχο στην άλλη ουρά
    max->node = pqueue_insert(depq->min, min);
    min->node = pqueue_insert(depq->max, max);
}

// Αφαιρεί το μέγιστο στοιχείο
void depq_remove_max(DEPQ depq){
    Value max = pqueue_max(depq->max);

    // Προσωρινή αλλαγή της destroy συνάρτησης για να μην καταστραφεί η τιμή δύο φορές
    DestroyFunc old = pqueue_set_destroy_value(depq->min, free);

    pqueue_remove(depq->min, max->node);

    // Επαναφέρει την αρχική destroy συνάρτηση
    pqueue_set_destroy_value(depq->min, old);

    // Αφαιρεί το στοιχείο από την max ουρά
    pqueue_remove(depq->max, pqueue_max_node(depq->max));
}

// Αφαιρεί το ελάχιστο στοιχείο
void depq_remove_min(DEPQ depq){
    Value min = pqueue_max(depq->min);

    DestroyFunc old = pqueue_set_destroy_value(depq->max, free);

    // Αφαιρεί τον αντίστοιχο κόμβο από την max ουρά
    pqueue_remove(depq->max, min->node);

    pqueue_set_destroy_value(depq->max, old);

    // Αφαιρεί το στοιχείο από την min ουρά
    pqueue_remove(depq->min, pqueue_max_node(depq->min));
}

// Αλλάζει τη συνάρτηση καταστροφής τιμών
DestroyFunc depq_set_destroy_value(DEPQ depq, DestroyFunc destroy_value){
    DestroyFunc old = depq->destroy_value;
    depq->destroy_value = destroy_value;
    return old;
}

// Καταστροφή DEPQ και όλων των περιεχομένων του
void depq_destroy(DEPQ depq){
    // Κάθε ουρά περιέχει τον ίδιο pointer για την τιμή
    // Οπότε η destroy_value καλείται μόνο μία φορά και καλούμε απλά free για ελευθέρωση του struct Value
    pqueue_set_destroy_value(depq->max, free);
    pqueue_set_destroy_value(depq->min, destroy_f);

    // Καταστροφή των ουρών και απελευθέρωση της δομής
    pqueue_destroy(depq->max);
    pqueue_destroy(depq->min);
    free(depq);
}
