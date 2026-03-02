![run-tests](../../workflows/run-tests/badge.svg)

## 2025 Project 2

Εκφώνηση: https://k08.chatzi.org/projects/project2/


### Προσωπικά στοιχεία

Στο αρχείο `AUTHORS` προσθέστε μια γραμμή με την παρακάτω μορφή:

```
sdi2400999,my-github-username,Όνομα Επώνυμο
```

Η σωστή μορφή του αρχείου ελέγχεται αυτόματα όταν κάνετε push στο github, σε
περίπτωση λάθους θα πάρετε μήνυμα στο email σας και θα πρέπει να το διορθώσετε για
να γίνει δεκτή η εργασία.

### Ασκήσεις που παραδίδονται

Συμπληρώστε `[x]` στις ασκήσεις (και τυχόν bonus) που παραδίδετε.

- [x] Άσκηση 1
- [x] Άσκηση 2
- [x] Άσκηση 3
- [x] Άσκηση 4
- [x] Άσκηση 5
- [x] Bonus 1
- [x] Bonus 2

### Documentation

Συμπληρώστε εδώ __όσο documentation χρειάζεται__ ώστε οι βαθμολογητές να
κατανοήσουν πλήρως τις λύσεις σας και να τις βαθμολογήσουν ανάλογα. Αυτό θα
πρέπει να γίνει ανεξάρτητα με το αν ο κώδικάς σας είναι καλά σχολιασμένος,
πράγμα που συνιστάται.

### Άσκηση 1

#### pqueue_create
Έχει πολυπλοκότητα Ο(n) με n το πλήθος των στοιχείων του Vector values

#### pqueue_size
Έχει πολυπλοκότητα Ο(1) απλά καλεί τη set_size

#### pqueue_max
Έχει πολυπλοκότητα Ο(1) απλά επιστρέφει το value του pqueue->first

#### pqueue_max_node
Έχει πολυπλοκότητα Ο(1) απλά επιστρέφει το pqueue->first

#### pqueue_insert
Έχει πολυπλοκότητα όσο το set_insert

#### pqueue_remove
Έχει πολυπλοκότητα όσο το set_remove

#### pqueue_node_value
Έχει πολυπλοκότητα Ο(1) απλά επιστρέφει το node->value

#### pqueue_set_destroy_value
Έχει πολυπλοκότητα Ο(1)

#### pqueue_destroy
Έχει πολυπλοκότητα όσο η set_destroy

### Άσκηση 2

#### depq_create
Έχει πολυπλοκότητα O(1) απλά καλεί τη set_create και αρχηκοποιεί τις τιμές του depq

#### depq_size
Έχει πολυπλοκότητα O(1) απλά καλεί την set_size

#### depq_max
Έχει πολυπλοκότητα όσο η set_node_value

#### depq_min
Έχει πολυπλοκότητα όσο η set_node_value

#### depq_insert
Έχει πολυπλοκότητα όσο η set_insert

#### depq_remove_max
Καλεί τις συναρτήσεις set_remove και depq_max οπότε έχει πολυπλοκότητα max( set_remove , depq_max )

#### depq_remove_min
Καλεί τις συναρτήσεις set_remove και depq_min οπότε έχει πολυπλοκότητα max( set_remove , depq_min )

#### depq_set_destroy_value
Έχει πολυπλοκότητα Ο(1)

#### depq_destroy
Έχει πολυπλοκότητα όσο η set_destroy

### Άσκηση 3

#### kpq_create
Έχει πολυπλοκότητα όσο η max( depq_create , pqueue_create )

#### kpq_size
Έχει πολυπλοκότητα O(1) απλά καλεί τις depq_size και pqueue_size

#### kpq_max
Έχει πολυπλοκότητα όσο η depq_max

#### kpq_k_th
Έχει πολυπλοκότητα όσο η depq_min

#### kpq_insert
Καλεί depq_insert και pqueue_insert οπότε έχει πολυπλοκότητα max( depq_insert , pqueue_insert )

#### kpq_remove_max
Καλεί depq_remove και pqueue_remove οπότε έχει πολυπλοκότητα max( depq_remove , pqueue_remove )

