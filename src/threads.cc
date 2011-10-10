#include "threads.h"
#include "ps3.h"
#include "track.h"

MessageQueue::MessageQueue()
{}

MessageQueue::~MessageQueue()
{}

void MessageQueue::producer()
{
	track();
}

void MessageQueue::consumer()
{
	ps3();
}
