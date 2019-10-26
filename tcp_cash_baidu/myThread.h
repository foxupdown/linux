#ifndef MY_THREAD__H
#define MY_THREAD__H
#include <pthread.h>
#include <signal.h>
class myThread{
	public:
		myThread();
		~myThread(){}
		bool start();
		static void *runProxy(void *self);
		static bool isRunning(const myThread &ref);
		unsigned int getTid();
		void join();
	private:
		virtual void run() = 0;
		pthread_t mTid;
};
#endif
