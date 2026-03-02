# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα tets για την υλοποίηση αυτή.


# Υλοποιήσεις μέσω ADT Set: ADTPriorityQueue
# Χρειαζόμαστε και υλοποιήσεις για τα ADTSet και ADTVector
#
#Άσκηση 1
UsingADTSet_ADTPriorityQueue_test_OBJS = ADTPriorityQueue_test.o $(MODULES)/UsingADTSet/ADTPriorityQueue.o $(MODULES)/UsingAVL/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o

#Άσκηση 2
UsingADTSet_ADTDEPQ_test_OBJS = ADTDEPQ_test.o $(MODULES)/UsingADTSet/ADTDEPQ.o $(MODULES)/UsingAVL/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o

#Άσκηση 3
UsingADTDEPQ_ADTKPQ_test_OBJS = ADTKPQ_test.o $(MODULES)/UsingADTDEPQ/ADTKPQ.o $(MODULES)/UsingADTSet/ADTDEPQ.o $(MODULES)/UsingADTSet/ADTPriorityQueue.o $(MODULES)/UsingAVL/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o

#Άσκηση 4
UsingHeap_ADTPriorityQueue_test_OBJS = ADTPriorityQueue_test.o $(MODULES)/UsingHeap/ADTPriorityQueue.o $(MODULES)/UsingAVL/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o

#Άσκηση 5
UsingHeap_ADTDEPQ_test_OBJS = ADTDEPQ_test.o $(MODULES)/UsingHeap/ADTPriorityQueue.o $(MODULES)/UsingADTPriorityQueue/ADTDEPQ.o $(MODULES)/UsingAVL/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o
UsingHeap_ADTKPQ_test_OBJS = ADTKPQ_test.o $(MODULES)/UsingHeap/ADTPriorityQueue.o $(MODULES)/UsingADTPriorityQueue/ADTDEPQ.o $(MODULES)/UsingADTDEPQ/ADTKPQ.o $(MODULES)/UsingAVL/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o

#Bonus 1
UsingADTSet_ADTKPQ_test_OBJS = ADTKPQ_test.o $(MODULES)/UsingAVL/ADTSet.o $(MODULES)/UsingDynamicArray/ADTVector.o $(MODULES)/UsingADTSet/ADTKPQ.o

#Bonus 2
UsingMinMaxHeap_ADTDEPQ_test_OBJS = ADTDEPQ_test.o $(MODULES)/UsingMinMaxHeap/ADTDEPQ.o $(MODULES)/UsingDynamicArray/ADTVector.o
UsingMinMaxHeap_ADTKPQ_test_OBJS = ADTKPQ_test.o $(MODULES)/UsingMinMaxHeap/ADTDEPQ.o $(MODULES)/UsingDynamicArray/ADTVector.o $(MODULES)/UsingADTDEPQ/ADTKPQ.o $(MODULES)/UsingHeap/ADTPriorityQueue.o

# Ο βασικός κορμός του Makefile
include ../common.mk