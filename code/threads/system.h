// system.h 
//	All global variables used in Nachos are defined here.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef SYSTEM_H
#define SYSTEM_H

#include "copyright.h"
#include "utility.h"
#include "thread.h"
#include "scheduler.h"
#include "interrupt.h"
#include "stats.h"
#include "timer.h"
#include "synch.h"

// Initialization and cleanup routines
extern void Initialize(int argc, char **argv); 	// Initialization,
						// called before anything else
extern void Cleanup();				// Cleanup, called when
						// Nachos is done.
						//
// lab1
#define MaxThreadNum 128
extern int threadPool[MaxThreadNum];
extern Thread* threadPtrPool[MaxThreadNum];

// lab2
enum SchedulerStrategy { SCHED_FIFO, SCHED_PRIORITY, SCHED_RR } ;
extern SchedulerStrategy CurrentSchedStrategy;

// lab3
#define MAX_BUFFER_SIZE 6
#define MAX_BARRIER_COUNT 6
extern Semaphore* mutex;
extern Semaphore* empty;
extern Semaphore* full;

extern Condition* consumerCondition;
extern Condition* producerCondition;
extern Lock* pcLock;
extern int currentSyncNum;
extern Condition* barrierCondition;
extern Lock* barrierLock;

extern Thread *currentThread;			// the thread holding the CPU
extern Thread *threadToBeDestroyed;  		// the thread that just finished
extern Scheduler *scheduler;			// the ready list
extern Interrupt *interrupt;			// interrupt status
extern Statistics *stats;			// performance metrics
extern Timer *timer;				// the hardware alarm clock

#ifdef USER_PROGRAM
#include "machine.h"
extern Machine* machine;	// user program memory and registers
#endif

#ifdef FILESYS_NEEDED 		// FILESYS or FILESYS_STUB 
#include "filesys.h"
extern FileSystem  *fileSystem;
#endif

#ifdef FILESYS
#include "synchdisk.h"
extern SynchDisk   *synchDisk;
#endif

#ifdef NETWORK
#include "post.h"
extern PostOffice* postOffice;
#endif

#endif // SYSTEM_H
