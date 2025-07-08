/* 

 Name        : Divine Mobote
 Date        : 02/17/2025
 Course      : ITSC 3146-101
 Professor   : Professor Mejias
 Assignment  :  Assignment 6-1: Working with Pthreads - Condition Variables in C++
 Description : Pthread condition variables with producer-consumer problem in C++.

 */



#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;

#define MAX 10  // Maximum number of orders
#define N 4     // The buffer size

// Structure to represent an order (item number and order number))
struct Order {
    int order_num;
    int item_num;
};


// Shared resources (buffer, unprocessed orders, order number)
Order new_orders[N];       
int num_new_orders = 0;    
int order_num = 0;         


// Mutexes for synchronization
pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;    
pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER; 


// Condition variables for producer-consumer synchronization (not full & not empty conditions)
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;   
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;  


// Producer function to take orders
void* takeOrders(void* arg) {
    int item;
    int index = 0;

    for (int i = 0; i < MAX; ++i) {
        // Lock console before taking user input
        pthread_mutex_lock(&console_mutex);
        cout << "Enter a menu item number between 1 and 50: ";
        cin >> item;
        pthread_mutex_unlock(&console_mutex);
        
      // Lock data_mutex
        pthread_mutex_lock(&data_mutex);
        // Wait if buffer is full
        while (num_new_orders == N) {
            pthread_cond_wait(&not_full, &data_mutex);
        }

        // Adds new order into buffer
        new_orders[index].order_num = order_num;
        new_orders[index++].item_num = item;
        ++num_new_orders;

        // Lock console for output and displays order details
        pthread_mutex_lock(&console_mutex);
        cout << "Got new order! Order number is " << order_num << " and item number: " << item << endl;
        // Unlock console
        pthread_mutex_unlock(&console_mutex);

        // Update order number and wrap buffer if needed
        ++order_num;
        if (index == N) index = 0;

        // Signal consumer that buffer is not empty
        pthread_cond_signal(&not_empty);
        // Unlock data_mutex
        pthread_mutex_unlock(&data_mutex);
    }
    // Exit thread
    pthread_exit(NULL);
}


// Consumer function for processing orders
void* processOrders(void* arg) {
    int item;
    int index = 0;
    int o_num;

    for (int i = 0; i < MAX; ++i) {
        // Lock data_mutex
        pthread_mutex_lock(&data_mutex);

        // Wait if buffer is empty
        while (num_new_orders == 0) {
            pthread_cond_wait(&not_empty, &data_mutex);
        }

        // Retrieve and removes order from buffer
        o_num = new_orders[index].order_num;
        item = new_orders[index++].item_num;
        --num_new_orders;

        // Signals the producer that buffer has space
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&data_mutex);

        // Lock console for output and display order processing
        pthread_mutex_lock(&console_mutex);
        cout << "Processing order number " << o_num << " with item number: " << item << endl;
        pthread_mutex_unlock(&console_mutex);

        // Processing time
        sleep(1);

        // Wrap buffer index if needed
        if (index == N) index = 0;
    }
    // Exit thread
    pthread_exit(NULL);
}


// Main function
int main() {
    // Create threads (producer and consumer)
    pthread_t id1, id2;
    pthread_create(&id2, NULL, takeOrders, NULL);
    pthread_create(&id1, NULL, processOrders, NULL);
    

    // Wait for both threads to complete
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    // Print goodbye message after both threads finish
    cout << "Phew! Done with orders for today!" << endl;
    // Exit 
    pthread_exit(NULL);
}