#### kpq_set_destroy_value
Έχει πολυπλοκότητα O(1)

#### kpq_destroy
Καλεί depq_destroy και pqueue_destroy οπότε έχει πολυπλοκότητα max( depq_destroy , pqueue_destroy )

### Άσκηση 4

#### pqueue_create
Έχει πολυπλοκότητα O(n) με n το πλήθος των στοιχείων του vector values

#### pqueue_size
Έχει πολυπλοκότητα O(1) όσο η πολυπλοκότητα του vector_size

#### pqueue_max
Κάλει τη συνάρτηση node_value η οποία καλεί την vector_get_at οπότε έχει πολυλποκότητα όσο η vector_get_at

#### pqueue_max_node
Κάλει τη συνάρτηση vector_get_at οπότε έχει πολυλποκότητα όσο η vector_get_at

#### pqueue_insert
Κάλει bubble_up οπότε έχει πολυπλοκότητα O(logn) με n το πλήθος των στοιχείων

#### pqueue_node_value
Κάλει τη συνάρτηση vector_get_at οπότε έχει πολυλποκότητα όσο η vector_get_at

#### pqueue_remove
Κάλει bubble_down οπότε έχει πολυπλοκότητα O(logn) με n το πλήθος των στοιχείων

#### pqueue_set_destroy_value
Έχει πολυπλοκότητα O(1)

#### pqueue_destroy
Καλεί vector_destroy οπότε έχει πολυλποκότητα όσο η vector_destroy

### Άσκηση 5

#### depq_create
Έχει πολυπλοκότητα O(1)

#### depq_size
Έχει πολυπλοκότητα O(1)

#### depq_max
Καλεί την pqueue_max οπότε έχει πολυπλοκότητα όσο η pqueue_max

#### depq_min
Καλεί την pqueue_max οπότε έχει πολυπλοκότητα όσο η pqueue_max

#### depq_insert
Καλεί την pqueue_insert οπότε έχει πολυπλοκότητα όσο η pqueue_insert

#### depq_remove_max
Καλεί την pqueue_remove και pqueue_max οπότε έχει πολυπλοκότητα max( pqueue_remove , pqueue_max )

#### depq_remove_min
Καλεί την pqueue_remove και pqueue_max οπότε έχει πολυπλοκότητα max( pqueue_remove , pqueue_max )

#### depq_set_destroy_value
Έχει πολυπλοκότητα O(1)

#### depq_destroy
Καλεί την pqueue_destroy οπότε έχει πολυπλοκότητα όσο η pqueue_destroy

### Bonus 1
Υλοποίηση ADTKPQ μέσω ADTSet 
kpq_max , kpq_k_th πολυπλοκότητα O(1)

### Bonus 2

#### depq_create
Έχει πολυπλοκότητα O(1) 

#### depq_size
Έχει πολυπλοκότητα O(1) 

#### depq_max
Έχει πολυπλοκότητα O(1) απλά καλεί vector_node_value

#### depq_min
Έχει πολυπλοκότητα O(1) απλά καλεί vector_node_value

#### depq_insert
Καλέι την push_up συνάρτηση η οποία έχει πολυπλοκότητα O(logn) οπότε και η depq_insert έχει O(logn)

#### depq_remove_max
Καλέι την push_down συνάρτηση η οποία έχει πολυπλοκότητα O(logn) οπότε και η depq_remove_max έχει O(logn)

#### depq_remove_min
Καλέι την push_down συνάρτηση η οποία έχει πολυπλοκότητα O(logn) οπότε και η depq_remove_min έχει O(logn)

#### depq_set_destroy_value
Έχει πολυπλοκότητα O(1) 

#### depq_destroy
Καλεί την vector_destroy οπότε έχει πολυπλοκότητα όσο η vector_destroy

### Min-Max Heap

Οι συναρτήσεις φτιάχτηκαν με βάση τον ψευδοκώδικα που είχε το [link](https://en.wikipedia.org/wiki/Min-max_heap)
 της εργασίας για τον min-max heap
