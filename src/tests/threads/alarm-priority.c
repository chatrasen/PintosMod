/* Checks that when the alarm clock wakes up threads, the
   higher-priority threads run first. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"
#include "threads/signal.h"
#include "threads/interrupt.h"
#include <bitmap.h>

/*static thread_func alarm_priority_thread;
static int64_t wake_time;
static struct semaphore wait_sema;
static int flag=0;
static int test_var=0;
void my_func(void);
*/



/*
static void
alarm_priority_thread (void *aux UNUSED) 
{

  // Busy-wait until the current time changes. 
  int64_t start_time = timer_ticks ();
 // while(1)
 while (timer_elapsed (start_time) == 0)
   continue;
  int i;


  if(running_thread()->tid==5)
  {
    //signal(SIG_USR,SIG_IGN);
  }


  if(running_thread()->tid==6)
  {
    int i;
    for(i=0;i<3;i++)
    {
      int priority = PRI_DEFAULT - (i + 5) % 10 - 1;
      thread_create("vedant",priority, my_func, NULL);
    }
   
  }

  if(running_thread()->tid == 4)
  {
   //   printf("=================kuch toh print kro\n");
      struct bitmap *set;
      set=sigemptyset(set);
      set=sigfillset(set);
      set=sigemptyset(set);
      if(set==NULL)
        printf("hi====\n");
      else
      {
        printf("bye------\n");
        //set=sigaddset(set,SIG_USR);
        //set=sigdelset(set,SIG_USR);
        sigprocmask(SIG_SETMASK,set,NULL);
      }
      
     // sigaddset(set,SIG_UBLOCK);
     // sigprocmask(SIG_SETMASK,set,NULL);
  }
  
  if(running_thread()->tid==6)
  {
      printf("block  the thread\n");
      enum intr_level old_level;
      old_level = intr_disable ();
      thread_block();
      intr_set_level (old_level);
  }

  if(running_thread()->tid==6)
  {
    printf("wakes up.....got unblocked\n");
    printf("killing the child having tid=%d\n",test_var);
  //  struct thread *t=get_thread(tid);
    //printf("parent of t is tid=%d\n",t-.tid);
    kill(test_var,SIG_KILL);
  }
  // Now we know we're at the very beginning of a timer tick, so
    // we can call timer_sleep() without worrying about races
    // between checking the time and a timer interrupt. 
  else
  {
      timer_sleep (wake_time - timer_ticks ());
  }
  
 // printf("hi hi hI hI ihsopvbdfojbgd\n");
  // Print a message on wake-up. 
  msg ("Thread %s woke up.", thread_name ());

  sema_up (&wait_sema);
}
 
void my_func(void)
{
    printf("usr signal parent my_tid=%d,my parent tid=%d\n",running_thread()->tid,running_thread()->tid_parent);
    kill(running_thread()->tid_parent,SIG_USR);
    if(flag==0)
    {
      flag=1;
      test_var=running_thread()->tid;
      printf("sending unblocking signal my tid=%d to thread having tid=3\n",running_thread()->tid);
      kill(6,SIG_UNBLOCK);

      while(1);
    }
    else
    {
      printf("usr signal my_tid=%d to tid=4----------------------------------------\n",running_thread()->tid);
      kill(4,SIG_USR);
      kill(5,SIG_USR);
    }
    
   // sleep(3);
    return ;
}
*/
struct semaphore sem; 

static void sleeper1(void* t) {
  struct thread* curr = thread_current();
  tid_t id = curr->tid_parent;
  kill(id,SIG_USR);
  timer_msleep(2000);
  printf("child exiting...\n");
}

static void sleeper2(void* t){
  setlifetime(100);
  //timer_sleep(1500);
  while(1);
}

static void sleeper3(void* t){
  /*sema_init (&sem, 0);
  printf("sema init ho gaya\n");
  sema_down(&sem);*/
  enum intr_level old_level;
  old_level = intr_disable ();
  thread_block();
  intr_set_level (old_level);
  printf("unblocked\n");
  while(1);
}

static struct bitmap *int_to_bitmap(int x){
  struct bitmap *bm = bitmap_create(4);
  int idx,power;
  for(idx=0,power=1; idx<4; idx++,power*=2){
    if(x & power){
      bitmap_set(bm,idx,true);
    }
  }
  return bm;
}

void ass6_1(){
  thread_create("1", 0, sleeper1, NULL);
  while(1);
}

void ass6_2(){
  thread_create("1", 0, sleeper1, NULL);
  signal(SIG_USR,SIG_IGN);
  signal(SIG_CHLD,SIG_IGN);
  while(1);
}

void ass6_3(){
  thread_create("1", 0, sleeper2, NULL);
  while(1);
}

void ass6_4(){
  tid_t id = thread_create("1", 0, sleeper3, NULL);
  printf("%d\n",id);
  timer_sleep(40);
  kill(id,SIG_UNBLOCK);
  timer_sleep(40);
  kill(id,SIG_KILL);
  timer_sleep(40);
}

void ass6_5(){
  int x = 15;

  sigprocmask(SIG_BLOCK,int_to_bitmap(x),NULL);
  ass6_1();
}

void
test_alarm_priority (void) 
{

  /*
  thread_create("1", 0, sleeper1, NULL);  
  timer_msleep(5000);
  */

  /*
  thread_create("1", 0, sleeper1, NULL);
  signal(SIG_USR,SIG_IGN);
  signal(SIG_CHLD,SIG_IGN);
  timer_msleep(5000);
  */

  /*
  thread_create("1", 0, sleeper2, NULL);
  timer_msleep(5000);
  */

  /*
  tid_t id = thread_create("1", 0, sleeper3, NULL);
  printf("id(in alarm_priority) = %d\n",id);
  timer_sleep(40);
  kill(id,SIG_UNBLOCK);
  timer_sleep(40);
  kill(id,SIG_KILL);
  timer_sleep(40);   
  */

  
  int x = 15;

  sigprocmask(SIG_BLOCK,int_to_bitmap(x),NULL);
  ass6_1();
  
}