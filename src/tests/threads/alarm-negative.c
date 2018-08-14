/* Tests timer_sleep(-100).  Only requirement is that it not crash. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"
#include "threads/malloc.h"


void
test_alarm_negative (void) 
{
  int* a = (int*)malloc(1500);

  printf("a = %u\n", a);
  *a = 15;
  print_memory();
  int* b = (int*)malloc(1000);
  printf("b = %d\n", *b);
  *b=8846;
  print_memory();
  int* c = (int*)malloc(25);
  printf("c = %u\n", c);
  *c=152;
  print_memory();
  
  int* d = (int*)malloc(200);
  printf("d = %d\n", *d);
  *d=8846;
  print_memory();
  int* e = (int*)malloc(300);
  printf("e = %d\n", *e);
  *e=8846;
  print_memory();
 // printf("a = %d\n", *a);
 // printf("b = %d\n", *b);
  printf("c = %d\n", *c);
  printf("..................FREE A.......................\n");
   printf("a = %d\n", *a);
  free(a);
  print_memory();
  printf("..................FREE C .......................\n");
  free(c);
  print_memory();
  printf("..................FREE B .......................\n");
  free(b);
  print_memory();
  printf("..................FREE D .......................\n");
  free(d);
  print_memory();
  printf("..................FREE E .......................\n");
  free(e);
  print_memory();
  

  pass ();
}
