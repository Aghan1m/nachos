// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "elevatortest.h"

//lab1
void MaxThreadTest();
void TsTest();

//lab2
void SchedPriorityTest();
void SchedRRTest();

//lab3
void ProducerConsumerSemaphoreTest();
void ProducerConsumerConditionTest();
void produce_sema(int n);
void consume_sema(int n);
void produce_cond(int n);
void consume_cond(int n);
void barrierFun(int num);
void BarrierTest();
int barrierCount = 0;

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 10; num++) {
	printf("*** thread %d looped %d times currentTimerTicks %d usedCpuTime %d\n", which, num, stats->totalTicks, currentThread->UsedCpuTime());
//interrupt->OneTick();
	interrupt->SetLevel(IntOff);
	interrupt->SetLevel(IntOn);
//        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, (void*)1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
    ThreadTest1();
    break;

    case 2:
    MaxThreadTest();
    break;

    case 3:
    TsTest();
    break;

	case 4:
	SchedPriorityTest();
	break;

	case 5:
	SchedRRTest();
	break;

	case 6:
	ProducerConsumerSemaphoreTest();
	break;

	case 7:
	ProducerConsumerConditionTest();
	break;

	case 8:
	BarrierTest();
	break;

    default:
	printf("No test specified.\n");
	break;
    }
}

void
MaxThreadTest()
{
    DEBUG('t', "Entering MaxThreadTest");

    for(int i=0; i<MaxThreadNum; i++)
    {
        Thread *t = new Thread("forked thread");
        printf("%d*** thread threadId: %d userId: %d\n",i, t->ThreadId(), t->UserId());

    }
}

// lab1: ts
void
TS()
{
    printf("Tid\tUid\tPri\tCPU\tStatus\tTime\n");
    for(int i=0; i<MaxThreadNum; i++)
    {
        if(threadPool[i]) 
        {
            threadPtrPool[i]->ShowThread();
        }
    }
}

void
TsTest()
{

    DEBUG('t', "Entering TsTest");

    //time_t timep;
    //time(&timep);
    //printf("%s\n",ctime(&timep));

    for(int i=0; i<5; i++)
    {
        Thread *t = new Thread("forked thread");
    }
    TS();
}

// lab2: priority scheduler
void
SchedPriorityTest()
{
	DEBUG('t', "Entering SchedPriorityTest");

	CurrentSchedStrategy = SCHED_PRIORITY;

	Thread *t1 = new Thread("forked thread", 1);
	Thread *t2 = new Thread("forked thread", 3);
	Thread *t3 = new Thread("forked thread", 2);
	Thread *t4 = new Thread("forked thread", 0);
	Thread *t5 = new Thread("forked thread", 5);

    t1->Fork(SimpleThread, (void*)1);
    t2->Fork(SimpleThread, (void*)2);
    t3->Fork(SimpleThread, (void*)3);
    t4->Fork(SimpleThread, (void*)4);
    t5->Fork(SimpleThread, (void*)5);
}

void
SchedRRTest()
{
	DEBUG('t', "Entering SchedRRTest");

	CurrentSchedStrategy = SCHED_RR;

	Thread *t1 = new Thread("forked thread", 1);
	Thread *t2 = new Thread("forked thread", 3);
	Thread *t3 = new Thread("forked thread", 2);
	Thread *t4 = new Thread("forked thread", 0);
	Thread *t5 = new Thread("forked thread", 5);

    t1->Fork(SimpleThread, (void*)1);
    t2->Fork(SimpleThread, (void*)2);
    t3->Fork(SimpleThread, (void*)3);
    t4->Fork(SimpleThread, (void*)4);
    t5->Fork(SimpleThread, (void*)5);
}

// lab3: producer and consumer test
//#define MAX_BUFFER_SIZE 6
int buffer = 0;

void
ProducerConsumerSemaphoreTest() 
{
	Thread* producer1 = new Thread("producer1");
	Thread* producer2 = new Thread("producer2");
	Thread* consumer1 = new Thread("consumer1");
	Thread* consumer2 = new Thread("consumer2");
	
	producer1->Fork(produce_sema, 8);
	producer2->Fork(produce_sema, 4);
	consumer1->Fork(consume_sema, 6);
	consumer2->Fork(consume_sema, 6);
}



