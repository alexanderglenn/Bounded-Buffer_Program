/*********************************************************************************************
 *
 * Alexander Glenn
 * Date created: 05/10/2016
 * Date since last modification: 14/10/2016
 *
 * File: buffer.c
 *
 * Contains the main function for the program.
 * The program requires 3 inputs from the user, the time for the main function
 * to sleep, the number of producer threads and the number of consumer threads
 * To execute the program: ./buffer [Sleep time for main] [No. of producer threads] [No. of consumer threads]
 * Example: ./buffer 10 5 5
 *
 **********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <time.h>
#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];	// The buffer
pthread_mutex_t mutex;			// Mutex lock
sem_t full, empty;			// Semaphores for full buffer and empty buffer
int counter;

/*
 * Description: Inserts an item on to the buffer
 * Input: any item of the buffer_item type
 * Output: return 0 if able to put item onto buffer or
 *        return -1 if the buffer is full
 */
int insert_item(buffer_item item){
	// When the buffer is not full add the item and increment the counter
	if (counter < BUFFER_SIZE) {
		buffer[counter] = item;
		counter++;
		return 0;
	}
	else { // Returns -1 if the buffer is full
		return -1;
	}
}

/*
 * Description: Removes the top item in the buffer
 * Input: a pointer to a item
 * Output: return 0 if able to remove item from buffer
 * 	   or -1 is returned if the buffer is empty
 */
int remove_item(buffer_item *item){
	// When the buffer is not empty remove the item and decrement the counter
	if (counter > 0) {
		*item = buffer[(counter - 1)];
		counter--;
		return 0;
	}
	else { // Returns -1 if the buffer is empty
		return -1;
	}
}

/*
 * Description: Creates a producer thread with a thread ID, the producer will sleep for a random time less than
 * 		eight seconds and then attempt to insert an item onto the buffer. The producer will wait if the
 * 		buffer is full. Mutex lock is acquired, the item inserted, then mutex lock is released and
 * 		the full semaphore is signalled to show that the buffer is no longer empty.
 * Input: generic type pointer
 * Output: reference to producer thread
 */
void *producer(void *param) {
	buffer_item item;
	int rand_num = 1;

	while (true) {
		// sleep for a random period of time
        	rand_num = rand() % MAX_NUM;	// Random number will be no greater than 8
		sleep(rand_num);

		// generate a random number for item
		item = rand();

		// Wait for empty then acquire mutex lock
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		if (insert_item(item)){
			fprintf(stderr, "report error condition");
        	}
		else {
			printf("producer %d produced %d\n", pthread_self(), item);
        	}

		// Release mutex lock and signal full semaphore
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

/*
 * Description: Creates a consumer thread with a thread ID, the consumer will sleep for a random time less than
 * 		eight seconds and then attempt to remove an item from the buffer. The consumer will wait if the
 * 		buffer is empty. Mutex lock is acquired, the item removed, then mutex lock is released and
 * 		the empty semaphore is signalled to show that the buffer is no longer full.
 * Input: generic type pointer
 * Output: reference to consumer thread
 */
void *consumer(void *param) {
	buffer_item item;
	int rand_num = 1;

	while (true) {
		// sleep for a random period of time
		rand_num = rand() % MAX_NUM;	// Random number will be no greater than 8
		sleep(rand_num);

		// Wait for full then acquire mutex lock
		sem_wait(&full);
		pthread_mutex_lock(&mutex);

		if (remove_item(&item)){
			fprintf(stderr, "report error condition");
        	}
		else {
			printf("consumer %d consumed %d\n", pthread_self(), item);
        	}

		// Release mutex lock and signal empty semaphore
        	pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}

/*
 * Description: (The main function) Creates a number of producer and consumer threads as specified by
 *		the command line arguments. These threads will insert and consume items from the buffer until
 *		the program has exit.
 * Input: time for main to sleep, number of producer threads to be created, and number of consumer
 * 	  threads to be created.
 * Ouput: The user will see the producer and consumer threads inserting and removing items from the
 * 	  buffer
 */
int main(int argc, char *argv[]) {

	srand( time(NULL));

	// Get command line arguments argv[1], argv[2], argv[3]
	int mainSleepTime = atoi(argv[1]); 	// Time for 'main' to sleep (seconds)
	int producer_num = atoi(argv[2]); 	// User Defines the number of producer threads
   	int consumer_num = atoi(argv[3]); 	// User Defines the number of consumer threads
   	int i;                                  // Used for the thread creation loops

	pthread_t p_tid[producer_num];		// Producer thread id's
	pthread_t c_tid[consumer_num];		// Consumer thread id's
	pthread_attr_t attr;

	pthread_mutex_init(&mutex, NULL);	// Initialise the mutex lock

	// Two semaphores are created; one for empty, one for full
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);

	pthread_attr_init(&attr);

	counter = 0;	// Counter initialised to 0;

	// Create producer threads
   	for(i = 0; i < producer_num; i++) {
      		/* Create the thread */
      		pthread_create(&(p_tid[i]),&attr,&producer,NULL);
    	}

	// Create consumer threads
	for(i = 0; i < consumer_num; i++) {
      		/* Create the thread */
      		pthread_create(&(c_tid[i]),&attr,&consumer,NULL);
 	}

	// Main function will sleep for a time specified by the user
	sleep(mainSleepTime);	// Specify time

	// Exit the program
	printf("Program exited\n");
   	exit(0);

}
