#include "threads/thread.h"
#include "threads/signal.h"

#include <debug.h>
#include <stddef.h>
#include <random.h>
#include <stdio.h>
#include <string.h>
#include "threads/flags.h"
#include "threads/interrupt.h"
#include "threads/intr-stubs.h"
#include "threads/palloc.h"
#include "threads/switch.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#ifdef USERPROG
#include "userprog/process.h"
#endif

/*
static int child_hand=0;
static int cpu_hand=0;
static int unblock_hand=0;
static int usr_hand=0;
*/
void CHLD_handler(tid_t tid)
{
	
	struct thread *cur= running_thread ();
	struct list_elem *e;

  	for (e = list_begin (&(cur->child_list)); e != list_end (&(cur->child_list));
       e = list_next (e))
    {
      struct child *t = list_entry (e, struct child, elem);
      if(t->tid==tid)
        {
        	list_remove(e);
        	break;
        }
    }

    printf("Total Number of children created by thread (tid=%d) =%d\n",cur->tid,cur->no_of_child_created);
    cur->no_of_child_died++;
    printf("Number of children died for thread(tid=%d) =%d\n",cur->tid,cur->no_of_child_died);

	
    return ;
}

void KILL_handler()
{
	printf("killing the thread(tid=%d) \n",running_thread()->tid);
	thread_exit();	// send signal to its parent just before the thread_exist finish
	return ;
}

void CPU_handler()
{
	
	printf("MAX CPU lifetime is=%d  \n",running_thread()->max_lifetime);
	printf("CPU lifetime is crossed so killing the thread(tid=%d)\n",running_thread()->tid);
	thread_exit();

		// check once
	return ;
}

void UNBLOCK_handler(tid_t tid)
{

	struct thread *t=get_thread(tid);
	if(t->status == THREAD_BLOCKED)
	{
		//printf("thread(tid=%d) is getting unblocked--------------------------------------\n",t->tid);
		thread_unblock(t);
	}
	
	
	return ;
}

void USR_handler(tid_t tid)
{
	
	struct thread *t=running_thread ();
	printf("signal is send to thread %d by thread %d\n",t->tid,tid);
	return ;
}

void signal(enum signal_name name,enum handler_status status)
{
	//printf("signal function gets called\n");
	struct bitmap *temp_bm = running_thread()->sig_ign;
	
	if(status == SIG_IGN)
		bitmap_set(temp_bm, name, true);
	else
		bitmap_set(temp_bm, name, false);

}


int kill(tid_t tid, enum signal_name name)
{
	if(name==SIG_CHLD||name==SIG_CPU)
		return -1;	// error
	else
	{
		struct thread *t=get_thread(tid);
		if(t==NULL)
			return -1;	// thread doesnt exist
		
		if(name == SIG_KILL)
		{
			
			struct thread *cur= running_thread ();
			
			if(t->tid_parent == cur->tid)
			{
				
				struct signal *temp2=(struct signal *)malloc(sizeof *temp2);;
				temp2->name=name;
				temp2->sender=(cur->tid);
				

				list_push_back (&(t->signal_list), &(temp2->elem));
				
				return 0;
			}
			else
			{
				
				return -1;
			}
		}

		// doubt here check addition to list .....tenp is not pointer
		struct signal *temp=(struct signal *)malloc(sizeof *temp);
		temp->name=name;
		struct thread *run_t = running_thread();
		temp->sender=(run_t->tid);
		printf("adding %d in kill\n", name);
		list_push_back (&(t->signal_list), &(temp->elem));
		return 0;	// success
	}
}


struct bitmap * sigemptyset(struct bitmap *set)
{
	printf("setting value to false------------------------------------------------\n");
	set=bitmap_create(4);
	return set;
}

struct bitmap * sigfillset(struct bitmap *set)
{
	printf("setting value to true------------------------------------------------\n");
	set = bitmap_create(4);
    bitmap_set_all(set, true);
	return set;
}

struct bitmap * sigdelset(struct bitmap *set, enum signal_name name)
{
	printf("calling sigdelset function -------------------------------------\n");
	size_t idx = name;
	bitmap_reset(set, idx);
	return set;
}

struct bitmap * sigaddset(struct bitmap *set, enum signal_name name)
{
	printf("calling sigaddset function -------------------------------------\n");
	size_t idx = name;

	bitmap_set(set, idx, true);
	
	//printf("over\n");
	return set;
}

int  sigprocmask(enum sigproc_status status , struct bitmap *set, struct bitmap *old_set)
{
	printf("calling sigprocmask function -------------------------------------\n");
	struct thread *t = running_thread();

	int i;

	// Copy sig_mask to old_set

	if(t->sig_mask != NULL && old_set != NULL)
	{
		for(i=0;i<4;i++)
		{
			if(bitmap_test(t->sig_mask ,i))
				bitmap_set(old_set, i, true);
			else
				bitmap_set(old_set, i, false);			
		}
	}

	if(status == SIG_BLOCK)
	{
		printf("calling BLK------\n");
		for(i=0;i<4;i++)
		{
			if(bitmap_test(set ,i))
				bitmap_set(t->sig_mask, i, true);
		}

	}
	else if(status == SIG_UNBLK)
	{
		printf("calling SIGUNBLK------\n");
		int i;
		for(i=0;i<4;i++)
		{
			if(bitmap_test(set ,i))
				bitmap_set(t->sig_mask, i, false);
		}

	}
	else if(status == SIG_SETMASK)
	{
		printf("calling SIGSETMASK------\n");
		for(i=0;i<4;i++)
		{
			if(bitmap_test(set ,i))
				bitmap_set(t->sig_mask, i, true);
			else
				bitmap_set(t->sig_mask, i, false);			
		}
				
	}
	else
		return -1;//error
}
