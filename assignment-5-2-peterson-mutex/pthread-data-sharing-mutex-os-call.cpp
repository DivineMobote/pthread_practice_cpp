/* 

   Name        : Divine Mobote
   Date        : 02/13/2025
   Course      : ITSC 3146-101
   Professor   : Professor Mejias
   Assignment  : Divine Mobte: Assignment 5-2: Working with Pthreads - Peterson's Solution and Mutex
   Description : Pthread mutex implmentation using OS calls.

   */



#include <iostream>
#include <pthread.h>
#include <stdlib.h>

#define TOTAL_THREADS 4

int count;                     // Shared counter 
pthread_mutex_t the_mutex;    // Mutex for mutual exclusion


// Function executed by each thread
void* myFunction(void* arg)
{
    int actual_arg = *((int*) arg);

    for (unsigned int i = 0; i < 10; ++i) {

        // Locks the mutex before entering the critical section
        pthread_mutex_lock(&the_mutex);

        // Critical section to ensure safe increments 
        count++;
        std::cout << "Thread #" << actual_arg << " count = " << count << std::endl;

        // Unlock the mutex after exiting the critical section
        pthread_mutex_unlock(&the_mutex);

        // Random wait 
        int max = rand() % 100000;
        for (int x = 0; x < max; x++);
    }

    
    // Exit the thread
    pthread_exit(NULL);
}


// Main function
int main()
{
    int rc[TOTAL_THREADS];
    pthread_t ids[TOTAL_THREADS];
    int args[TOTAL_THREADS];

    
    // Initialize the mutex
    pthread_mutex_init(&the_mutex, NULL);

    count = 0;

    
    // Create the threads
    for (unsigned int i = 0; i < TOTAL_THREADS; ++i) {
        args[i] = i;
        rc[i] = pthread_create(&ids[i], NULL, myFunction, (void*) &args[i]);
    }

    
    // Wait for all threads to complete
    for (unsigned int i = 0; i < TOTAL_THREADS; ++i) {
        pthread_join(ids[i], NULL);
    }

    
    // Ends the mutex
    pthread_mutex_destroy(&the_mutex);

    
    // Print final count (this should 40 always)
    std::cout << "Final count = " << count << std::endl;
    pthread_exit(NULL);
}

