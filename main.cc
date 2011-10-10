#include "src/threads.h"
#include "glibmm.h"

int main(int, char**)
{
	/*
	Glib::RefPtr<Glib::MainLoop> main_loop;
	*/
	Glib::thread_init();

	MessageQueue queue;

	Glib::Thread *const consumer = Glib::Thread::create(
			sigc::mem_fun(queue, &MessageQueue::consumer), true);

	
	Glib::Thread *const producer = Glib::Thread::create(
			sigc::mem_fun(queue, &MessageQueue::producer), true);


  	producer->join();
	consumer->join();

	/*
	 * main_loop->run();
	 */
	return 0;
}
