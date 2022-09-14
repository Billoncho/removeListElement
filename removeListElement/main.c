/*
Author: Billy Gene Ridgeway
Date: 13 Sept. 2022
Purpose: Learn how to remove an element from a list in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This what is going to be in the queue.
typedef struct listitem {
	struct listitem *next;			// Pointer to the next item.
	struct listitem *prev;			// Pointer to the previous item.
	int data;						// Some integer data.
} LISTITEM;

// This is the queue 'header.'
typedef struct {
	struct listitem *first;			// Pointer to the 1st. item.
	struct listitem *last;			// Pointer to the last item.
} LISTHDR;

LISTHDR head;						// Our queue.

									// This puts an item in at the end of a queue.
void enqueue(LISTHDR *queue, LISTITEM *item) {
	LISTITEM *temp;

	temp = queue->last;				// Get the 'last' item in the queue and keep hold of it.
	queue->last = item;				// Put the item in the queue at the end.
	item->prev = temp;				// Link back to old 'last' item.
	item->next = (LISTITEM*)queue;	// Set the forward link of the new item.
	temp->next = item;				// Finally, set the forward link of the old 'last' item to the new one.
}

// This removes an item from the front of a queue and returns the item or NULL if there are no more items.
LISTITEM *dequeue(LISTHDR *queue) {
	LISTITEM *temp;

	temp = queue->first;				// Get the 'first' item.
	if (temp == (LISTITEM*)queue) {		// If the head of the queue points to itself,
		temp = NULL;					// then the queue is empty.			
	} else {
		queue->first = temp->next;		// Set the queue header to point to the 'second' item.
		queue->first->prev = (LISTITEM*)queue;
	}
	return temp;
}

// Returns the number of items in a queue.
int queue_length(LISTHDR *queue) {
	LISTITEM *temp;
	int length;

	temp = queue->first;			// Get the 'first' item.
	length = 0;						// Initialize the length.
	do {
		// Check for an empty queue or if we've gone through the whole queue.
		if (temp == (LISTITEM*)queue) {
			temp = NULL;			// This will break out of the do ... while loop.
			break;
		}
		temp = temp->next;			// Get the next item in the queue.
		length = length + 1;
	} while (temp != NULL);

	return length;
}

// This removes an item from 'position' in a queue and returns the item or NULL if the position does not exist.
// NOTE: This uses a similar queue iteration technique to that used in 'queue_length.'
LISTITEM *remove_at(LISTHDR *queue, int position) {
	LISTITEM *temp;
	int i;

	// NOTE: We don't deal with negative positions in this example, but we could use a negative position to
	// indicate that we want to use the backward pointers as the position.
	if (position < 0) {
		return NULL;
	}

	temp = queue->first;			// Get the 'first' item.
	i = 0;
	do {
		// Here, check for an empty queue or if we've gone through the whole queue.
		if (temp == (LISTITEM*)queue) {
			temp = NULL;			// This will break out of the do ... while loop.
			break;
		}
		if (i == position) {
			// This is where we disconnect the queue item we've found.
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			// NOTE: Setting the pointers to NULL for the removed item isn't necessary,
			// but it might be a good idea to avoid accidental reuse of the pointers
			// since temp's next and prev still point into elements of the queue.
			temp->next = NULL;
			temp->prev = NULL;
			break;
		}
		temp = temp->next;			// Get the next item in the queue,
		i = i + 1;					// and increment the corresponding index position.
	} while (temp != NULL);

	return temp;
}

int main() {
	LISTITEM *temp;
	int requested_index;
	char input[50];

	// First, make an empty queue,
	// which is a queue where the header points to itself and there are no items in it.
	head.first = (LISTITEM*)&head;
	head.last = (LISTITEM*)&head;

	for (int i = 0; i < 5; i++) {			// As before, populate the queue.
		temp = malloc(sizeof(LISTITEM)); 	// Allocate some memory for the new queue item.
		temp->data = i;						// Set the item's data to the loop count so that we can see where it is in the queue,
		enqueue(&head, temp);				// and put it in the queue.
	}

	setbuf(stdout, NULL); 	// NOTE: If you are using an IDE such as CodeLite and nothing is initially shown 
							// in its Terminal you may need to uncomment this to turn off buffering.

	printf("\nThe length of the queue is %d.\n", queue_length(&head));
	// Remove at a user entered index.
	printf("Enter the index of the queue entry ... ");
	requested_index = atoi(gets(input));
	// do {							// keep going ...
	temp = remove_at(&head, requested_index);	// If we can't do it we will get NULL returned.
	if (temp != NULL) {
		printf("\nOK: data removed at %d is %d.\n", requested_index, temp->data);
		free(temp);				// Call 'free' to clean up. 
	} else {
		printf("ERROR: Cannot remove at %d\n", requested_index);
	}
	//	} while (temp != NULL);

	// See what we've got left.
	printf("\nThe length of the queue is %d\n", queue_length(&head));
	do {							// Keep going until the queue is empty.
		temp = dequeue(&head);		// If the queue is empty we will get NULL returned.
		if (temp != NULL) {
			printf("data is %d\n", temp->data);
			free(temp);				// Call 'free' to clean up.
		}
	} while (temp != NULL);

	return 0;
}