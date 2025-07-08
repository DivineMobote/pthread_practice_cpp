/* 

   Name        : Divine Mobote
   Date        : 02/13/2025
   Course      : ITSC 3146-101
   Professor   : Professor Mejias
   Assignment  : Divine Mobte: Assignment 5-2: Working with Pthreads - Peterson's Solution and Mutex
   Description : Peterson's Solution 

   */



#include <iostream>
#include <pthread.h>
#include <stdlib.h>

#define TOTAL_THREADS 2


int count;                         // Shared counter for both threads
int turn;                          // Control turns for both threads              


bool interested[TOTAL_THREADS];   // Flags for the threads entrance


// The function makes sure mutual exclusion by only allowing one thread to enter the critical region at a time
void enter_region(int thread_id)
{
   int other = 1 - thread_id;        // ID the other thread  

   interested[thread_id] = true;     // Set the flag for the thread to enter
    
   turn = other;                     // Set the turn to the other thread

    // Wait until it is the other thread's turn to enter 
   while (interested[other] && turn == other);
}


// This function makes sure only one thread leaves the critical section by marking the flag as false and lets the other one thread enter at a time
void leave_region(int thread_id)
{
  
   interested[thread_id] = false;
}


// This function is what the thread actually does
void* myFunction(void* arg)
{
    int thread_id = *((int*) arg);

    for(unsigned int i = 0; i < 10; ++i) {

        // wait for its turn
        enter_region(thread_id);

        // Beginning of the critical region
        count++;
        std::cout << "Thread #" << thread_id 
                  << " count = " << count << std::endl;
        // End of the critical region

        // Let other thread have a turn
        leave_region(thread_id);

        // Random wait
        int max = rand() % 1000000;
        for (int x = 0; x < max; x++);
    }
    
    // Exit the thread
     pthread_exit(NULL);
}



// Main function 
int main()
{
    // Thread information (return code, thread id, and thread argument)
    int rc[TOTAL_THREADS];
    pthread_t ids[TOTAL_THREADS];
    int args[TOTAL_THREADS];

    
    // Initialize the shared counter
    count = 0; 

    
    // Create the threads
    for(unsigned int i = 0; i < TOTAL_THREADS; ++i) {
        args[i] = i;
        rc[i] = pthread_create(&ids[i], NULL, myFunction, (void*) &args[i]);
    }

    
    // Wait for all threads to finish
    for(unsigned int i = 0; i < TOTAL_THREADS; ++i) {
        pthread_join(ids[i], NULL);
    }

    
    // Print final count 
    std::cout << "Final count = " << count << std::endl;
    pthread_exit(NULL);
}
