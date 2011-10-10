#include <iostream>
#include <queue>
#include <glibmm/random.h>
#include <glibmm/thread.h>
#include <glibmm/timer.h>

class MessageQueue : public sigc::trackable
{
public:
  MessageQueue();
  ~MessageQueue();

  void producer();
  void consumer();

private:
  Glib::Mutex     mutex_;
  Glib::Cond      cond_push_;
  Glib::Cond      cond_pop_;
  std::queue<int> queue_;
};