void
produce_sema(int n)
{
	while(n>0) {
		if(buffer < MAX_BUFFER_SIZE) {
			empty->P();
			mutex->P();
			printf("producer thread %s producing! now buffer number is:%d \n", currentThread->getName(), buffer);
			buffer ++;
			n --;
			printf("producer thread %s produce completed! now buffer number is:%d \n", currentThread->getName(), buffer);
			full->V();
			mutex->V();
		} else {
			printf("producer thread %s producing! But buffer is full! buffer number is:%d \n", currentThread->getName(), buffer);
			currentThread->Yield();
		}
	}
	
}

void 
consume_sema(int n)
{
	while(n>0) {
		if(buffer > 0) {
			full->P();
			mutex->P();
			printf("consumer thread %s consuming! now buffer number is:%d \n", currentThread->getName(), buffer);
			buffer --;
			n --;
			printf("consumer thread %s consume completed! now buffer number is:%d \n", currentThread->getName(), buffer);
			empty->V();
			mutex->V();
		} else {
			printf("consumer thread %s consuming! But buffer is empty! buffer number is:%d \n", currentThread->getName(), buffer);
			currentThread->Yield();
		}
	}
}

void
ProducerConsumerConditionTest() 
{
	Thread* producer1 = new Thread("producer1");
	Thread* producer2 = new Thread("producer2");
	Thread* consumer1 = new Thread("consumer1");
	Thread* consumer2 = new Thread("consumer2");
	
	producer1->Fork(produce_cond, 6);
	producer2->Fork(produce_cond, 4);
	consumer1->Fork(consume_cond, 6);
	consumer2->Fork(consume_cond, 6);
}

void
produce_cond(int n)
{
	while(true) {
		pcLock->Acquire();
		while(currentSyncNum >= MAX_BUFFER_SIZE) {
			printf("producer thread %s producing! But sync number is full! sync number is:%d \n", currentThread->getName(), currentSyncNum);
			producerCondition->Wait(pcLock);	
		}

		printf("producer thread %s producing! now sync number is:%d \n", currentThread->getName(), currentSyncNum);
		currentSyncNum ++;
		printf("producer thread %s produce completed! now sync number is:%d \n", currentThread->getName(), currentSyncNum);
		// inform thread waiting for consuming
		consumerCondition->Signal(pcLock);
		pcLock->Release();
	}
	
}

void 
consume_cond(int n)
{
	while(true) {
		pcLock->Acquire();
		while(currentSyncNum <= 0) {
			printf("producer thread %s producing! But sync number is empty! sync number is:%d \n", currentThread->getName(), currentSyncNum);
			producerCondition->Wait(pcLock);	
		}
		printf("consumer thread %s consuming! now sync number is:%d \n", currentThread->getName(), currentSyncNum);
		currentSyncNum --;
		printf("consumer thread %s consume completed! now sync number is:%d \n", currentThread->getName(), currentSyncNum);
		// inform thread waiting for producing 
		producerCondition->Signal(pcLock);
		pcLock->Release();
	}
	
}

void
BarrierTest()
{
	for(int i=0; i<MAX_BARRIER_COUNT; i++) {
		Thread* t = new Thread("barrierThread");
		t->Fork(barrierFun, i+1);
	}	
}

void 
barrierFun(int num)
{
	barrierLock->Acquire();
	barrierCount ++;
	if(barrierCount == MAX_BARRIER_COUNT) {
		printf("threadName:%s%d, barrierCount:%d, requireCount:%d, Broadcast all threads.\n", currentThread->getName(), num, barrierCount, MAX_BARRIER_COUNT);
		barrierCondition->Broadcast(barrierLock);
		barrierLock->Release();
	} else {
		printf("threadName:%s%d, barrierCount:%d, requireCount:%d, Wait for signal.\n", currentThread->getName(), num, barrierCount, MAX_BARRIER_COUNT);
		barrierCondition->Wait(barrierLock);
		barrierLock->Release();
	}
	printf("threadName:%s%d, wake up, continue.\n", currentThread->getName(), num);
}
