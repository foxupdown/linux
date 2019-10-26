#include "myThread.h"
myThread::myThread():mTid(-1){}
bool myThread::start()
{
	int res;
	res = pthread_create(&mTid,NULL,runProxy,this);
	return res == 0;
}
void *myThread::runProxy(void *self)
{
	myThread *current = reinterpret_cast<myThread *>(self);
	current ->mTid = pthread_self();
	current ->run();
	return NULL;
}
bool myThread::isRunning(const myThread &ref)
{
	int res = pthread_kill(ref.mTid , 0);
	return res == 0;
}
unsigned int myThread::getTid()
{
	return static_cast<unsigned int>(mTid);
}
void myThread::join()
{
	pthread_join(mTid , NULL);
}
