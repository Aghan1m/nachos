// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

int TlbReplaceIdx = 0;
void
TLBMissHandler(int virtAddr) {
	unsigned int vpn;
	vpn = (unsigned) virtAddr/PageSize;
	machine->tlb[TlbReplaceIdx] = machine->pageTable[vpn];
	TlbReplaceIdx = TlbReplaceIdx?0:1;
}


void FIFOSwap(int virtAddr) {
    int TLBreplaceIdx = -1;
	unsigned int vpn = (unsigned) virtAddr/PageSize;
	printf("FIFO virtual address is: %d, vpn is: %d\n", virtAddr);
    for (int i = 0; i < TLBSize; i++) {
        if (machine->tlb[i].valid == FALSE) {
            TLBreplaceIdx = i;
            break;
        }
    }
    if (TLBreplaceIdx == -1) {
        TLBreplaceIdx = TLBSize - 1;
        for (int i = 0; i < TLBSize - 1; i++) {
            machine->tlb[i] = machine->tlb[i+1];
        }
    }
    machine->tlb[TLBreplaceIdx] = machine->pageTable[vpn];
}

void LRUSwap(int virtAddr) {
    int TLBreplaceIdx = -1;
	unsigned int vpn = (unsigned) virtAddr/PageSize;
	printf("LRU virtual address is: %d, vpn is: %d\n", virtAddr);
    for (int i = 0; i < TLBSize; i++) {
        if (machine->tlb[i].valid == FALSE) {
            TLBreplaceIdx = i;
			machine->tlb[i].lastUsedTime = stats->totalTicks;
            break;
        }
    }

    if (TLBreplaceIdx == -1) {
		int theEarliest = machine->tlb[0].lastUsedTime;
		TLBreplaceIdx = 0;
        for (int i = 1; i < TLBSize; i++) {
            if(theEarliest > machine->tlb[i].lastUsedTime) {
				theEarliest = machine->tlb[i].lastUsedTime;
				TLBreplaceIdx = i;
			}
        }
    }
    machine->tlb[TLBreplaceIdx] = machine->pageTable[vpn];
}
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
	// lab vm: Page Fault handle
	if(which == PageFaultException) {
		if(machine->tlb==NULL) {
			DEBUG('m', "=> PageTable Page Fault.\n");
			ASSERT(FALSE);
		} else { // lab vm: tlb miss
			DEBUG('m', "=> TLB Miss. \n");
			int BadVAddr = machine->ReadRegister(BadVAddrReg);
//			TLBMissHandler(BadVAddr);
//			FIFOSwap(BadVAddr);
			LRUSwap(BadVAddr);
		}
		return;
	}
   
	int type = machine->ReadRegister(2);

    if ((which == SyscallException) && (type == SC_Halt)) {
    DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    } else {
    printf("Unexpected user mode exception %d %d\n", which, type);
    ASSERT(FALSE);
    }
}


