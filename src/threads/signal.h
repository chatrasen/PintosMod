#ifndef THREADS_SIGNAL_H
#define THREADS_SIGNAL_H

#include <debug.h>
#include <list.h>
#include <stdint.h>
#include "threads/thread.h"


enum signal_name
  {
    SIG_CHLD,
    SIG_CPU,
    SIG_UNBLOCK,
    SIG_USR,
    SIG_KILL
  };

enum handler_status
{
	SIG_DFL,
	SIG_IGN
};
struct signal
{
	enum signal_name name;
	tid_t sender;
	struct list_elem elem;

};

enum sigproc_status
{
  SIG_BLOCK,
  SIG_UNBLK,
  SIG_SETMASK
};


void CHILD_handler(tid_t tid);	// tid stores id of child which got terminated
void KILL_handler();
void CPU_handler();	// iska peace maara hai aab tak
void UNBLOCK_handler(tid_t tid);
void USR_handler(tid_t tid);	// tid stores tid of sender

void signal(enum signal_name name,enum handler_status status);// return 0 or 1 corresponding

int kill(tid_t tid, enum signal_name name); // return 0 or -1 success or faliure and tid stores id of receiver

struct bitmap * sigemptyset(struct bitmap *set);
struct bitmap * sigfillset(struct bitmap *set);
struct bitmap * sigdelset(struct bitmap *set, enum signal_name name);
struct bitmap * sigaddset(struct bitmap *set, enum signal_name name);
int sigprocmask(enum sigproc_status status , struct bitmap *set, struct bitmap *old_set);


#endif /* threads/signal.h */
