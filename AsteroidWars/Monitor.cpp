#include "stdafx.h"
#include "Monitor.h"

// Synchronisation is required so that a message is not deposited if the
// queue is full, and a message is not fetched if the queue is empty.

// Using signalling and continue (SC)
// Mutual exclusion is implicit, only one process can be 'accessing'
// the monitor at any one time.

Monitor::Monitor()
{
	front = 0;
	rear = 0;
	count = 0;
	n = 10;

	not_full = true;
	not_empty = false;
	newMessage = false;
}

Monitor::~Monitor(){}

void Monitor::Deposit(std::string data)
{
	// While queue is full
	while (count == n)
	{
		// Wait until not full
		while (not_full == true)
		{
			// Wait
			cout << "Here" << endl;
		}
	}
	buf[rear] = data;// Deposit message
	rear = (rear + 1) % n;

	count++;

	not_empty = false;// signal(not_empty)

	newMessage = true;
}

void Monitor::Fetch(std::string &result)
{
	// While queue is empty
	while (count == 0)
	{
		// Wait until not empty
		while (not_empty == true) 
		{
			// Wait
			cout << "Here2" << endl;
		}
	}
	result = buf[front];// Fetch message
	front = (front + 1) % n;

	count--;

	not_full = false;// signal(not_full)

	newMessage = false;
}

bool Monitor::NewMessage()
{
	return newMessage;
}