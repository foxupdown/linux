/*
 * thread_poll.c
 *
 *  Created on: May 25, 2019
 *      Author: linux
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
typedef struct threadpool_work{
	void *(* progress)(void *arg);
	void *arg;
	struct threadpool_work *next;
}threadpool_work_t;
typedef struct threadpool{
	pthread_mutex_t queue_lock;
	pthread_cond_t  queue_ready;
	threadpool_work_t * queue_head;
	int shutdown;
	pthread_t * threadid;
	int max_thread_num;
	int cur_queue_num;
}threadpool_t;
void * work_thread(void *arg);
void * task_handle(void *arg);
static threadpool_t * pool = NULL;
void threadpool_init(int max_thread_num)
{
	int i = 0;
	pool = (threadpool_t *)malloc(sizeof(threadpool_t));
	if(NULL == pool){
		printf("fail to malloc threadpool_t\n");
		return ;}
	pthread_mutex_init(&(pool->queue_lock) , NULL);
	pthread_cond_init(&(pool->queue_ready) , NULL);
	pool->queue_head = NULL;
	pool->max_thread_num = max_thread_num;
	pool ->cur_queue_num = 0;
	pool->shutdown = 0;
	pool->threadid = (pthread_t *)malloc(max_thread_num * sizeof(pthread_t));
	for(i = 0 ; i < max_thread_num ; i ++)
	{
		pthread_create(&(pool->threadid[i]) , NULL , work_thread , NULL);
	}
}
int threadpool_work_add(void *(* progress)(void *arg) , void * arg)
{
	//构造一个新任务
	threadpool_work_t *newwork = (threadpool_work_t *)malloc(sizeof(threadpool_work_t));
	newwork->progress = progress;
	newwork->arg = arg;
	newwork->next = NULL;
	pthread_mutex_lock(&(pool->queue_lock));
	threadpool_work_t *member = pool->queue_head;
	if(NULL != member)
	{
		while(member ->next){
			member = member -> next;
		}
		member->next = newwork;
	}else{
		pool->queue_head = newwork;

	}
	assert(pool->queue_head != NULL);
	//if(pool->queue_head == NULL)
		//return -1;
	pool->cur_queue_num ++;
	pthread_mutex_unlock(&(pool->queue_lock));
	pthread_cond_signal(&(pool->queue_ready));
	return 0;
}
int threadpool_destory(void){
	int i = 0;
	pool->shutdown = 1;
	pthread_cond_broadcast(&(pool->queue_ready));
	for(i = 0 ; i < pool->max_thread_num ; i ++)
	{
		pthread_join(pool->threadid[i] , NULL);
	}
	free(pool->threadid);
	threadpool_work_t *head = NULL;
	while(pool->queue_head != NULL){
		head = pool->queue_head;
		pool ->queue_head = pool->queue_head -> next;
		free(head);
	}
	pthread_mutex_destroy(&(pool->queue_lock));
	pthread_mutex_destroy(&(pool->queue_ready));
	free(pool);
	pool = NULL;
	return 0;
}
void *work_thread(void * arg)
{
	printf("启动序号为0x%x的工作线程\n" , (unsigned int)pthread_self());
	while(1)
	{
		pthread_mutex_lock(&(pool->queue_lock));
		while(pool ->cur_queue_num == 0 && !(pool ->shutdown)){
			printf("序号为0x%x的工作线程等待分配任务\n" , (unsigned int)pthread_self());
			pthread_cond_wait(&(pool ->queue_ready) , &(pool ->queue_lock));
		}
		if(pool ->shutdown){
			pthread_mutex_unlock(&(pool ->queue_lock));
			printf("the 0x%x thread is going out\n" , (unsigned int)pthread_self());
			pthread_exit(NULL);
		}
		printf("the 0x%x thread is going to handler\n" , (unsigned int)pthread_self());
		pool->cur_queue_num --;
		threadpool_work_t *worker = pool ->queue_head;
		pool ->queue_head = worker -> next;
		pthread_mutex_unlock(&(pool->queue_lock));
		(*(worker->progress))(worker->arg);
		free(worker);
		worker = NULL;
	}
	return NULL;
}
void * task_handle(void *arg){
	printf("the thread 0x%x is do handler\n" , (unsigned int)pthread_self());
	sleep(1);
	return NULL;
}
int main()
{
	printf("hello world\n");
	int i = 0;
	threadpool_init(5);
	int *task = (int * )malloc(sizeof(int )*50);
	for(i = 0 ; i < 50 ; i ++){
		task[i] = i;
		threadpool_work_add(task_handle , &task[i]);
	}
	sleep(10);
	threadpool_destory();
	free(task);
    return 0;
}
