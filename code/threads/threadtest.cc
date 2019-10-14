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
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
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
    printf("Tid\tUid\tPriority\tStatus\tTime\n");
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
