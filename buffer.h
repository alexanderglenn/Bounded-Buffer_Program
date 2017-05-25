/********************************************************************
 *
 * Alexander Glenn
 * Date created: 05/10/2016
 * Date since last modification: 14/10/2016
 *
 * File: buffer.h
 *
 * Contains the constants, BUFFER_SIZE and MAX_NUM as well as
 * the function prototypes for insert_item, remove_item, and
 * the producer and consumer threads.
 *
 *******************************************************************/

#ifndef	BUFFER_H
#define	BUFFER_H

typedef int buffer_item;
#define BUFFER_SIZE	10
#define MAX_NUM		8    // Maximum value for the random number

// Function Prototypes
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *producer(void *param);
void *consumer(void *param);

#endif
